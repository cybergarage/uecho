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

#import <CGEcho/CGEchoTypes.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, CGEchoCapabilityState) {
  /// The map has not been fetched in this session.
  CGEchoCapabilityStateUnknown = 0,
  /// The map was fetched and validated. `properties` may be empty (valid-empty).
  CGEchoCapabilityStateAvailable = 1,
  /// Fetching failed (SNA, timeout or malformed). See `error` and `rawData`.
  CGEchoCapabilityStateFailed = 2,
};

/// One property map (0x9D, 0x9E or 0x9F) of a remote object.
@interface CGEchoPropertyMap : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

@property (nonatomic, readonly) CGEchoEPC mapEPC;
@property (nonatomic, readonly) CGEchoCapabilityState state;
/// EPCs (0x80–0xFF) declared by the device. nil unless state is Available.
@property (nonatomic, readonly, copy, nullable) NSIndexSet* properties;
/// Raw EDT as received, kept for diagnosis even when decoding failed.
@property (nonatomic, readonly, copy, nullable) NSData* rawData;
@property (nonatomic, readonly, nullable) NSError* error;

/// YES only when state is Available and the EPC is listed.
- (BOOL)containsProperty:(CGEchoEPC)epc NS_SWIFT_NAME(contains(_:));

/// Decodes an ECHONET property map EDT (format 1: count < 16, format 2: 17-byte bitmap).
/// Validates length, declared count, range and duplicates. Returns nil and
/// a CGEchoErrorMalformedResponse error when the map is invalid.
+ (nullable NSIndexSet*)decodePropertyMapData:(NSData*)data error:(NSError* _Nullable* _Nullable)error;

@end

/// The three capability maps of a remote object.
@interface CGEchoCapabilities : NSObject <NSCopying>

- (instancetype)init NS_UNAVAILABLE;

/// Get property map (0x9F).
@property (nonatomic, readonly) CGEchoPropertyMap* readable;
/// Set property map (0x9E).
@property (nonatomic, readonly) CGEchoPropertyMap* writable;
/// Status change announcement property map (0x9D).
@property (nonatomic, readonly) CGEchoPropertyMap* notifiable;

@end

NS_ASSUME_NONNULL_END
