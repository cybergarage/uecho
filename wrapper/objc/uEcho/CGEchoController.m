/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoController.h"
#import "CGEchoCapabilities.h"
#import "CGEchoError.h"
#import "CGEchoRemoteNode.h"
#import "Private/CGEchoFrame.h"
#import "Private/CGEchoInternal.h"
#import "Private/CGEchoTransport.h"

#include <stdatomic.h>
#include <time.h>

static void* const CGEchoStateQueueKey = (void*)&CGEchoStateQueueKey;

static uint64_t CGEchoMonotonicNow(void)
{
  return clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW);
}

static BOOL CGEchoIsNodeProfile(CGEchoEOJ eoj)
{
  return (eoj & 0xFFFF00) == 0x0EF000;
}

#pragma mark - Registry records (state queue only)

@interface CGEchoObjectRecord : NSObject
@property (nonatomic) CGEchoEOJ eoj;
@property (nonatomic, strong) CGEchoCapabilities* capabilities;
@property (nonatomic, strong) NSMutableDictionary<NSNumber*, CGEchoPropertyValue*>* lastValues;
@end

@implementation CGEchoObjectRecord
- (instancetype)initWithEOJ:(CGEchoEOJ)eoj
{
  if ((self = [super init]) == nil)
    return nil;
  _eoj = eoj;
  _capabilities = [CGEchoCapabilities unknownCapabilities];
  _lastValues = [NSMutableDictionary dictionary];
  return self;
}
@end

@interface CGEchoNodeRecord : NSObject
@property (nonatomic, copy) NSString* address;
@property (nonatomic, strong) NSMutableDictionary<NSNumber*, CGEchoObjectRecord*>* objects;
@property (nonatomic, copy) NSArray<NSNumber*>* objectOrder;
@property (nonatomic) BOOL hasObjectList;
@property (nonatomic, strong) NSDate* lastSeen;
@end

@implementation CGEchoNodeRecord
- (instancetype)initWithAddress:(NSString*)address
{
  if ((self = [super init]) == nil)
    return nil;
  _address = [address copy];
  _objects = [NSMutableDictionary dictionary];
  _objectOrder = @[];
  _lastSeen = [NSDate date];
  return self;
}
@end

#pragma mark - Requests

typedef void (^CGEchoPendingResultHandler)(CGEchoFrame* _Nullable response, NSError* _Nullable error);

@interface CGEchoPendingRequest : NSObject <CGEchoRequest>
@property (nonatomic, weak) CGEchoController* controller;
@property (nonatomic, copy) NSString* operation;
@property (nonatomic, copy) NSString* address;
@property (nonatomic) CGEchoEOJ deoj;
@property (nonatomic) CGEchoEPC epc;
@property (nonatomic, copy) NSData* edt;
@property (nonatomic) CGEchoESV requestESV;
@property (nonatomic) CGEchoESV successESV;
@property (nonatomic) CGEchoESV snaESV;
@property (nonatomic) BOOL hasTID;
@property (nonatomic) CGEchoTID tid;
@property (nonatomic) BOOL finished;
@property (nonatomic, strong, nullable) dispatch_source_t timer;
@property (nonatomic, copy, nullable) CGEchoPendingResultHandler resultHandler;
@end

@interface CGEchoDiscoverySession : NSObject <CGEchoRequest>
@property (nonatomic, weak) CGEchoController* controller;
@property (nonatomic) BOOL hasTID;
@property (nonatomic) CGEchoTID tid;
@property (nonatomic) BOOL finished;
@property (nonatomic, strong) NSMutableOrderedSet<NSString*>* seen;
@property (nonatomic, strong, nullable) dispatch_source_t timer;
@property (nonatomic, copy, nullable) void (^completion)(NSArray<CGEchoRemoteNode*>*, NSError* _Nullable);
@end

@interface CGEchoCompositeRequest : NSObject <CGEchoRequest>
@property (nonatomic, weak) CGEchoController* controller;
@property (nonatomic, strong) NSArray<CGEchoPendingRequest*>* children;
@property (nonatomic) BOOL cancelled;
@property (nonatomic) BOOL finished;
@end

@interface CGEchoObservationToken : NSObject <CGEchoObservation> {
@public
  atomic_bool valid;
}
@property (nonatomic, weak) CGEchoController* controller;
@property (nonatomic) uint64_t identifier;
@property (nonatomic, copy, nullable) NSString* address;
@property (nonatomic) CGEchoEOJ eoj;
@property (nonatomic, copy) void (^handler)(CGEchoPropertyValue*);
@end

@interface CGEchoController ()
- (void)cancelPending:(CGEchoPendingRequest*)request;
- (void)cancelDiscovery:(CGEchoDiscoverySession*)session;
- (void)cancelComposite:(CGEchoCompositeRequest*)request;
- (void)removeObservation:(CGEchoObservationToken*)token;
@end

@implementation CGEchoPendingRequest
- (void)cancel
{
  [self.controller cancelPending:self];
}
@end

@implementation CGEchoDiscoverySession
- (void)cancel
{
  [self.controller cancelDiscovery:self];
}
@end

@implementation CGEchoCompositeRequest
- (void)cancel
{
  [self.controller cancelComposite:self];
}
@end

