/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <CGEcho/CGEcho.h>
#import <XCTest/XCTest.h>

#import "CGEchoFakeTransport.h"
#import "CGEchoInternal.h"

static NSString* const DeviceAddress = @"192.0.2.10";
static const CGEchoEOJ LightEOJ = 0x029001;
static const CGEchoEOJ AirconEOJ = 0x013001;

static NSData* Byte1(uint8_t b)
{
  return [NSData dataWithBytes:&b length:1];
}

/// A simple simulated device used by the responder.
@interface CGEchoTestDevice : NSObject
@property (atomic) BOOL respondToSearch;
@property (atomic) BOOL silent;
@property (atomic, copy) NSArray<NSNumber*>* objects;
@property (atomic, strong) NSMutableDictionary<NSNumber*, NSData*>* values; // EPC -> EDT
@property (atomic, strong) NSMutableSet<NSNumber*>* snaEPCs;
@end

@implementation CGEchoTestDevice
- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  _respondToSearch = YES;
  _objects = @[ @(AirconEOJ), @(LightEOJ) ];
  _values = [NSMutableDictionary dictionary];
  _values[@0x80] = Byte1(0x30);
  // 0x9F: 0x80, 0x9D, 0x9E, 0x9F. 0x9E: 0x80 only.
  const uint8_t getMap[] = { 4, 0x80, 0x9D, 0x9E, 0x9F };
  const uint8_t setMap[] = { 1, 0x80 };
  _values[@0x9F] = [NSData dataWithBytes:getMap length:sizeof(getMap)];
  _values[@0x9E] = [NSData dataWithBytes:setMap length:sizeof(setMap)];
  _snaEPCs = [NSMutableSet setWithObject:@0x9D];
  return self;
}

- (NSArray<CGEchoFrame*>*)reply:(CGEchoFrame*)request address:(NSString*)address
{
  if (self.silent)
    return nil;
  CGEchoFrameProperty* prop = request.properties.firstObject;
  if (!address) {
    if (self.respondToSearch && request.ESV == CGEchoESVGet && prop.EPC == CGEchoEPCSelfNodeInstanceListS)
      return @[ CGEchoTestReply(request, CGEchoESVGetResponse, prop.EPC, CGEchoTestInstanceList(self.objects)) ];
    return nil;
  }
  if (request.ESV == CGEchoESVGet) {
    NSData* value = self.values[@(prop.EPC)];
    if (!value || [self.snaEPCs containsObject:@(prop.EPC)])
      return @[ CGEchoTestReply(request, CGEchoESVGetSNA, prop.EPC, nil) ];
    return @[ CGEchoTestReply(request, CGEchoESVGetResponse, prop.EPC, value) ];
  }
  if (request.ESV == CGEchoESVSetC) {
    if ([self.snaEPCs containsObject:@(prop.EPC)])
      return @[ CGEchoTestReply(request, CGEchoESVSetCSNA, prop.EPC, prop.data) ];
    self.values[@(prop.EPC)] = prop.data;
    return @[ CGEchoTestReply(request, CGEchoESVSetResponse, prop.EPC, nil) ];
  }
  return nil;
}
@end

@interface CGEchoControllerTests : XCTestCase <CGEchoControllerDelegate>
@property (nonatomic, strong) CGEchoFakeTransport* transport;
@property (nonatomic, strong) CGEchoTestDevice* device;
@property (nonatomic, strong) CGEchoController* controller;
@property (atomic) NSUInteger addedCount;
@property (atomic) NSUInteger updatedCount;
@end

@implementation CGEchoControllerTests

- (CGEchoControllerConfiguration*)configuration
{
  CGEchoControllerConfiguration* config = [CGEchoControllerConfiguration defaultConfiguration];
  config.callbackQueue = dispatch_queue_create("org.cybergarage.uecho.test.callback", DISPATCH_QUEUE_SERIAL);
  config.requestTimeout = 0.3;
  config.discoveryTimeout = 0.5;
  return config;
}

- (void)setUp
{
  self.transport = [[CGEchoFakeTransport alloc] init];
  self.device = [[CGEchoTestDevice alloc] init];
  CGEchoTestDevice* device = self.device;
  self.transport.responder = ^NSArray<CGEchoFrame*>*(CGEchoFrame* request, NSString* address) {
    return [device reply:request address:address];
  };
  self.controller = [[CGEchoController alloc] initWithConfiguration:[self configuration] transport:self.transport];
  self.controller.delegate = self;
  self.addedCount = 0;
  self.updatedCount = 0;
}

