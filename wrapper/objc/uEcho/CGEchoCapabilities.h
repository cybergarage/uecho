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

/// Immutable metadata copied from the compiled C standard tables, independent of a controller.
/// Attributes describe standard definitions; only remote property maps establish device support.
@interface CGEchoStandardProperty : NSObject <NSCopying>
- (instancetype)init NS_UNAVAILABLE;
@property (nonatomic, readonly) CGEchoEPC epc;
@property (nonatomic, readonly, copy) NSString* name;
/// Raw uEchoPropertyAttr flags, including separate mandatory bits.
@property (nonatomic, readonly) uint8_t standardAttributes;
/// YES when this build retains the original MRA data schema.
@property (nonatomic, readonly) BOOL hasValueMetadata;
/// Immutable original MRA schema; refs resolve via CGEchoStandardClass.valueDefinitions.
/// Type/size/enum/range/unit are present only where defined.
/// nil when disabled or absent. This is metadata, not a generic EDT decoder.
@property (nonatomic, readonly, copy, nullable) NSDictionary<NSString*, id>* valueSchema;
@end

@interface CGEchoStandardClass : NSObject <NSCopying>
- (instancetype)init NS_UNAVAILABLE;
@property (nonatomic, readonly) uint8_t groupCode;
@property (nonatomic, readonly) uint8_t classCode;
@property (nonatomic, readonly, copy) NSString* name;
@property (nonatomic, readonly, copy) NSDictionary<NSNumber*, CGEchoStandardProperty*>* properties;
/// nil for an unknown or excluded class; never fabricates superclass-only device metadata.
+ (nullable instancetype)classWithGroupCode:(uint8_t)groupCode classCode:(uint8_t)classCode;
/// Standard-known, readable definitions intersected with a successfully fetched device Get map.
/// Unknown/vendor EPCs remain in the original map and are not discarded from device capabilities.
- (NSDictionary<NSNumber*, CGEchoStandardProperty*>*)readableDefinitionsForMap:(CGEchoPropertyMap*)map;
/// Provenance of the checked-in generated tables, not a conformance certification.
+ (NSString*)sourceDescription;
+ (BOOL)hasFullDatabase;
/// Shared immutable definitions for local #/definitions/name refs, or nil when disabled.
/// Schema siblings are preserved verbatim; this API does not guess merge/decoding rules.
+ (nullable NSDictionary<NSString*, id>*)valueDefinitions;
@end

NS_ASSUME_NONNULL_END