@implementation CGEchoObservationToken
- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  atomic_init(&valid, true);
  return self;
}
- (void)invalidate
{
  if (atomic_exchange(&valid, false))
    [self.controller removeObservation:self];
}
@end

#pragma mark - Controller

@implementation CGEchoController {
  id<CGEchoTransport> transport;
  dispatch_queue_t stateQueue;
  dispatch_queue_t callbackQueue;
  _Atomic uint64_t generationValue;
  _Atomic NSInteger stateValue;

  NSMutableDictionary<NSString*, CGEchoNodeRecord*>* nodeRecords;
  NSMutableArray<NSString*>* nodeOrder;
  NSMutableDictionary<NSNumber*, CGEchoPendingRequest*>* pending;
  NSMutableArray<CGEchoDiscoverySession*>* discoveries;
  NSMutableDictionary<NSNumber*, NSNumber*>* retainedTIDs; // TID -> expiry (monotonic ns)
  CGEchoTID nextTID;
  NSMutableDictionary<NSNumber*, CGEchoObservationToken*>* observations;
  uint64_t nextObservationID;
}

- (instancetype)init
{
  return [self initWithConfiguration:[CGEchoControllerConfiguration defaultConfiguration]];
}

- (instancetype)initWithConfiguration:(CGEchoControllerConfiguration*)configuration
{
  return [self initWithConfiguration:configuration transport:[[CGEchoCTransport alloc] init]];
}

- (instancetype)initWithConfiguration:(CGEchoControllerConfiguration*)configuration transport:(id<CGEchoTransport>)aTransport
{
  if (![configuration validate:NULL] || !aTransport)
    return nil;
  if ((self = [super init]) == nil)
    return nil;
  _configuration = [configuration copy];
  transport = aTransport;
  stateQueue = dispatch_queue_create("org.cybergarage.uecho.controller.state", DISPATCH_QUEUE_SERIAL);
  dispatch_queue_set_specific(stateQueue, CGEchoStateQueueKey, CGEchoStateQueueKey, NULL);
  callbackQueue = _configuration.callbackQueue;
  atomic_init(&generationValue, 1);
  atomic_init(&stateValue, CGEchoControllerStateStopped);
  nodeRecords = [NSMutableDictionary dictionary];
  nodeOrder = [NSMutableArray array];
  pending = [NSMutableDictionary dictionary];
  discoveries = [NSMutableArray array];
  retainedTIDs = [NSMutableDictionary dictionary];
  nextTID = (CGEchoTID)arc4random_uniform(0x10000);
  observations = [NSMutableDictionary dictionary];
  nextObservationID = 1;
  return self;
}

- (void)dealloc
{
  if (!stateQueue)
    return; // initialization failed before the queue was created
  // dealloc may run on the state queue when the last strong reference is
  // released by a block executing there.
  __unsafe_unretained CGEchoController* unsafeSelf = self;
  dispatch_block_t teardown = ^{
    [unsafeSelf->transport stop];
    [unsafeSelf failAllWithError:CGEchoMakeError(CGEchoErrorNotRunning, @"The controller was deallocated", nil)];
  };
  if (dispatch_get_specific(CGEchoStateQueueKey) == CGEchoStateQueueKey)
    teardown();
  else
    dispatch_sync(stateQueue, teardown);
}

- (uint64_t)generation
{
  return atomic_load(&generationValue);
}

- (CGEchoControllerState)state
{
  return (CGEchoControllerState)atomic_load(&stateValue);
}

- (void)setStateValue:(CGEchoControllerState)state
{
  atomic_store(&stateValue, state);
}

#pragma mark Delivery

/// Delivers an event unless the session it belongs to has ended.
- (void)deliverEvent:(dispatch_block_t)block
{
  uint64_t gen = self.generation;
  __weak CGEchoController* weakSelf = self;
  dispatch_async(callbackQueue, ^{
    CGEchoController* strongSelf = weakSelf;
    if (!strongSelf || strongSelf.generation != gen)
      return;
    block();
  });
}

/// Delivers a request completion. Completions are always delivered exactly once.
- (void)deliverCompletion:(dispatch_block_t)block
{
  dispatch_async(callbackQueue, block);
}

- (NSDictionary*)infoForRequest:(CGEchoPendingRequest*)request response:(CGEchoFrame*)response
{
  NSMutableDictionary* info = [NSMutableDictionary dictionary];
  if (request.operation)
    info[CGEchoErrorOperationKey] = request.operation;
  if (request.address)
    info[CGEchoErrorAddressKey] = request.address;
  info[CGEchoErrorEOJKey] = @(request.deoj);
  info[CGEchoErrorEPCKey] = @(request.epc);
  if (request.hasTID)
    info[CGEchoErrorTIDKey] = @(request.tid);
  if (response) {
    info[CGEchoErrorESVKey] = @(response.ESV);
    info[CGEchoErrorResponseDataKey] = response.rawData;
  }
  return info;
}

#pragma mark Lifecycle

