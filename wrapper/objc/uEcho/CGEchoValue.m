/******************************************************************
 * uEcho for ObjC
 * Copyright (C) The uecho Authors 2026
 * This is licensed under BSD-style license, see file COPYING.
 ******************************************************************/
#import <CGEcho/CGEchoError.h>
#import <CGEcho/CGEchoValue.h>
#include <uecho/value.h>

@implementation CGEchoValue
+ (instancetype)decodeData:(NSData*)data groupCode:(uint8_t)groupCode classCode:(uint8_t)classCode epc:(uint8_t)epc error:(NSError**)error
{
  uEchoValue value;
  if (!uecho_value_decode(uecho_standard_getvalueconstraint(groupCode, classCode, epc), data.bytes, data.length, &value)) {
    if (error)
      *error = [NSError errorWithDomain:CGEchoErrorDomain code:CGEchoErrorInvalidArgument userInfo:nil];
    return nil;
  }
  CGEchoValue* result = [[self alloc] initPrivate];
  result->_status = (CGEchoValueStatus)value.status;
  result->_rawData = [NSData dataWithBytes:value.raw length:value.rawSize];
  result->_integer = value.integer;
  result->_number = value.number;
  result->_code = value.code;
  result->_readOnly = value.readOnly;
  result->_name = value.name ? [NSString stringWithUTF8String:value.name] : nil;
  result->_unit = value.unit ? [NSString stringWithUTF8String:value.unit] : nil;
  return result;
}

- (instancetype)initPrivate
{
  return [super init];
}

+ (NSData*)encodeInteger:(int64_t)integer kind:(CGEchoValueStatus)kind groupCode:(uint8_t)groupCode classCode:(uint8_t)classCode epc:(uint8_t)epc error:(NSError**)error
{
  byte data[4];
  size_t size;
  const uEchoValueConstraint* constraint = uecho_standard_getvalueconstraint(groupCode, classCode, epc);
  if (!uecho_value_encode(constraint, (uEchoValueStatus)kind, integer, data, sizeof(data), &size)) {
    if (error)
      *error = [NSError errorWithDomain:CGEchoErrorDomain code:constraint ? CGEchoErrorInvalidArgument : CGEchoErrorUnsupportedProperty userInfo:nil];
    return nil;
  }
  return [NSData dataWithBytes:data length:size];
}
@end
