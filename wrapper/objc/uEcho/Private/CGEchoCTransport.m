/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoTransport.h"
#import <CGEcho/CGEchoError.h>
#import "CGEchoInternal.h"

#include <pthread.h>
#include <uecho/controller.h>
#include <uecho/message.h>
#include <uecho/node.h>

/// Shared between the transport and the native receive callback.
/// The handler is invoked while holding the mutex, so once -stop has cleared
/// it no further ObjC callback can be running or start.
@interface CGEchoCTransportContext : NSObject {
@public
  pthread_mutex_t mutex;
  CGEchoTransportFrameHandler handler;
}
@end

@implementation CGEchoCTransportContext

- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  pthread_mutex_init(&mutex, NULL);
  return self;
}

- (void)dealloc
{
  pthread_mutex_destroy(&mutex);
}

@end

static void CGEchoCTransportMessageListener(uEchoController* ctrl, uEchoMessage* msg)
{
  CGEchoCTransportContext* context = (__bridge CGEchoCTransportContext*)uecho_controller_getuserdata(ctrl);
  if (!context || !msg)
    return;

  @autoreleasepool {
    // Copy everything out of the borrowed C message before returning.
    NSData* bytes = nil;
    size_t size = uecho_message_size(msg);
    byte* raw = uecho_message_getbytes(msg);
    if (raw && 0 < size)
      bytes = [NSData dataWithBytes:raw length:size];
    const char* addr = uecho_message_getsourceaddress(msg);
    NSString* address = addr ? [NSString stringWithUTF8String:addr] : nil;
    if (!bytes || !address)
      return;
    CGEchoFrame* frame = [CGEchoFrame frameWithData:bytes address:address];
    if (!frame)
      return;

    pthread_mutex_lock(&context->mutex);
    CGEchoTransportFrameHandler handler = context->handler;
    if (handler)
      handler(frame);
    pthread_mutex_unlock(&context->mutex);
  }
}

@implementation CGEchoCTransport {
  uEchoController* cController;
  CGEchoCTransportContext* context;
  BOOL running;
}

- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  context = [[CGEchoCTransportContext alloc] init];
  return self;
}

- (void)dealloc
{
  [self stop];
  if (cController) {
    // uecho_controller_stop() joins the receive workers (POSIX threads are
    // joinable and the server stops workers before freeing their sockets),
    // so no worker is inside the C library when the controller is deleted.
    uecho_controller_delete(cController);
    cController = NULL;
  }
}

- (BOOL)startWithFrameHandler:(CGEchoTransportFrameHandler)handler error:(NSError**)error
{
  if (running)
    return YES;

  if (!cController) {
    cController = uecho_controller_new();
    if (!cController) {
      if (error)
        *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"Could not allocate the native controller", @{ CGEchoErrorOperationKey : @"start" });
      return NO;
    }
    uecho_controller_setuserdata(cController, (__bridge void*)context);
    uecho_controller_setmessagelistener(cController, CGEchoCTransportMessageListener);
  }

  pthread_mutex_lock(&context->mutex);
  context->handler = [handler copy];
  pthread_mutex_unlock(&context->mutex);

  if (!uecho_controller_start(cController)) {
    [self clearHandler];
    uecho_controller_stop(cController);
    if (error)
      *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"Could not start the native controller (UDP 3610 / multicast)", @{ CGEchoErrorOperationKey : @"start" });
    return NO;
  }
  running = YES;
  return YES;
}

- (void)clearHandler
{
  pthread_mutex_lock(&context->mutex);
  context->handler = nil;
  pthread_mutex_unlock(&context->mutex);
}

- (void)stop
{
  [self clearHandler];
  if (running && cController)
    uecho_controller_stop(cController);
  running = NO;
}

- (BOOL)sendData:(NSData*)data toAddress:(NSString*)address error:(NSError**)error
{
  BOOL ok = running && cController && address.length > 0 && uecho_node_sendmessagebytes(uecho_controller_getlocalnode(cController), address.UTF8String, (byte*)data.bytes, data.length);
  if (!ok && error)
    *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"Could not send the datagram", @{ CGEchoErrorAddressKey : address ?: @"" });
  return ok;
}

- (BOOL)multicastData:(NSData*)data error:(NSError**)error
{
  BOOL ok = running && cController && uecho_node_announcemessagebytes(uecho_controller_getlocalnode(cController), (byte*)data.bytes, data.length);
  if (!ok && error)
    *error = CGEchoMakeError(CGEchoErrorTransportFailure, @"Could not send the multicast datagram", nil);
  return ok;
}

@end
