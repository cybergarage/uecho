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

#import <stdio.h>

typedef void uEchoProperty;

@interface CGEchoProperty : NSObject {
}
- (id)init;
- (id)initWithCObject:(uEchoProperty*)cobj;
- (NSString*)name;
- (BOOL)setCode:(Byte)code;
- (BOOL)setData:(Byte*)data size:(size_t)size;
- (Byte)code;
- (Byte*)data;
- (size_t)size;
@end