- (void)tearDown
{
  XCTestExpectation* stopped = [self expectationWithDescription:@"stop"];
  [self.controller stopWithCompletion:^{ [stopped fulfill]; }];
  [self waitForExpectations:@[ stopped ] timeout:3];
}

- (void)echoController:(CGEchoController*)controller didAddNode:(CGEchoRemoteNode*)node
{
  self.addedCount++;
}

- (void)echoController:(CGEchoController*)controller didUpdateNode:(CGEchoRemoteNode*)node
{
  self.updatedCount++;
}

#pragma mark Helpers

- (void)start
{
  XCTestExpectation* started = [self expectationWithDescription:@"start"];
  [self.controller startWithCompletion:^(NSError* error) {
    XCTAssertNil(error);
    [started fulfill];
  }];
  [self waitForExpectations:@[ started ] timeout:3];
}

- (NSArray<CGEchoRemoteNode*>*)discover
{
  __block NSArray* result = nil;
  XCTestExpectation* found = [self expectationWithDescription:@"discover"];
  [self.controller discoverWithTimeout:0.3 completion:^(NSArray<CGEchoRemoteNode*>* nodes, NSError* error) {
    XCTAssertNil(error);
    result = nodes;
    [found fulfill];
  }];
  [self waitForExpectations:@[ found ] timeout:3];
  return result;
}

- (CGEchoRemoteObject*)startAndDiscoverObject:(CGEchoEOJ)eoj
{
  [self start];
  NSArray<CGEchoRemoteNode*>* nodes = [self discover];
  XCTAssertEqual(nodes.count, 1u);
  return [nodes.firstObject objectWithEOJ:eoj];
}

- (NSArray<CGEchoFrame*>*)sentFramesWithESV:(CGEchoESV)esv
{
  return [self.transport.sentFrames filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(CGEchoFrame* f, NSDictionary* b) { return f.ESV == esv; }]];
}

- (NSError*)readError:(CGEchoEPC)epc object:(CGEchoRemoteObject*)object
{
  __block NSError* result = nil;
  XCTestExpectation* done = [self expectationWithDescription:@"read"];
  [self.controller readProperty:epc ofObject:object timeout:0 completion:^(CGEchoPropertyValue* value, NSError* error) {
    result = error;
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];
  return result;
}

- (NSError*)write:(CGEchoEPC)epc data:(NSData*)data object:(CGEchoRemoteObject*)object options:(CGEchoWriteOptions)options
{
  __block NSError* result = nil;
  XCTestExpectation* done = [self expectationWithDescription:@"write"];
  [self.controller writeProperty:epc data:data ofObject:object options:options timeout:0 completion:^(NSError* error) {
    result = error;
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];
  return result;
}

- (void)settle:(NSTimeInterval)seconds
{
  XCTestExpectation* wait = [self expectationWithDescription:@"settle"];
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{ [wait fulfill]; });
  [self waitForExpectations:@[ wait ] timeout:seconds + 2];
}

#pragma mark Lifecycle (U08)

- (void)testStartStopIsIdempotent
{
  [self start];
  [self start];
  XCTAssertEqual(self.controller.state, CGEchoControllerStateRunning);
  XCTAssertEqual(self.transport.startCount, 1u);
  XCTestExpectation* stopped = [self expectationWithDescription:@"stop"];
  [self.controller stopWithCompletion:^{ [stopped fulfill]; }];
  [self waitForExpectations:@[ stopped ] timeout:3];
  XCTAssertEqual(self.controller.state, CGEchoControllerStateStopped);
  [self start];
  XCTAssertEqual(self.transport.startCount, 2u);
}

- (void)testStartFailureIsReported
{
  self.transport.failStart = YES;
  XCTestExpectation* started = [self expectationWithDescription:@"start"];
  [self.controller startWithCompletion:^(NSError* error) {
    XCTAssertEqual(error.code, CGEchoErrorTransportFailure);
    [started fulfill];
  }];
  [self waitForExpectations:@[ started ] timeout:3];
  XCTAssertEqual(self.controller.state, CGEchoControllerStateStopped);
}

- (void)testRequestsBeforeStartFail
{
  XCTestExpectation* done = [self expectationWithDescription:@"discover"];
  [self.controller discoverWithTimeout:0 completion:^(NSArray* nodes, NSError* error) {
    XCTAssertEqual(error.code, CGEchoErrorNotRunning);
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];
}

- (void)testStopFailsPendingOnceBeforeStopCompletion
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.device.silent = YES;
  __block NSUInteger calls = 0;
  __block BOOL stopCompleted = NO;
  XCTestExpectation* read = [self expectationWithDescription:@"read"];
  XCTestExpectation* stopped = [self expectationWithDescription:@"stop"];
  [self.controller readProperty:0x80 ofObject:object timeout:5 completion:^(CGEchoPropertyValue* value, NSError* error) {
    calls++;
    XCTAssertFalse(stopCompleted);
    XCTAssertEqual(error.code, CGEchoErrorNotRunning);
    [read fulfill];
  }];
  [self.controller stopWithCompletion:^{
    stopCompleted = YES;
    [stopped fulfill];
  }];
  [self waitForExpectations:@[ read, stopped ] timeout:3 enforceOrder:YES];
  [self settle:0.2];
  XCTAssertEqual(calls, 1u);
}

- (void)testSnapshotFromPreviousSessionIsRejected
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  XCTestExpectation* stopped = [self expectationWithDescription:@"stop"];
  [self.controller stopWithCompletion:^{ [stopped fulfill]; }];
  [self waitForExpectations:@[ stopped ] timeout:3];
  [self start];
  XCTAssertEqual([self readError:0x80 object:object].code, CGEchoErrorDeviceUnavailable);
  XCTAssertEqual(object.EOJ, AirconEOJ, @"old snapshots stay readable");
}

