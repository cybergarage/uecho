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

@interface CGEchoFrameProperty : NSObject
@property (nonatomic, readonly) CGEchoEPC EPC;
@property (nonatomic, readonly, copy) NSData* data;
+ (instancetype)propertyWithEPC:(CGEchoEPC)epc data:(nullable NSData*)data;
@end

/// Immutable, fully copied ECHONET Lite format 1 frame.
@interface CGEchoFrame : NSObject

/// Source address for received frames, nil for outgoing frames.
@property (nonatomic, readonly, copy, nullable) NSString* address;
@property (nonatomic, readonly) CGEchoTID TID;
@property (nonatomic, readonly) CGEchoEOJ SEOJ;
@property (nonatomic, readonly) CGEchoEOJ DEOJ;
@property (nonatomic, readonly) CGEchoESV ESV;
/// Properties of the single OPC list. Empty for SetGet family frames (not parsed in the MVP).
@property (nonatomic, readonly, copy) NSArray<CGEchoFrameProperty*>* properties;
/// The raw datagram for received frames, the encoding for outgoing frames.
@property (nonatomic, readonly, copy) NSData* rawData;

+ (instancetype)frameWithTID:(CGEchoTID)tid SEOJ:(CGEchoEOJ)seoj DEOJ:(CGEchoEOJ)deoj ESV:(CGEchoESV)esv properties:(NSArray<CGEchoFrameProperty*>*)properties;

/// Parses a received datagram. Returns nil on malformed input.
+ (nullable instancetype)frameWithData:(NSData*)data address:(nullable NSString*)address;

/// YES if ESV is a response, SNA, or notification-response code (0x5x, 0x7x except 0x73/0x74).
@property (nonatomic, readonly) BOOL isResponse;

@end

NS_ASSUME_NONNULL_END
