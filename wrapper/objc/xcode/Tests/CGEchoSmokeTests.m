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

#include "uecho/net/interface.h"

/// Network smoke test against a real ECHONET Lite node.
///
/// Skipped unless CGECHO_SMOKE=1 (the CGEchoSmoke scheme sets it). Start a
/// node first, e.g. the uecholight scheme (mono functional lighting, 0x029101).
/// The target EOJ can be changed with CGECHO_SMOKE_EOJ (hex, default 029101).
///
/// The test writes to the target, so it only targets a node running on this
/// host (one of its interface addresses) unless CGECHO_SMOKE_ADDRESS names
/// another node explicitly. Other devices on the LAN are never written.
@interface CGEchoSmokeTests : XCTestCase
@property (nonatomic, strong) CGEchoController* controller;
@end

@implementation CGEchoSmokeTests

- (void)setUp
{
  NSDictionary* env = NSProcessInfo.processInfo.environment;
  if (![env[@"CGECHO_SMOKE"] isEqualToString:@"1"])
    return;
  CGEchoControllerConfiguration* config = [CGEchoControllerConfiguration defaultConfiguration];
  config.callbackQueue = dispatch_queue_create("org.cybergarage.uecho.smoke", DISPATCH_QUEUE_SERIAL);
  self.controller = [[CGEchoController alloc] initWithConfiguration:config];
}

- (CGEchoEOJ)targetEOJ
{
  NSString* hex = NSProcessInfo.processInfo.environment[@"CGECHO_SMOKE_EOJ"] ?: @"029101";
  unsigned int value = 0;
  [[NSScanner scannerWithString:hex] scanHexInt:&value];
  return value;
}

- (NSSet<NSString*>*)targetAddresses
{
  NSString* explicitAddress = NSProcessInfo.processInfo.environment[@"CGECHO_SMOKE_ADDRESS"];
  if (explicitAddress.length)
    return [NSSet setWithObject:explicitAddress];
  NSMutableSet* addresses = [NSMutableSet set];
  uEchoNetworkInterfaceList* list = uecho_net_interfacelist_new();
  uecho_net_gethostinterfaces(list);
  for (uEchoNetworkInterface* netIf = uecho_net_interfacelist_gets(list); netIf; netIf = uecho_net_interface_next(netIf)) {
    const char* addr = uecho_net_interface_getaddress(netIf);
    if (addr)
      [addresses addObject:[NSString stringWithUTF8String:addr]];
  }
  uecho_net_interfacelist_delete(list);
  return addresses;
}

- (NSError*)write:(NSData*)data object:(CGEchoRemoteObject*)object
{
  __block NSError* result = nil;
  XCTestExpectation* done = [self expectationWithDescription:@"write"];
  [self.controller writeProperty:0x80 data:data ofObject:object options:CGEchoWriteOptionsNone timeout:0 completion:^(NSError* error) {
    result = error;
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:10];
  return result;
}

- (CGEchoPropertyValue*)read:(CGEchoEPC)epc object:(CGEchoRemoteObject*)object
{
  __block CGEchoPropertyValue* result = nil;
  XCTestExpectation* done = [self expectationWithDescription:@"read"];
  [self.controller readProperty:epc ofObject:object timeout:0 completion:^(CGEchoPropertyValue* value, NSError* error) {
    XCTAssertNil(error, @"read %02X: %@", epc, error);
    result = value;
    [done fulfill];
  }];
  [self waitForExpectations:@[ done ] timeout:10];
  return result;
}

- (void)testDiscoverReadWriteAgainstRealNode
{
  if (!self.controller)
    XCTSkip(@"Set CGECHO_SMOKE=1 (CGEchoSmoke scheme) and start an ECHONET Lite node to run this test.");

  XCTestExpectation* started = [self expectationWithDescription:@"start"];
  [self.controller startWithCompletion:^(NSError* error) {
    XCTAssertNil(error, @"start: %@", error);
    [started fulfill];
  }];
  [self waitForExpectations:@[ started ] timeout:10];

  __block NSArray<CGEchoRemoteNode*>* nodes = nil;
  XCTestExpectation* found = [self expectationWithDescription:@"discover"];
  [self.controller discoverWithTimeout:3 completion:^(NSArray<CGEchoRemoteNode*>* result, NSError* error) {
    XCTAssertNil(error, @"discover: %@", error);
    nodes = result;
    [found fulfill];
  }];
  [self waitForExpectations:@[ found ] timeout:10];

  NSSet<NSString*>* targets = [self targetAddresses];
  CGEchoRemoteObject* object = nil;
  for (CGEchoRemoteNode* node in nodes) {
    NSMutableArray* eojs = [NSMutableArray array];
    for (CGEchoRemoteObject* o in node.objects)
      [eojs addObject:[NSString stringWithFormat:@"%06X", o.EOJ]];
    NSLog(@"[smoke] node %@ hasObjectList=%d objects=[%@]", node.address, node.hasObjectList, [eojs componentsJoinedByString:@","]);
    if ([targets containsObject:node.address])
      object = object ?: [node objectWithEOJ:self.targetEOJ];
  }
  XCTAssertNotNil(object, @"no node with EOJ %06X at %@ among %lu nodes; start the uecholight scheme or set CGECHO_SMOKE_ADDRESS", self.targetEOJ, [targets.allObjects componentsJoinedByString:@","], (unsigned long)nodes.count);
  if (!object)
    return;

  __block CGEchoRemoteObject* updated = nil;
  XCTestExpectation* fetched = [self expectationWithDescription:@"capabilities"];
  [self.controller fetchCapabilitiesOfObject:object completion:^(CGEchoRemoteObject* result, NSError* error) {
    XCTAssertNil(error, @"capabilities: %@", error);
    updated = result;
    [fetched fulfill];
  }];
  [self waitForExpectations:@[ fetched ] timeout:20];
  CGEchoCapabilities* caps = updated.capabilities;
  NSLog(@"[smoke] capabilities get=%@ set=%@ inf=%@", caps.readable, caps.writable, caps.notifiable);
  XCTAssertEqual(caps.readable.state, CGEchoCapabilityStateAvailable);
  XCTAssertTrue([caps.readable containsProperty:0x80]);
  XCTAssertEqual(caps.writable.state, CGEchoCapabilityStateAvailable);
  XCTAssertTrue([caps.writable containsProperty:0x80]);

  NSLog(@"[smoke] target %@ %06X", updated.address, updated.EOJ);
  CGEchoPropertyValue* original = [self read:0x80 object:updated];
  XCTAssertEqual(original.data.length, 1u);
  NSLog(@"[smoke] 0x80 = %@", original.data);

  const uint8_t off = 0x31;
  XCTAssertNil([self write:[NSData dataWithBytes:&off length:1] object:updated]);
  XCTAssertEqualObjects([self read:0x80 object:updated].data, [NSData dataWithBytes:&off length:1], @"readback after SetC");
  if (original.data.length == 1)
    XCTAssertNil([self write:original.data object:updated], @"restore");

  XCTestExpectation* stopped = [self expectationWithDescription:@"stop"];
  [self.controller stopWithCompletion:^{ [stopped fulfill]; }];
  [self waitForExpectations:@[ stopped ] timeout:10];
  self.controller = nil; // exercises native teardown (join + delete)
}

@end