#pragma mark Discovery (U03)

- (void)testDiscoveryFindsNodeAndObjects
{
  [self start];
  NSArray<CGEchoRemoteNode*>* nodes = [self discover];
  XCTAssertEqual(nodes.count, 1u);
  CGEchoRemoteNode* node = nodes.firstObject;
  XCTAssertEqualObjects(node.address, @"192.0.2.1");
  XCTAssertTrue(node.hasObjectList);
  XCTAssertEqual(node.objects.count, 2u);
  XCTAssertEqual(node.objects[0].EOJ, AirconEOJ);
  XCTAssertEqual(node.objects[0].classGroupCode, 0x01);
  XCTAssertEqual(node.objects[0].classCode, 0x30);
  XCTAssertEqual(node.objects[0].instanceCode, 0x01);
  XCTAssertEqual(node.availability, CGEchoNodeAvailabilityActive);
  XCTAssertEqual(self.controller.nodes.count, 1u);
  XCTAssertEqual(node.objects[0].capabilities.writable.state, CGEchoCapabilityStateUnknown);

  // A second search with an identical list does not raise added/updated again.
  [self discover];
  [self settle:0.1];
  XCTAssertEqual(self.addedCount, 1u);
  XCTAssertEqual(self.updatedCount, 0u);
}

- (void)testDiscoveryWithNoDevicesIsEmptyNotError
{
  self.device.respondToSearch = NO;
  [self start];
  XCTAssertEqual([self discover].count, 0u);
}

