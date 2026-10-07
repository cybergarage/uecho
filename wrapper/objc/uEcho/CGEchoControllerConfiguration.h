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

NS_ASSUME_NONNULL_BEGIN

/// Settings copied by CGEchoController at initialization.
///
/// The numeric defaults are initial design values, not measured results.
@interface CGEchoControllerConfiguration : NSObject <NSCopying>

+ (instancetype)defaultConfiguration;

/// Queue for all public callbacks. Must be a serial queue. Default: main queue.
@property (nonatomic, strong) dispatch_queue_t callbackQueue;

/// Default discovery window in seconds. Default 3. Allowed 0.5–60.
@property (nonatomic) NSTimeInterval discoveryTimeout;

/// Default request timeout in seconds. Default 5. Allowed 0.1–60.
@property (nonatomic) NSTimeInterval requestTimeout;

/// Maximum number of pending requests. Default 8. Allowed 1–64.
@property (nonatomic) NSUInteger maxConcurrentRequests;

/// Seconds since a node was last seen before it is reported as stale. Default 300. Allowed 1–86400.
@property (nonatomic) NSTimeInterval staleInterval;

/// Seconds a finished TID is kept so that late responses are ignored and the TID is not reused. Default 10. Allowed 0–120.
@property (nonatomic) NSTimeInterval lateResponseRetention;

/// Returns NO and an CGEchoErrorInvalidArgument error if a value is out of range.
- (BOOL)validate:(NSError* _Nullable* _Nullable)error;

@end

NS_ASSUME_NONNULL_END