- (void)startWithCompletion:(void (^)(NSError*))completion
{
  void (^done)(NSError*) = [completion copy];
  dispatch_async(stateQueue, ^{
    if (self.state == CGEchoControllerStateRunning) {
      if (done)
        [self deliverCompletion:^{ done(nil); }];
      return;
    }
    [self setStateValue:CGEchoControllerStateStarting];
    [self->nodeRecords removeAllObjects];
    [self->nodeOrder removeAllObjects];
    [self->retainedTIDs removeAllObjects];

    uint64_t gen = self.generation;
    __weak CGEchoController* weakSelf = self;
    CGEchoTransportFrameHandler handler = ^(CGEchoFrame* frame) {
      CGEchoController* strongSelf = weakSelf;
      if (!strongSelf)
        return;
      dispatch_async(strongSelf->stateQueue, ^{
        if (strongSelf.generation != gen || strongSelf.state != CGEchoControllerStateRunning)
          return;
        [strongSelf handleFrame:frame];
      });
    };

    NSError* error = nil;
    if (![self->transport startWithFrameHandler:handler error:&error]) {
      [self setStateValue:CGEchoControllerStateStopped];
      NSError* failure = error ?: CGEchoMakeError(CGEchoErrorTransportFailure, @"Could not start the transport", @{ CGEchoErrorOperationKey : @"start" });
      if (done)
        [self deliverCompletion:^{ done(failure); }];
      return;
    }
    [self setStateValue:CGEchoControllerStateRunning];
    if (done)
      [self deliverCompletion:^{ done(nil); }];
  });
}

- (void)stopWithCompletion:(void (^)(void))completion
{
  dispatch_block_t done = [completion copy];
  dispatch_async(stateQueue, ^{
    if (self.state != CGEchoControllerStateStopped) {
      [self setStateValue:CGEchoControllerStateStopping];
      atomic_fetch_add(&self->generationValue, 1);
      [self->transport stop];
      [self failAllWithError:CGEchoMakeError(CGEchoErrorNotRunning, @"The controller stopped", nil)];
      [self setStateValue:CGEchoControllerStateStopped];
    }
    if (done)
      [self deliverCompletion:done];
  });
}

- (void)failAllWithError:(NSError*)error
{
  for (CGEchoPendingRequest* request in self->pending.allValues)
    [self finishPending:request response:nil error:error];
  for (CGEchoDiscoverySession* session in [self->discoveries copy])
    [self finishDiscovery:session error:error];
  [self->retainedTIDs removeAllObjects];
}

#pragma mark Registry

- (CGEchoRemoteObject*)snapshotObject:(CGEchoObjectRecord*)object ofNode:(CGEchoNodeRecord*)node
{
  return [[CGEchoRemoteObject alloc] initWithAddress:node.address EOJ:object.eoj capabilities:object.capabilities generation:self.generation];
}

- (CGEchoRemoteNode*)snapshotNode:(CGEchoNodeRecord*)node
{
  NSMutableArray* objects = [NSMutableArray array];
  for (NSNumber* eoj in node.objectOrder) {
    CGEchoObjectRecord* object = node.objects[eoj];
    if (object)
      [objects addObject:[self snapshotObject:object ofNode:node]];
  }
  BOOL stale = self.configuration.staleInterval < -[node.lastSeen timeIntervalSinceNow];
  return [[CGEchoRemoteNode alloc] initWithAddress:node.address objects:objects hasObjectList:node.hasObjectList lastSeen:node.lastSeen availability:(stale ? CGEchoNodeAvailabilityStale : CGEchoNodeAvailabilityActive) generation:self.generation];
}

- (NSArray<CGEchoRemoteNode*>*)nodes
{
  __block NSArray* result = nil;
  dispatch_sync(stateQueue, ^{
    NSMutableArray* snapshots = [NSMutableArray array];
    for (NSString* address in self->nodeOrder) {
      CGEchoNodeRecord* node = self->nodeRecords[address];
      if (node)
        [snapshots addObject:[self snapshotNode:node]];
    }
    result = snapshots;
  });
  return result;
}

- (void)forgetNode:(CGEchoRemoteNode*)node
{
  NSString* address = [node.address copy];
  uint64_t gen = node.generation;
  dispatch_async(stateQueue, ^{
    if (gen != self.generation)
      return;
    [self->nodeRecords removeObjectForKey:address];
    [self->nodeOrder removeObject:address];
  });
}

- (CGEchoPropertyValue*)lastValueOfProperty:(CGEchoEPC)epc ofObject:(CGEchoRemoteObject*)object
{
  __block CGEchoPropertyValue* value = nil;
  dispatch_sync(stateQueue, ^{
    if (object.generation != self.generation)
      return;
    value = self->nodeRecords[object.address].objects[@(object.EOJ)].lastValues[@(epc)];
  });
  return value;
}

- (CGEchoNodeRecord*)touchNodeWithAddress:(NSString*)address isNew:(BOOL*)isNew
{
  CGEchoNodeRecord* node = nodeRecords[address];
  *isNew = (node == nil);
  if (!node) {
    node = [[CGEchoNodeRecord alloc] initWithAddress:address];
    nodeRecords[address] = node;
    [nodeOrder addObject:address];
  }
  node.lastSeen = [NSDate date];
  for (CGEchoDiscoverySession* session in discoveries)
    [session.seen addObject:address];
  return node;
}

