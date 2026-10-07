/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoFakeTransport.h"
#import "CGEchoInternal.h"

@implementation CGEchoFakeTransport {
  NSMutableArray<CGEchoFrame*>* sent;
  NSMutableArray* addresses;
  CGEchoTransportFrameHandler handler;
  dispatch_queue_t receiveQueue;
}

- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  sent = [NSMutableArray array];
  addresses = [NSMutableArray array];
  receiveQueue = dispatch_queue_create("org.cybergarage.uecho.test.receive", DISPATCH_QUEUE_SERIAL);
  return self;
}

- (BOOL)startWithFrameHandler:(CGEchoTransportFrameHandler)aHandler error:(NSError**)error
{
  @synchronized(self)
  {
    _startCount++;
    if (self.failStart) {
      if (error)
        *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"fake start failure", nil);
      return NO;
    }
    handler = [aHandler copy];
  }
  return YES;
}

- (void)stop
{
  @synchronized(self)
  {
    _stopCount++;
    handler = nil;
  }
}

- (BOOL)record:(NSData*)data address:(NSString*)address error:(NSError**)error
{
  if (self.failSend) {
    if (error)
      *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"fake send failure", nil);
    return NO;
  }
  CGEchoFrame* frame = [CGEchoFrame frameWithData:data address:nil];
  @synchronized(self)
  {
    [sent addObject:frame];
    [addresses addObject:address ?: (id)[NSNull null]];
  }
  CGEchoFakeResponder responder = self.responder;
  NSArray<CGEchoFrame*>* replies = responder ? responder(frame, address) : nil;
  for (CGEchoFrame* reply in replies)
    [self injectFrame:reply fromAddress:reply.address ?: address ?: @"192.0.2.1"];
  return YES;
}

- (BOOL)sendData:(NSData*)data toAddress:(NSString*)address error:(NSError**)error
{
  return [self record:data address:address error:error];
}

- (BOOL)multicastData:(NSData*)data error:(NSError**)error
{
  return [self record:data address:nil error:error];
}

- (NSArray<CGEchoFrame*>*)sentFrames
{
  @synchronized(self)
  {
    return [sent copy];
  }
}

- (NSArray*)sentAddresses
{
  @synchronized(self)
  {
    return [addresses copy];
  }
}

- (void)injectFrame:(CGEchoFrame*)frame fromAddress:(NSString*)address
{
  CGEchoFrame* received = [CGEchoFrame frameWithData:frame.rawData address:address];
  dispatch_async(receiveQueue, ^{
    CGEchoTransportFrameHandler h;
    @synchronized(self)
    {
      h = self->handler;
    }
    if (h && received)
      h(received);
  });
}

@end

CGEchoFrame* CGEchoTestReply(CGEchoFrame* request, CGEchoESV esv, CGEchoEPC epc, NSData* data)
{
  return [CGEchoFrame frameWithTID:request.TID SEOJ:request.DEOJ DEOJ:request.SEOJ ESV:esv properties:@[ [CGEchoFrameProperty propertyWithEPC:epc data:data] ]];
}

NSData* CGEchoTestInstanceList(NSArray<NSNumber*>* eojs)
{
  NSMutableData* data = [NSMutableData data];
  uint8_t count = (uint8_t)eojs.count;
  [data appendBytes:&count length:1];
  for (NSNumber* eoj in eojs) {
    uint32_t v = eoj.unsignedIntValue;
    uint8_t b[3] = { (v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF };
    [data appendBytes:b length:3];
  }
  return data;
}
