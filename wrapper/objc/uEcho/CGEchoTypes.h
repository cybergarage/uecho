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

/// 24-bit ECHONET object code (class group, class, instance).
typedef uint32_t CGEchoEOJ;

/// 8-bit ECHONET property code.
typedef uint8_t CGEchoEPC;

/// 16-bit ECHONET transaction ID.
typedef uint16_t CGEchoTID;

/// ECHONET Lite service codes used by this SDK.
typedef NS_ENUM(uint8_t, CGEchoESV) {
  CGEchoESVSetI = 0x60,
  CGEchoESVSetC = 0x61,
  CGEchoESVGet = 0x62,
  CGEchoESVInfRequest = 0x63,
  CGEchoESVSetGet = 0x6E,
  CGEchoESVSetResponse = 0x71,
  CGEchoESVGetResponse = 0x72,
  CGEchoESVInf = 0x73,
  CGEchoESVInfC = 0x74,
  CGEchoESVInfCResponse = 0x7A,
  CGEchoESVSetGetResponse = 0x7E,
  CGEchoESVSetISNA = 0x50,
  CGEchoESVSetCSNA = 0x51,
  CGEchoESVGetSNA = 0x52,
  CGEchoESVInfSNA = 0x53,
  CGEchoESVSetGetSNA = 0x5E,
};

/// Node profile object (0x0EF001).
FOUNDATION_EXPORT const CGEchoEOJ CGEchoNodeProfileEOJ;
/// Read-only node profile object (0x0EF002).
FOUNDATION_EXPORT const CGEchoEOJ CGEchoNodeProfileReadOnlyEOJ;
/// Controller object used as SEOJ by this SDK (0x05FF01).
FOUNDATION_EXPORT const CGEchoEOJ CGEchoControllerEOJ;

/// ECHONET Lite UDP port.
FOUNDATION_EXPORT const uint16_t CGEchoUDPPort;

/// Well-known EPCs used by discovery and capabilities.
typedef NS_ENUM(uint8_t, CGEchoWellKnownEPC) {
  CGEchoEPCOperationStatus = 0x80,
  CGEchoEPCAnnouncePropertyMap = 0x9D,
  CGEchoEPCSetPropertyMap = 0x9E,
  CGEchoEPCGetPropertyMap = 0x9F,
  CGEchoEPCInstanceListNotification = 0xD5,
  CGEchoEPCSelfNodeInstanceListS = 0xD6,
};

NS_ASSUME_NONNULL_END