- (void)notifyNode:(CGEchoNodeRecord*)node added:(BOOL)added
{
  CGEchoRemoteNode* snapshot = [self snapshotNode:node];
  __weak CGEchoController* weakSelf = self;
  [self deliverEvent:^{
    CGEchoController* strongSelf = weakSelf;
    id<CGEchoControllerDelegate> delegate = strongSelf.delegate;
    if (added && [delegate respondsToSelector:@selector(echoController:didAddNode:)])
      [delegate echoController:strongSelf didAddNode:snapshot];
    if (!added && [delegate respondsToSelector:@selector(echoController:didUpdateNode:)])
      [delegate echoController:strongSelf didUpdateNode:snapshot];
  }];
}

/// Applies a complete instance list (D5 / D6 EDT). Malformed lists are ignored
/// and never remove objects.
- (void)applyInstanceList:(NSData*)edt fromAddress:(NSString*)address
{
  const uint8_t* b = edt.bytes;
  NSUInteger length = edt.length;
  if (length < 1 || length != 1 + 3 * (NSUInteger)b[0])
    return;

  NSMutableOrderedSet<NSNumber*>* eojs = [NSMutableOrderedSet orderedSet];
  for (NSUInteger n = 0; n < b[0]; n++) {
    const uint8_t* p = b + 1 + 3 * n;
    CGEchoEOJ eoj = ((CGEchoEOJ)p[0] << 16) | ((CGEchoEOJ)p[1] << 8) | p[2];
    if (!CGEchoIsNodeProfile(eoj))
      [eojs addObject:@(eoj)];
  }

  BOOL isNew = NO;
  CGEchoNodeRecord* node = [self touchNodeWithAddress:address isNew:&isNew];
  NSArray* order = eojs.array;
  BOOL changed = isNew || !node.hasObjectList || ![node.objectOrder isEqualToArray:order];

  NSMutableDictionary* objects = [NSMutableDictionary dictionary];
  for (NSNumber* eoj in order)
    objects[eoj] = node.objects[eoj] ?: [[CGEchoObjectRecord alloc] initWithEOJ:eoj.unsignedIntValue];
  node.objects = objects;
  node.objectOrder = order;
  node.hasObjectList = YES;

  if (changed)
    [self notifyNode:node added:isNew];
}

#pragma mark Frames

- (void)handleFrame:(CGEchoFrame*)frame
{
  NSString* address = frame.address;
  if (!address)
    return;

  nodeRecords[address].lastSeen = [NSDate date];

  if (frame.isResponse) {
    CGEchoPendingRequest* request = pending[@(frame.TID)];
    if (request && [self request:request matchesResponse:frame]) {
      [self finishPending:request response:frame error:nil];
      return;
    }
  }

  if (CGEchoIsNodeProfile(frame.SEOJ)) {
    BOOL isDiscoveryReply = NO;
    for (CGEchoDiscoverySession* session in discoveries) {
      if (session.hasTID && session.tid == frame.TID)
        isDiscoveryReply = YES;
    }
    BOOL applied = NO;
    if (frame.ESV == CGEchoESVGetResponse || frame.ESV == CGEchoESVInf) {
      for (CGEchoFrameProperty* prop in frame.properties) {
        BOOL isList = (prop.EPC == CGEchoEPCSelfNodeInstanceListS && frame.ESV == CGEchoESVGetResponse) || prop.EPC == CGEchoEPCInstanceListNotification;
        if (isList) {
          [self applyInstanceList:prop.data fromAddress:address];
          applied = YES;
        }
      }
    }
    if (!applied && isDiscoveryReply) {
      // A node that answered the search but without a usable list (e.g. SNA).
      BOOL isNew = NO;
      CGEchoNodeRecord* node = [self touchNodeWithAddress:address isNew:&isNew];
      if (isNew)
        [self notifyNode:node added:YES];
    }
    return;
  }

  if (frame.ESV == CGEchoESVInf)
    [self handleNotification:frame];
}

- (void)handleNotification:(CGEchoFrame*)frame
{
  // Policy: notifications from nodes or objects that are not in the registry are ignored.
  CGEchoObjectRecord* object = nodeRecords[frame.address].objects[@(frame.SEOJ)];
  if (!object)
    return;

  NSDate* now = [NSDate date];
  NSMutableArray<CGEchoObservationToken*>* targets = [NSMutableArray array];
  for (CGEchoObservationToken* token in observations.allValues) {
    if (!token.address || ([token.address isEqualToString:frame.address] && token.eoj == frame.SEOJ))
      [targets addObject:token];
  }

  for (CGEchoFrameProperty* prop in frame.properties) {
    CGEchoPropertyValue* value = [[CGEchoPropertyValue alloc] initWithAddress:frame.address EOJ:frame.SEOJ EPC:prop.EPC data:prop.data receivedAt:now source:CGEchoPropertyValueSourceNotification];
    object.lastValues[@(prop.EPC)] = value;
    __weak CGEchoController* weakSelf = self;
    NSArray* observers = [targets copy];
    [self deliverEvent:^{
      CGEchoController* strongSelf = weakSelf;
      id<CGEchoControllerDelegate> delegate = strongSelf.delegate;
      if ([delegate respondsToSelector:@selector(echoController:didReceiveNotification:)])
        [delegate echoController:strongSelf didReceiveNotification:value];
      for (CGEchoObservationToken* token in observers) {
        if (atomic_load(&token->valid))
          token.handler(value);
      }
    }];
  }
}

