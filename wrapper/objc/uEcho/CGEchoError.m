/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoError.h"
#import "CGEchoTypes.h"
#import "Private/CGEchoInternal.h"

NSErrorDomain const CGEchoErrorDomain = @"org.cybergarage.uecho";

NSErrorUserInfoKey const CGEchoErrorOperationKey = @"CGEchoOperation";
NSErrorUserInfoKey const CGEchoErrorAddressKey = @"CGEchoAddress";
NSErrorUserInfoKey const CGEchoErrorEOJKey = @"CGEchoEOJ";
NSErrorUserInfoKey const CGEchoErrorEPCKey = @"CGEchoEPC";
NSErrorUserInfoKey const CGEchoErrorTIDKey = @"CGEchoTID";
NSErrorUserInfoKey const CGEchoErrorESVKey = @"CGEchoESV";
NSErrorUserInfoKey const CGEchoErrorResponseDataKey = @"CGEchoResponseData";

const CGEchoEOJ CGEchoNodeProfileEOJ = 0x0EF001;
const CGEchoEOJ CGEchoNodeProfileReadOnlyEOJ = 0x0EF002;
const CGEchoEOJ CGEchoControllerEOJ = 0x05FF01;
const uint16_t CGEchoUDPPort = 3610;

NSError* CGEchoMakeError(CGEchoErrorCode code, NSString* description, NSDictionary* _Nullable info)
{
  NSMutableDictionary* userInfo = [NSMutableDictionary dictionaryWithDictionary:info ?: @{}];
  userInfo[NSLocalizedDescriptionKey] = description;
  return [NSError errorWithDomain:CGEchoErrorDomain code:code userInfo:userInfo];
}
