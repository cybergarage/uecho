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

FOUNDATION_EXPORT NSErrorDomain const CGEchoErrorDomain;

typedef NS_ERROR_ENUM(CGEchoErrorDomain, CGEchoErrorCode) {
  /// An argument was out of range (EPC, EDT length, timeout, configuration).
  CGEchoErrorInvalidArgument = 1,
  /// The controller is not running, or it stopped before the request finished.
  CGEchoErrorNotRunning = 2,
  /// Too many requests are pending; the request was not queued.
  CGEchoErrorBusy = 3,
  /// The object is unknown, forgotten or belongs to a previous session.
  CGEchoErrorDeviceUnavailable = 4,
  /// The device answered SNA, or a known property map excludes the EPC.
  CGEchoErrorUnsupportedProperty = 5,
  /// SetC was refused locally because the Set property map is not known.
  CGEchoErrorCapabilityUnknown = 6,
  /// The datagram could not be sent or the transport failed to start.
  CGEchoErrorTransportFailure = 7,
  /// The device answered with an unexpected protocol response.
  CGEchoErrorProtocolFailure = 8,
  /// The response could not be parsed or failed validation.
  CGEchoErrorMalformedResponse = 9,
  /// No matching response arrived before the deadline. The device state is unknown.
  CGEchoErrorTimeout = 10,
  /// The request was cancelled locally. Sent datagrams are not recalled.
  CGEchoErrorCancelled = 11,
};

/// NSString describing the operation (e.g. "read", "write", "discover").
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorOperationKey;
/// NSString remote address.
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorAddressKey;
/// NSNumber (CGEchoEOJ).
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorEOJKey;
/// NSNumber (CGEchoEPC).
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorEPCKey;
/// NSNumber (CGEchoTID).
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorTIDKey;
/// NSNumber (CGEchoESV) of the response, when one was received.
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorESVKey;
/// NSData raw response frame, when one was received.
FOUNDATION_EXPORT NSErrorUserInfoKey const CGEchoErrorResponseDataKey;

NS_ASSUME_NONNULL_END