- (void)testDiscoverySendFailureIsReported
{
  [self start];
  self.transport.failSend = YES;
  XCTestExpectation* done = [self expectationWithDescription:@"discover"];
  [self.controller discoverWithTimeout:0.3 completion:^(NSArray* nodes, NSError* error) {
    XCTAssertEqual(error.code, CGEchoErrorTransportFailure);
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];
}

- (void)testMalformedInstanceListIsIgnored
{
  self.transport.responder = ^NSArray<CGEchoFrame*>*(CGEchoFrame* request, NSString* address) {
    const uint8_t bad[] = { 2, 0x01, 0x30, 0x01 }; // declares 2, carries 1
    return @[ CGEchoTestReply(request, CGEchoESVGetResponse, CGEchoEPCSelfNodeInstanceListS, [NSData dataWithBytes:bad length:sizeof(bad)]) ];
  };
  [self start];
  XCTAssertEqual([self discover].count, 0u);
}

- (void)testInstanceListNotificationAddsAndUpdatesNode
{
  [self start];
  CGEchoFrame* inf = [CGEchoFrame frameWithTID:1 SEOJ:CGEchoNodeProfileEOJ DEOJ:CGEchoNodeProfileEOJ ESV:CGEchoESVInf properties:@[ [CGEchoFrameProperty propertyWithEPC:CGEchoEPCInstanceListNotification data:CGEchoTestInstanceList(@[ @(LightEOJ) ])] ]];
  [self.transport injectFrame:inf fromAddress:DeviceAddress];
  [self settle:0.2];
  XCTAssertEqual(self.controller.nodes.count, 1u);
  XCTAssertEqual(self.controller.nodes.firstObject.objects.count, 1u);

  CGEchoFrame* inf2 = [CGEchoFrame frameWithTID:2 SEOJ:CGEchoNodeProfileEOJ DEOJ:CGEchoNodeProfileEOJ ESV:CGEchoESVInf properties:@[ [CGEchoFrameProperty propertyWithEPC:CGEchoEPCInstanceListNotification data:CGEchoTestInstanceList(@[ @(LightEOJ), @(AirconEOJ) ])] ]];
  [self.transport injectFrame:inf2 fromAddress:DeviceAddress];
  [self settle:0.2];
  XCTAssertEqual(self.controller.nodes.firstObject.objects.count, 2u);
  XCTAssertEqual(self.addedCount, 1u);
  XCTAssertEqual(self.updatedCount, 1u);
}

#pragma mark Get / SetC (U05, U06)

- (void)testReadSuccessStoresLastValue
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  XCTestExpectation* done = [self expectationWithDescription:@"read"];
  [self.controller readProperty:0x80 ofObject:object timeout:0 completion:^(CGEchoPropertyValue* value, NSError* error) {
    XCTAssertNil(error);
    XCTAssertEqualObjects(value.data, Byte1(0x30));
    XCTAssertEqual(value.source, CGEchoPropertyValueSourceGet);
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];

  // A later failure does not erase the last successful value (U07).
  self.device.silent = YES;
  XCTAssertEqual([self readError:0x80 object:object].code, CGEchoErrorTimeout);
  XCTAssertEqualObjects([self.controller lastValueOfProperty:0x80 ofObject:object].data, Byte1(0x30));
}

- (void)testReadSNA
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  NSError* error = [self readError:0xB0 object:object];
  XCTAssertEqual(error.code, CGEchoErrorUnsupportedProperty);
  XCTAssertEqualObjects(error.userInfo[CGEchoErrorESVKey], @(CGEchoESVGetSNA));
  XCTAssertNotNil(error.userInfo[CGEchoErrorResponseDataKey]);
}

- (void)testMismatchedResponsesDoNotComplete
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.transport.responder = ^NSArray<CGEchoFrame*>*(CGEchoFrame* request, NSString* address) {
    CGEchoFrame* ok = CGEchoTestReply(request, CGEchoESVGetResponse, 0x80, Byte1(0x30));
    return @[
      [CGEchoFrame frameWithTID:(CGEchoTID)(request.TID + 1) SEOJ:request.DEOJ DEOJ:request.SEOJ ESV:CGEchoESVGetResponse properties:ok.properties], // TID
      CGEchoTestReply(request, CGEchoESVGetResponse, 0x81, Byte1(0x30)), // EPC
      [CGEchoFrame frameWithTID:request.TID SEOJ:LightEOJ DEOJ:request.SEOJ ESV:CGEchoESVGetResponse properties:ok.properties], // SEOJ
      [CGEchoFrame frameWithTID:request.TID SEOJ:request.DEOJ DEOJ:0x05FF02 ESV:CGEchoESVGetResponse properties:ok.properties], // DEOJ
      CGEchoTestReply(request, CGEchoESVSetResponse, 0x80, nil), // ESV
    ];
  };
  XCTAssertEqual([self readError:0x80 object:object].code, CGEchoErrorTimeout);

  // Correct frame from a different address.
  self.transport.responder = ^NSArray<CGEchoFrame*>*(CGEchoFrame* request, NSString* address) {
    CGEchoFrame* reply = CGEchoTestReply(request, CGEchoESVGetResponse, 0x80, Byte1(0x30));
    return @[ [CGEchoFrame frameWithData:reply.rawData address:@"192.0.2.99"] ];
  };
  XCTAssertEqual([self readError:0x80 object:object].code, CGEchoErrorTimeout);
}

