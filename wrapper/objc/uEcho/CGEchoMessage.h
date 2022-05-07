/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <Foundation/NSArray.h>
#import <Foundation/NSString.h>

typedef NS_ENUM(Byte, CGEchoESV) {
  CGEchoESVWriteRequest = 0x60,
  CGEchoESVWriteRequestResponseRequired = 0x61,
  CGEchoESVReadRequest = 0x62,
  CGEchoESVNotificationRequest = 0x63,
  CGEchoESVWriteReadRequest = 0x6E,

  CGEchoESVWriteResponse = 0x71,
  CGEchoESVReadResponse = 0x72,
  CGEchoESVNotification = 0x73,
  CGEchoESVNotificationResponseRequired = 0x74,
  CGEchoESVNotificationResponse = 0x7A,
  CGEchoESVWriteReadResponse = 0x7E,

  CGEchoESVWriteRequestError = 0x50,
  CGEchoESVWriteRequestResponseRequiredError = 0x51,
  CGEchoESVReadRequestError = 0x52,
  CGEchoESVNotificationRequestError = 0x53,
  CGEchoESVWriteReadRequestError = 0x5E,
};

typedef void uEchoMessage;

@interface CGEchoMessage : NSObject {
}
- (id)init;
- (id)initWithCObject:(uEchoMessage*)cobj;
- (NSArray*)properties;
- (NSArray*)setproperties;
- (NSArray*)getproperties;
- (BOOL)setESV:(CGEchoESV)esv;
- (CGEchoESV)ESV;
- (BOOL)setDestination:(int)code;
- (int)destination;
- (BOOL)addProperty:(Byte)code data:(Byte*)data size:(size_t)size;
- (BOOL)addGetProperty:(Byte)code data:(Byte*)data size:(size_t)size;
- (BOOL)addSetProperty:(Byte)code data:(Byte*)data size:(size_t)size;
@end
