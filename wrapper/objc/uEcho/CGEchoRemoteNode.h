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
#import <CGEcho/CGEchoTypes.h>

NS_ASSUME_NONNULL_BEGIN

/// Immutable snapshot of a remote ECHONET object.
///
/// Snapshots stay readable after the session ends, but requests resolve the
/// object against the current registry and fail with
/// CGEchoErrorDeviceUnavailable for snapshots of an earlier session.
/// Identity is endpoint address + EOJ; an IP address is not a permanent device ID.
@interface CGEchoRemoteObject : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

@property (nonatomic, readonly, copy) NSString* address;
@property (nonatomic, readonly) CGEchoEOJ EOJ;
@property (nonatomic, readonly) uint8_t classGroupCode;
@property (nonatomic, readonly) uint8_t classCode;
@property (nonatomic, readonly) uint8_t instanceCode;
@property (nonatomic, readonly) CGEchoCapabilities* capabilities;
/// Session generation in which this snapshot was taken.
@property (nonatomic, readonly) uint64_t generation;

@end

typedef NS_ENUM(NSInteger, CGEchoNodeAvailability) {
  /// Seen within the configured stale interval.
  CGEchoNodeAvailabilityActive = 0,
  /// Not seen within the stale interval. ECHONET Lite has no removal message, so the node is not removed automatically.
  CGEchoNodeAvailabilityStale = 1,
};

/// Immutable snapshot of a remote ECHONET node.
@interface CGEchoRemoteNode : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

/// Source address the node responded from. The destination port is always 3610.
@property (nonatomic, readonly, copy) NSString* address;
/// Device objects from the latest complete instance list (node profile excluded).
@property (nonatomic, readonly, copy) NSArray<CGEchoRemoteObject*>* objects;
/// YES once a complete instance list (D5 or D6) has been received.
@property (nonatomic, readonly) BOOL hasObjectList;
@property (nonatomic, readonly, copy) NSDate* lastSeen;
@property (nonatomic, readonly) CGEchoNodeAvailability availability;
@property (nonatomic, readonly) uint64_t generation;

- (nullable CGEchoRemoteObject*)objectWithEOJ:(CGEchoEOJ)eoj;

@end

typedef NS_ENUM(NSInteger, CGEchoPropertyValueSource) {
  CGEchoPropertyValueSourceGet = 0,
  CGEchoPropertyValueSourceNotification = 1,
};

/// Immutable property value received from a device.
@interface CGEchoPropertyValue : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

@property (nonatomic, readonly, copy) NSString* address;
@property (nonatomic, readonly) CGEchoEOJ EOJ;
@property (nonatomic, readonly) CGEchoEPC EPC;
/// Raw EDT bytes. Class specific decoding is left to the application.
@property (nonatomic, readonly, copy) NSData* data;
@property (nonatomic, readonly, copy) NSDate* receivedAt;
@property (nonatomic, readonly) CGEchoPropertyValueSource source;

@end

NS_ASSUME_NONNULL_END
