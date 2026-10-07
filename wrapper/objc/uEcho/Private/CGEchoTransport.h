/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <Foundation/Foundation.h>

#import "CGEchoFrame.h"

NS_ASSUME_NONNULL_BEGIN

/// Called for each received frame, on an arbitrary (native receive) thread.
/// The frame is a full copy; implementations must not block.
typedef void (^CGEchoTransportFrameHandler)(CGEchoFrame* frame);

/// Datagram transport used by CGEchoController. Methods other than the
/// handler are always called on the controller's private state queue.
@protocol CGEchoTransport <NSObject>

- (BOOL)startWithFrameHandler:(CGEchoTransportFrameHandler)handler error:(NSError* _Nullable* _Nullable)error;

/// Stops receiving. After this returns the handler must not be called again.
- (void)stop;

- (BOOL)sendData:(NSData*)data toAddress:(NSString*)address error:(NSError* _Nullable* _Nullable)error;

- (BOOL)multicastData:(NSData*)data error:(NSError* _Nullable* _Nullable)error;

@end

/// Transport backed by the uEcho C controller (UDP 3610, multicast 224.0.23.0).
@interface CGEchoCTransport : NSObject <CGEchoTransport>
@end

NS_ASSUME_NONNULL_END
