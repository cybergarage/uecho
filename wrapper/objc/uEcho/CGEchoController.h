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

#import <CGEcho/CGEchoControllerConfiguration.h>
#import <CGEcho/CGEchoRemoteNode.h>
#import <CGEcho/CGEchoTypes.h>

NS_ASSUME_NONNULL_BEGIN

@class CGEchoController;

/// Handle for an accepted request. Every accepted request completes exactly once.
@protocol CGEchoRequest <NSObject>
/// Ends local waiting and completes with CGEchoErrorCancelled if still pending.
/// A datagram that was already sent, and any device activity, is not recalled.
- (void)cancel;
@end

/// Handle for a local notification listener. ECHONET Lite has no subscription protocol.
@protocol CGEchoObservation <NSObject>
- (void)invalidate;
@end

/// Delegate methods are called on the configuration's callbackQueue.
@protocol CGEchoControllerDelegate <NSObject>
@optional
- (void)echoController:(CGEchoController*)controller didAddNode:(CGEchoRemoteNode*)node;
- (void)echoController:(CGEchoController*)controller didUpdateNode:(CGEchoRemoteNode*)node;
- (void)echoController:(CGEchoController*)controller didReceiveNotification:(CGEchoPropertyValue*)value;
@end

typedef NS_ENUM(NSInteger, CGEchoControllerState) {
  CGEchoControllerStateStopped = 0,
  CGEchoControllerStateStarting = 1,
  CGEchoControllerStateRunning = 2,
  CGEchoControllerStateStopping = 3,
};

typedef NS_OPTIONS(NSUInteger, CGEchoWriteOptions) {
  CGEchoWriteOptionsNone = 0,
  /// Send SetC even if the Set property map (0x9E) has not been fetched.
  CGEchoWriteOptionsAllowUnknownCapability = 1 << 0,
};

/// ECHONET Lite controller for remote nodes (client side only).
///
/// Only one CGEchoController per process is supported, because each one binds UDP 3610.
/// All callbacks are delivered on configuration.callbackQueue, never on a native receive thread.
/// Requests are never retried automatically; in particular a timed-out SetC is not re-sent.
@interface CGEchoController : NSObject

- (instancetype)init;
- (nullable instancetype)initWithConfiguration:(CGEchoControllerConfiguration*)configuration;

@property (nonatomic, readonly, copy) CGEchoControllerConfiguration* configuration;
@property (atomic, weak, nullable) id<CGEchoControllerDelegate> delegate;
@property (nonatomic, readonly) CGEchoControllerState state;

/// Starts networking. Calling while running completes immediately without error.
- (void)startWithCompletion:(void (^_Nullable)(NSError* _Nullable error))completion;

/// Stops networking. Pending requests complete once with CGEchoErrorNotRunning before
/// the completion is called; no event from the stopped session is delivered after it.
- (void)stopWithCompletion:(void (^_Nullable)(void))completion;

/// Snapshot of the registry of the current session.
@property (nonatomic, readonly, copy) NSArray<CGEchoRemoteNode*>* nodes;

/// Sends a node profile search (Get 0xD6) and collects responses for `timeout` seconds
/// (<= 0 uses configuration.discoveryTimeout). Completes with the nodes seen during the window;
/// an empty array is a valid result. Instance list notifications (0xD5) are also accepted.
- (id<CGEchoRequest>)discoverWithTimeout:(NSTimeInterval)timeout
                              completion:(void (^)(NSArray<CGEchoRemoteNode*>* nodes, NSError* _Nullable error))completion;

/// Removes a node from the registry. Its snapshots can no longer be used for requests.
- (void)forgetNode:(CGEchoRemoteNode*)node;

/// Fetches the Get / Set / Announce property maps (0x9F / 0x9E / 0x9D).
/// Completes with the updated object; individual map failures are reported in its capabilities.
- (id<CGEchoRequest>)fetchCapabilitiesOfObject:(CGEchoRemoteObject*)object
                                    completion:(void (^)(CGEchoRemoteObject* _Nullable object, NSError* _Nullable error))completion;

/// Sends Get (0x62) for one EPC and waits for Get_Res (0x72) or Get_SNA (0x52).
- (id<CGEchoRequest>)readProperty:(CGEchoEPC)epc
                         ofObject:(CGEchoRemoteObject*)object
                          timeout:(NSTimeInterval)timeout
                       completion:(void (^)(CGEchoPropertyValue* _Nullable value, NSError* _Nullable error))completion;

/// Sends SetC (0x61) for one EPC and waits for Set_Res (0x71) or SetC_SNA (0x51).
/// Success means the device accepted the request, not that its physical state changed;
/// read the property back if the application needs confirmation. A timeout means the
/// result is unknown. Refused locally with CGEchoErrorCapabilityUnknown unless the Set
/// property map is known or CGEchoWriteOptionsAllowUnknownCapability is given.
- (id<CGEchoRequest>)writeProperty:(CGEchoEPC)epc
                              data:(NSData*)data
                          ofObject:(CGEchoRemoteObject*)object
                           options:(CGEchoWriteOptions)options
                           timeout:(NSTimeInterval)timeout
                        completion:(void (^)(NSError* _Nullable error))completion;

/// Registers a local listener for INF (0x73) property notifications.
/// Pass nil to observe every object. Each received property is delivered once.
- (id<CGEchoObservation>)observeNotificationsOfObject:(nullable CGEchoRemoteObject*)object
                                              handler:(void (^)(CGEchoPropertyValue* value))handler;

/// Last successful value per EPC for the object in the current session, or nil.
- (nullable CGEchoPropertyValue*)lastValueOfProperty:(CGEchoEPC)epc ofObject:(CGEchoRemoteObject*)object;

@end

NS_ASSUME_NONNULL_END
