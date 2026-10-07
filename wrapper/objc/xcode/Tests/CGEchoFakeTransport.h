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
#import "CGEchoTransport.h"

NS_ASSUME_NONNULL_BEGIN

/// Returns frames to inject in reply to an outgoing frame. `address` is nil for multicast.
typedef NSArray<CGEchoFrame*>* _Nullable (^CGEchoFakeResponder)(CGEchoFrame* request, NSString* _Nullable address);

/// In-memory transport for tests. Injected frames are delivered on a background queue,
/// like the native receive thread.
@interface CGEchoFakeTransport : NSObject <CGEchoTransport>

@property (atomic, copy, nullable) CGEchoFakeResponder responder;
@property (atomic) BOOL failStart;
@property (atomic) BOOL failSend;
@property (atomic, readonly) NSUInteger startCount;
@property (atomic, readonly) NSUInteger stopCount;

/// Outgoing frames (unicast and multicast), in send order.
- (NSArray<CGEchoFrame*>*)sentFrames;
/// Destination of each sent frame; NSNull for multicast.
- (NSArray*)sentAddresses;

/// Delivers a received frame from `address` as if it came from the network.
- (void)injectFrame:(CGEchoFrame*)frame fromAddress:(NSString*)address;

@end

/// Builds a reply to `request`: same TID, SEOJ/DEOJ swapped.
FOUNDATION_EXPORT CGEchoFrame* CGEchoTestReply(CGEchoFrame* request, CGEchoESV esv, CGEchoEPC epc, NSData* _Nullable data);

/// Builds an instance list EDT (count + 3-byte EOJs).
FOUNDATION_EXPORT NSData* CGEchoTestInstanceList(NSArray<NSNumber*>* eojs);

NS_ASSUME_NONNULL_END