- (BOOL)request:(CGEchoPendingRequest*)request matchesResponse:(CGEchoFrame*)frame
{
  if (![frame.address isEqualToString:request.address])
    return NO;
  if (frame.SEOJ != request.deoj || frame.DEOJ != CGEchoControllerEOJ)
    return NO;
  if (frame.ESV != request.successESV && frame.ESV != request.snaESV)
    return NO;
  return frame.properties.count == 1 && frame.properties[0].EPC == request.epc;
}

#pragma mark TIDs

- (BOOL)isTIDInUse:(CGEchoTID)tid now:(uint64_t)now
{
  if (pending[@(tid)])
    return YES;
  for (CGEchoDiscoverySession* session in discoveries) {
    if (session.hasTID && session.tid == tid)
      return YES;
  }
  NSNumber* expiry = retainedTIDs[@(tid)];
  if (expiry) {
    if (now < expiry.unsignedLongLongValue)
      return YES;
    [retainedTIDs removeObjectForKey:@(tid)];
  }
  return NO;
}

- (BOOL)allocateTID:(CGEchoTID*)tid
{
  uint64_t now = CGEchoMonotonicNow();
  for (NSUInteger n = 0; n < 0x10000; n++) {
    CGEchoTID candidate = nextTID++;
    if (![self isTIDInUse:candidate now:now]) {
      *tid = candidate;
      return YES;
    }
  }
  return NO;
}

- (void)retainTID:(CGEchoTID)tid
{
  uint64_t expiry = CGEchoMonotonicNow() + (uint64_t)(self.configuration.lateResponseRetention * NSEC_PER_SEC);
  retainedTIDs[@(tid)] = @(expiry);
}

- (dispatch_source_t)timerWithTimeout:(NSTimeInterval)timeout handler:(dispatch_block_t)handler
{
  dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, stateQueue);
  dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, (int64_t)(timeout * NSEC_PER_SEC)), DISPATCH_TIME_FOREVER, 10 * NSEC_PER_MSEC);
  dispatch_source_set_event_handler(timer, handler);
  dispatch_resume(timer);
  return timer;
}

#pragma mark Pending requests (state queue)

- (void)finishPending:(CGEchoPendingRequest*)request response:(CGEchoFrame*)response error:(NSError*)error
{
  if (request.finished)
    return;
  request.finished = YES;
  if (request.timer) {
    dispatch_source_cancel(request.timer);
    request.timer = nil;
  }
  if (request.hasTID) {
    if (pending[@(request.tid)] == request)
      [pending removeObjectForKey:@(request.tid)];
    [self retainTID:request.tid];
  }
  CGEchoPendingResultHandler handler = request.resultHandler;
  request.resultHandler = nil;
  if (handler)
    handler(response, error);
}

- (void)submitPending:(CGEchoPendingRequest*)request timeout:(NSTimeInterval)timeout
{
  if (request.finished)
    return;
  if (self.configuration.maxConcurrentRequests <= pending.count) {
    [self finishPending:request response:nil error:CGEchoMakeError(CGEchoErrorBusy, @"Too many pending requests", [self infoForRequest:request response:nil])];
    return;
  }
  CGEchoTID tid = 0;
  if (![self allocateTID:&tid]) {
    [self finishPending:request response:nil error:CGEchoMakeError(CGEchoErrorBusy, @"No transaction ID is available", [self infoForRequest:request response:nil])];
    return;
  }
  request.tid = tid;
  request.hasTID = YES;
  pending[@(tid)] = request;

  CGEchoFrame* frame = [CGEchoFrame frameWithTID:tid SEOJ:CGEchoControllerEOJ DEOJ:request.deoj ESV:request.requestESV properties:@[ [CGEchoFrameProperty propertyWithEPC:request.epc data:request.edt] ]];
  NSError* error = nil;
  if (![transport sendData:frame.rawData toAddress:request.address error:&error]) {
    [self finishPending:request response:nil error:CGEchoMakeError(CGEchoErrorTransportFailure, error.localizedDescription ?: @"Could not send the request", [self infoForRequest:request response:nil])];
    return;
  }

  __weak CGEchoController* weakSelf = self;
  __weak CGEchoPendingRequest* weakRequest = request;
  request.timer = [self timerWithTimeout:timeout handler:^{
    CGEchoController* strongSelf = weakSelf;
    CGEchoPendingRequest* strongRequest = weakRequest;
    if (!strongSelf || !strongRequest)
      return;
    [strongSelf finishPending:strongRequest response:nil error:CGEchoMakeError(CGEchoErrorTimeout, @"No response before the deadline; the device state is unknown", [strongSelf infoForRequest:strongRequest response:nil])];
  }];
}

- (void)cancelPending:(CGEchoPendingRequest*)request
{
  dispatch_async(stateQueue, ^{
    [self finishPending:request response:nil error:CGEchoMakeError(CGEchoErrorCancelled, @"The request was cancelled locally", [self infoForRequest:request response:nil])];
  });
}