- (void)testImmediateDuplicateResponseCompletesOnce
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.transport.responder = ^NSArray<CGEchoFrame*>*(CGEchoFrame* request, NSString* address) {
    CGEchoFrame* reply = CGEchoTestReply(request, CGEchoESVGetResponse, 0x80, Byte1(0x31));
    return @[ reply, reply ];
  };
  __block NSUInteger calls = 0;
  XCTestExpectation* done = [self expectationWithDescription:@"read"];
  [self.controller readProperty:0x80 ofObject:object timeout:0 completion:^(CGEchoPropertyValue* value, NSError* error) {
    calls++;
    XCTAssertNil(error);
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:3];
  [self settle:0.4];
  XCTAssertEqual(calls, 1u);
}

- (void)testCancelCompletesOnceAndIgnoresLateResponse
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.device.silent = YES;
  __block NSUInteger calls = 0;
  __block NSError* result = nil;
  XCTestExpectation* done = [self expectationWithDescription:@"read"];
  id<CGEchoRequest> request = [self.controller readProperty:0x80 ofObject:object timeout:5 completion:^(CGEchoPropertyValue* value, NSError* error) {
    calls++;
    result = error;
    [done fulfill];
  }];
  [self settle:0.1];
  CGEchoFrame* sentRequest = [self sentFramesWithESV:CGEchoESVGet].lastObject;
  [request cancel];
  [request cancel];
  [self waitForExpectations:@[ done ] timeout:3];
  [self.transport injectFrame:CGEchoTestReply(sentRequest, CGEchoESVGetResponse, 0x80, Byte1(0x30)) fromAddress:@"192.0.2.1"];
  [self settle:0.2];
  XCTAssertEqual(calls, 1u);
  XCTAssertEqual(result.code, CGEchoErrorCancelled);
  XCTAssertNil([self.controller lastValueOfProperty:0x80 ofObject:object]);
}

- (void)testBusyWhenTooManyPending
{
  CGEchoControllerConfiguration* config = [self configuration];
  config.maxConcurrentRequests = 1;
  self.controller = [[CGEchoController alloc] initWithConfiguration:config transport:self.transport];
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.device.silent = YES;
  XCTestExpectation* first = [self expectationWithDescription:@"first"];
  [self.controller readProperty:0x80 ofObject:object timeout:1 completion:^(CGEchoPropertyValue* value, NSError* error) {
    XCTAssertEqual(error.code, CGEchoErrorTimeout);
    [first fulfill];
  }];
  XCTAssertEqual([self readError:0x80 object:object].code, CGEchoErrorBusy);
  [self waitForExpectations:@[ first ] timeout:3];
}

- (void)testInvalidArguments
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  XCTAssertEqual([self readError:0x7F object:object].code, CGEchoErrorInvalidArgument);
  XCTAssertEqual([self write:0x80 data:[NSData data] object:object options:CGEchoWriteOptionsAllowUnknownCapability].code, CGEchoErrorInvalidArgument);
  XCTAssertNil([[CGEchoController alloc] initWithConfiguration:({
    CGEchoControllerConfiguration* c = [self configuration];
    c.maxConcurrentRequests = 0;
    c;
  })]);
}

- (void)testWriteRequiresKnownCapabilities
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  XCTAssertEqual([self write:0x80 data:Byte1(0x31) object:object options:CGEchoWriteOptionsNone].code, CGEchoErrorCapabilityUnknown);
  XCTAssertEqual([self sentFramesWithESV:CGEchoESVSetC].count, 0u, @"refused locally, nothing sent");
  XCTAssertNil([self write:0x80 data:Byte1(0x31) object:object options:CGEchoWriteOptionsAllowUnknownCapability]);
  XCTAssertEqual([self sentFramesWithESV:CGEchoESVSetC].count, 1u);
}

