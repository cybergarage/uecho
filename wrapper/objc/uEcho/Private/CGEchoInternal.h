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

#import <CGEcho/CGEchoCapabilities.h>
#import <CGEcho/CGEchoController.h>
#import <CGEcho/CGEchoError.h>
#import <CGEcho/CGEchoRemoteNode.h>
#import "CGEchoTransport.h"

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSError* CGEchoMakeError(CGEchoErrorCode code, NSString* description, NSDictionary* _Nullable info);

@interface CGEchoPropertyMap ()
+ (instancetype)unknownMapWithEPC:(CGEchoEPC)mapEPC;
+ (instancetype)mapWithEPC:(CGEchoEPC)mapEPC rawData:(NSData*)rawData;
+ (instancetype)failedMapWithEPC:(CGEchoEPC)mapEPC error:(NSError*)error rawData:(nullable NSData*)rawData;
@end

@interface CGEchoCapabilities ()
+ (instancetype)unknownCapabilities;
- (instancetype)initWithReadable:(CGEchoPropertyMap*)readable writable:(CGEchoPropertyMap*)writable notifiable:(CGEchoPropertyMap*)notifiable;
@end

@interface CGEchoRemoteObject ()
- (instancetype)initWithAddress:(NSString*)address EOJ:(CGEchoEOJ)eoj capabilities:(nullable CGEchoCapabilities*)capabilities generation:(uint64_t)generation;
@end

@interface CGEchoRemoteNode ()
- (instancetype)initWithAddress:(NSString*)address objects:(NSArray<CGEchoRemoteObject*>*)objects hasObjectList:(BOOL)hasObjectList lastSeen:(NSDate*)lastSeen availability:(CGEchoNodeAvailability)availability generation:(uint64_t)generation;
@end

@interface CGEchoPropertyValue ()
- (instancetype)initWithAddress:(NSString*)address EOJ:(CGEchoEOJ)eoj EPC:(CGEchoEPC)epc data:(NSData*)data receivedAt:(NSDate*)receivedAt source:(CGEchoPropertyValueSource)source;
@end

@interface CGEchoController ()
/// Designated initializer for tests: injects a transport (e.g. a fake).
- (nullable instancetype)initWithConfiguration:(CGEchoControllerConfiguration*)configuration transport:(id<CGEchoTransport>)transport;
/// Current session generation (thread-safe).
@property (nonatomic, readonly) uint64_t generation;
@end

NS_ASSUME_NONNULL_END