/// Validates common inputs and resolves the object. Returns nil and sets error on failure.
- (CGEchoObjectRecord*)resolveObject:(CGEchoRemoteObject*)object request:(CGEchoPendingRequest*)request error:(NSError**)error
{
  if (self.state != CGEchoControllerStateRunning) {
    *error = CGEchoMakeError(CGEchoErrorNotRunning, @"The controller is not running", [self infoForRequest:request response:nil]);
    return nil;
  }
  if (object.generation != self.generation) {
    *error = CGEchoMakeError(CGEchoErrorDeviceUnavailable, @"The object belongs to an earlier session", [self infoForRequest:request response:nil]);
    return nil;
  }
  CGEchoObjectRecord* record = nodeRecords[object.address].objects[@(object.EOJ)];
  if (!record) {
    *error = CGEchoMakeError(CGEchoErrorDeviceUnavailable, @"The object is not in the registry", [self infoForRequest:request response:nil]);
    return nil;
  }
  return record;
}

- (BOOL)resolveTimeout:(NSTimeInterval*)timeout
{
  if (*timeout <= 0)
    *timeout = self.configuration.requestTimeout;
  return 0.1 <= *timeout && *timeout <= 60;
}

- (CGEchoPendingRequest*)newRequestForObject:(CGEchoRemoteObject*)object epc:(CGEchoEPC)epc operation:(NSString*)operation
{
  CGEchoPendingRequest* request = [[CGEchoPendingRequest alloc] init];
  request.controller = self;
  request.operation = operation;
  request.address = object.address;
  request.deoj = object.EOJ;
  request.epc = epc;
  request.edt = [NSData data];
  return request;
}

#pragma mark Get / SetC

- (id<CGEchoRequest>)readProperty:(CGEchoEPC)epc ofObject:(CGEchoRemoteObject*)object timeout:(NSTimeInterval)timeout completion:(void (^)(CGEchoPropertyValue*, NSError*))completion
{
  CGEchoPendingRequest* request = [self newRequestForObject:object epc:epc operation:@"read"];
  request.requestESV = CGEchoESVGet;
  request.successESV = CGEchoESVGetResponse;
  request.snaESV = CGEchoESVGetSNA;
  void (^done)(CGEchoPropertyValue*, NSError*) = [completion copy];
  dispatch_queue_t cbq = callbackQueue;

  __weak CGEchoController* weakSelf = self;
  dispatch_async(stateQueue, ^{
    NSTimeInterval to = timeout;
    NSError* error = nil;
    CGEchoObjectRecord* record = nil;
    if (epc < 0x80 || ![self resolveTimeout:&to])
      error = CGEchoMakeError(CGEchoErrorInvalidArgument, @"EPC must be 0x80-0xFF and timeout 0.1-60 seconds", [self infoForRequest:request response:nil]);
    else
      record = [self resolveObject:object request:request error:&error];

    __weak CGEchoPendingRequest* weakRequest = request;
    request.resultHandler = ^(CGEchoFrame* response, NSError* failure) {
      CGEchoController* strongSelf = weakSelf;
      CGEchoPendingRequest* strongRequest = weakRequest;
      if (!failure && response.ESV == CGEchoESVGetSNA)
        failure = CGEchoMakeError(CGEchoErrorUnsupportedProperty, @"The device answered Get_SNA", [strongSelf infoForRequest:strongRequest response:response]);
      if (failure) {
        if (done)
          dispatch_async(cbq, ^{ done(nil, failure); });
        return;
      }
      CGEchoPropertyValue* value = [[CGEchoPropertyValue alloc] initWithAddress:object.address EOJ:object.EOJ EPC:epc data:response.properties[0].data receivedAt:[NSDate date] source:CGEchoPropertyValueSourceGet];
      record.lastValues[@(epc)] = value;
      if (done)
        dispatch_async(cbq, ^{ done(value, nil); });
    };

    if (error) {
      [self finishPending:request response:nil error:error];
      return;
    }
    [self submitPending:request timeout:to];
  });
  return request;
}