- (void)testFetchCapabilitiesThenWrite
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  __block CGEchoRemoteObject* updated = nil;
  XCTestExpectation* fetched = [self expectationWithDescription:@"caps"];
  [self.controller fetchCapabilitiesOfObject:object completion:^(CGEchoRemoteObject* result, NSError* error) {
    XCTAssertNil(error);
    updated = result;
    [fetched fulfill];
  }];
  [self waitForExpectations:@[ fetched ] timeout:3];
  XCTAssertEqual(updated.capabilities.readable.state, CGEchoCapabilityStateAvailable);
  XCTAssertTrue([updated.capabilities.readable containsProperty:0x9E]);
  XCTAssertEqual(updated.capabilities.writable.state, CGEchoCapabilityStateAvailable);
  XCTAssertEqual(updated.capabilities.notifiable.state, CGEchoCapabilityStateFailed, @"0x9D answered SNA");
  XCTAssertEqual(object.capabilities.writable.state, CGEchoCapabilityStateUnknown, @"old snapshot is immutable");

  XCTAssertNil([self write:0x80 data:Byte1(0x31) object:updated options:CGEchoWriteOptionsNone]);
  XCTAssertEqualObjects(self.device.values[@0x80], Byte1(0x31));
  NSError* error = [self write:0xB0 data:Byte1(0x41) object:updated options:CGEchoWriteOptionsNone];
  XCTAssertEqual(error.code, CGEchoErrorUnsupportedProperty, @"not in the Set map, refused locally");
  XCTAssertEqual([self sentFramesWithESV:CGEchoESVSetC].count, 1u);
}

- (void)testWriteSNA
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  [self.device.snaEPCs addObject:@0x80];
  XCTAssertEqual([self write:0x80 data:Byte1(0x31) object:object options:CGEchoWriteOptionsAllowUnknownCapability].code, CGEchoErrorUnsupportedProperty);
}

- (void)testTimedOutWriteIsNotResent
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  self.device.silent = YES;
  XCTAssertEqual([self write:0x80 data:Byte1(0x31) object:object options:CGEchoWriteOptionsAllowUnknownCapability].code, CGEchoErrorTimeout);
  [self settle:0.5];
  XCTAssertEqual([self sentFramesWithESV:CGEchoESVSetC].count, 1u);
}

- (void)testTIDsAreNotReusedWhilePendingOrRetained
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  for (int n = 0; n < 4; n++)
    XCTAssertNil([self readError:0x80 object:object]);
  NSArray* frames = [self sentFramesWithESV:CGEchoESVGet];
  NSMutableSet* tids = [NSMutableSet set];
  for (CGEchoFrame* f in frames)
    [tids addObject:@(f.TID)];
  XCTAssertEqual(tids.count, frames.count);
}

#pragma mark INF (U07)

- (void)testNotificationDeliveredOnceAndObservationCanBeInvalidated
{
  CGEchoRemoteObject* object = [self startAndDiscoverObject:AirconEOJ];
  __block NSUInteger objectCalls = 0;
  __block NSUInteger allCalls = 0;
  id<CGEchoObservation> objectObservation = [self.controller observeNotificationsOfObject:object handler:^(CGEchoPropertyValue* value) {
    objectCalls++;
    XCTAssertEqual(value.source, CGEchoPropertyValueSourceNotification);
  }];
  [self.controller observeNotificationsOfObject:nil handler:^(CGEchoPropertyValue* value) {
    allCalls++;
  }];
  [self settle:0.1];

  CGEchoFrame* inf = [CGEchoFrame frameWithTID:7 SEOJ:AirconEOJ DEOJ:CGEchoControllerEOJ ESV:CGEchoESVInf properties:@[ [CGEchoFrameProperty propertyWithEPC:0x80 data:Byte1(0x31)] ]];
  [self.transport injectFrame:inf fromAddress:@"192.0.2.1"];
  [self settle:0.2];
  XCTAssertEqual(objectCalls, 1u);
  XCTAssertEqual(allCalls, 1u);
  XCTAssertEqualObjects([self.controller lastValueOfProperty:0x80 ofObject:object].data, Byte1(0x31));

  [objectObservation invalidate];
  [self.transport injectFrame:inf fromAddress:@"192.0.2.1"];
  [self settle:0.2];
  XCTAssertEqual(objectCalls, 1u);
  XCTAssertEqual(allCalls, 2u);

  // Notifications from unknown nodes are ignored.
  [self.transport injectFrame:inf fromAddress:DeviceAddress];
  [self settle:0.2];
  XCTAssertEqual(allCalls, 2u);
}

@end
