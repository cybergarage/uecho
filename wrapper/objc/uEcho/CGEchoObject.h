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

typedef void uEchoObject;

@interface CGEchoObject : NSObject {
}
- (id)initWithCObject:(uEchoObject*)cobj;
- (NSArray*)properties;
- (NSString*)name;
- (int)code;
- (Byte)group;
- (Byte)class;
@end