- (id<CGEchoRequest>)writeProperty:(CGEchoEPC)epc data:(NSData*)data ofObject:(CGEchoRemoteObject*)object options:(CGEchoWriteOptions)options timeout:(NSTimeInterval)timeout completion:(void (^)(NSError*))completion
{
  CGEchoPendingRequest* request = [self newRequestForObject:object epc:epc operation:@"write"];
  request.edt = [data copy] ?: [NSData data];
  request.requestESV = CGEchoESVSetC;
  request.successESV = CGEchoESVSetResponse;
  request.snaESV = CGEchoESVSetCSNA;
  void (^done)(NSError*) = [completion copy];
  dispatch_queue_t cbq = callbackQueue;

  __weak CGEchoController* weakSelf = self;
  dispatch_async(stateQueue, ^{
    NSTimeInterval to = timeout;
    NSError* error = nil;
    CGEchoObjectRecord* record = nil;
    if (epc < 0x80 || request.edt.length < 1 || 0xFF < request.edt.length || ![self resolveTimeout:&to])
      error = CGEchoMakeError(CGEchoErrorInvalidArgument, @"EPC must be 0x80-0xFF, EDT 1-255 bytes and timeout 0.1-60 seconds", [self infoForRequest:request response:nil]);
    else
      record = [self resolveObject:object request:request error:&error];

    if (record) {
      CGEchoPropertyMap* map = record.capabilities.writable;
      if (map.state == CGEchoCapabilityStateAvailable) {
        if (![map containsProperty:epc])
          error = CGEchoMakeError(CGEchoErrorUnsupportedProperty, @"The Set property map does not list this EPC; the request was not sent", [self infoForRequest:request response:nil]);
      }
      else if (!(options & CGEchoWriteOptionsAllowUnknownCapability)) {
        error = CGEchoMakeError(CGEchoErrorCapabilityUnknown, @"The Set property map is not known; fetch capabilities or pass CGEchoWriteOptionsAllowUnknownCapability", [self infoForRequest:request response:nil]);
      }
    }

    __weak CGEchoPendingRequest* weakRequest = request;
    request.resultHandler = ^(CGEchoFrame* response, NSError* failure) {
      CGEchoController* strongSelf = weakSelf;
      CGEchoPendingRequest* strongRequest = weakRequest;
      if (!failure && response.ESV == CGEchoESVSetCSNA)
        failure = CGEchoMakeError(CGEchoErrorUnsupportedProperty, @"The device answered SetC_SNA", [strongSelf infoForRequest:strongRequest response:response]);
      if (done)
        dispatch_async(cbq, ^{ done(failure); });
    };

    if (error) {
      [self finishPending:request response:nil error:error];
      return;
    }
    [self submitPending:request timeout:to];
  });
  return request;
}

#pragma mark Capabilities

- (id<CGEchoRequest>)fetchCapabilitiesOfObject:(CGEchoRemoteObject*)object completion:(void (^)(CGEchoRemoteObject*, NSError*))completion
{
  CGEchoCompositeRequest* composite = [[CGEchoCompositeRequest alloc] init];
  composite.controller = self;
  void (^done)(CGEchoRemoteObject*, NSError*) = [completion copy];
  dispatch_queue_t cbq = callbackQueue;
  const CGEchoEPC mapEPCs[3] = { CGEchoEPCGetPropertyMap, CGEchoEPCSetPropertyMap, CGEchoEPCAnnouncePropertyMap };

  NSMutableArray* children = [NSMutableArray array];
  for (int n = 0; n < 3; n++) {
    CGEchoPendingRequest* child = [self newRequestForObject:object epc:mapEPCs[n] operation:@"fetchCapabilities"];
    child.requestESV = CGEchoESVGet;
    child.successESV = CGEchoESVGetResponse;
    child.snaESV = CGEchoESVGetSNA;
    [children addObject:child];
  }
  composite.children = children;

  dispatch_async(stateQueue, ^{
    NSError* error = nil;
    CGEchoObjectRecord* record = [self resolveObject:object request:children[0] error:&error];
    if (!error && self.configuration.maxConcurrentRequests < self->pending.count + 3)
      error = CGEchoMakeError(CGEchoErrorBusy, @"Too many pending requests", [self infoForRequest:children[0] response:nil]);
    if (error) {
      composite.finished = YES;
      for (CGEchoPendingRequest* child in children)
        child.finished = YES;
      if (done)
        [self deliverCompletion:^{ done(nil, error); }];
      return;
    }

    NSMutableDictionary<NSNumber*, CGEchoPropertyMap*>* maps = [NSMutableDictionary dictionary];
    __weak CGEchoController* weakSelf = self;
    void (^finishIfComplete)(void) = ^{
      CGEchoController* strongSelf = weakSelf;
      if (maps.count < 3 || composite.finished)
        return;
      composite.finished = YES;
      if (composite.cancelled || !strongSelf || strongSelf.state != CGEchoControllerStateRunning) {
        NSError* failure = composite.cancelled
            ? CGEchoMakeError(CGEchoErrorCancelled, @"The request was cancelled locally", nil)
            : CGEchoMakeError(CGEchoErrorNotRunning, @"The controller stopped", nil);
        if (done)
          dispatch_async(cbq, ^{ done(nil, failure); });
        return;
      }
      record.capabilities = [[CGEchoCapabilities alloc] initWithReadable:maps[@(CGEchoEPCGetPropertyMap)] writable:maps[@(CGEchoEPCSetPropertyMap)] notifiable:maps[@(CGEchoEPCAnnouncePropertyMap)]];
      CGEchoNodeRecord* node = strongSelf->nodeRecords[object.address];
      CGEchoRemoteObject* updated = (node && node.objects[@(object.EOJ)] == record) ? [strongSelf snapshotObject:record ofNode:node] : nil;
      NSError* failure = updated ? nil : CGEchoMakeError(CGEchoErrorDeviceUnavailable, @"The object was removed while fetching", nil);
      if (done)
        dispatch_async(cbq, ^{ done(updated, failure); });
    };

    for (CGEchoPendingRequest* child in children) {
      CGEchoEPC mapEPC = child.epc;
      child.resultHandler = ^(CGEchoFrame* response, NSError* failure) {
        CGEchoPropertyMap* map = nil;
        if (failure)
          map = [CGEchoPropertyMap failedMapWithEPC:mapEPC error:failure rawData:nil];
        else if (response.ESV == CGEchoESVGetSNA)
          map = [CGEchoPropertyMap failedMapWithEPC:mapEPC error:CGEchoMakeError(CGEchoErrorUnsupportedProperty, @"The device answered Get_SNA", nil) rawData:nil];
        else
          map = [CGEchoPropertyMap mapWithEPC:mapEPC rawData:response.properties[0].data];
        maps[@(mapEPC)] = map;
        finishIfComplete();
      };
      [self submitPending:child timeout:self.configuration.requestTimeout];
    }
  });
  return composite;
}

- (void)cancelComposite:(CGEchoCompositeRequest*)request
{
  dispatch_async(stateQueue, ^{
    if (request.finished)
      return;
    request.cancelled = YES;
    for (CGEchoPendingRequest* child in request.children)
      [self finishPending:child response:nil error:CGEchoMakeError(CGEchoErrorCancelled, @"The request was cancelled locally", nil)];
  });
}

#pragma mark Discovery

- (id<CGEchoRequest>)discoverWithTimeout:(NSTimeInterval)timeout completion:(void (^)(NSArray<CGEchoRemoteNode*>*, NSError*))completion
{
  CGEchoDiscoverySession* session = [[CGEchoDiscoverySession alloc] init];
  session.controller = self;
  session.seen = [NSMutableOrderedSet orderedSet];
  session.completion = completion;

  dispatch_async(stateQueue, ^{
    NSTimeInterval window = (timeout <= 0) ? self.configuration.discoveryTimeout : timeout;
    if (self.state != CGEchoControllerStateRunning) {
      [self finishDiscovery:session error:CGEchoMakeError(CGEchoErrorNotRunning, @"The controller is not running", @{ CGEchoErrorOperationKey : @"discover" })];
      return;
    }
    if (window < 0.1 || 60 < window) {
      [self finishDiscovery:session error:CGEchoMakeError(CGEchoErrorInvalidArgument, @"Discovery timeout must be 0.1-60 seconds", @{ CGEchoErrorOperationKey : @"discover" })];
      return;
    }
    CGEchoTID tid = 0;
    if (![self allocateTID:&tid]) {
      [self finishDiscovery:session error:CGEchoMakeError(CGEchoErrorBusy, @"No transaction ID is available", @{ CGEchoErrorOperationKey : @"discover" })];
      return;
    }
    session.tid = tid;
    session.hasTID = YES;
    [self->discoveries addObject:session];

    CGEchoFrame* frame = [CGEchoFrame frameWithTID:tid SEOJ:CGEchoControllerEOJ DEOJ:CGEchoNodeProfileEOJ ESV:CGEchoESVGet properties:@[ [CGEchoFrameProperty propertyWithEPC:CGEchoEPCSelfNodeInstanceListS data:nil] ]];
    NSError* error = nil;
    if (![self->transport multicastData:frame.rawData error:&error]) {
      [self finishDiscovery:session error:CGEchoMakeError(CGEchoErrorTransportFailure, error.localizedDescription ?: @"Could not send the search", @{ CGEchoErrorOperationKey : @"discover", CGEchoErrorTIDKey : @(tid) })];
      return;
    }

    __weak CGEchoController* weakSelf = self;
    __weak CGEchoDiscoverySession* weakSession = session;
    session.timer = [self timerWithTimeout:window handler:^{
      CGEchoDiscoverySession* strongSession = weakSession;
      if (strongSession)
        [weakSelf finishDiscovery:strongSession error:nil];
    }];
  });
  return session;
}

- (void)finishDiscovery:(CGEchoDiscoverySession*)session error:(NSError*)error
{
  if (session.finished)
    return;
  session.finished = YES;
  if (session.timer) {
    dispatch_source_cancel(session.timer);
    session.timer = nil;
  }
  [discoveries removeObject:session];
  if (session.hasTID)
    [self retainTID:session.tid];

  NSMutableArray* nodes = [NSMutableArray array];
  for (NSString* address in session.seen) {
    CGEchoNodeRecord* node = nodeRecords[address];
    if (node)
      [nodes addObject:[self snapshotNode:node]];
  }
  void (^done)(NSArray*, NSError*) = session.completion;
  session.completion = nil;
  if (done)
    [self deliverCompletion:^{ done(nodes, error); }];
}

- (void)cancelDiscovery:(CGEchoDiscoverySession*)session
{
  dispatch_async(stateQueue, ^{
    [self finishDiscovery:session error:CGEchoMakeError(CGEchoErrorCancelled, @"The discovery was cancelled locally", @{ CGEchoErrorOperationKey : @"discover" })];
  });
}

#pragma mark Observation

- (id<CGEchoObservation>)observeNotificationsOfObject:(CGEchoRemoteObject*)object handler:(void (^)(CGEchoPropertyValue*))handler
{
  CGEchoObservationToken* token = [[CGEchoObservationToken alloc] init];
  token.controller = self;
  token.address = object.address;
  token.eoj = object.EOJ;
  token.handler = handler;
  dispatch_async(stateQueue, ^{
    if (!atomic_load(&token->valid))
      return;
    token.identifier = self->nextObservationID++;
    self->observations[@(token.identifier)] = token;
  });
  return token;
}

- (void)removeObservation:(CGEchoObservationToken*)token
{
  dispatch_async(stateQueue, ^{
    if (token.identifier)
      [self->observations removeObjectForKey:@(token.identifier)];
  });
}

@end
