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

typedef void uEchoManufacture;

@interface CGEchoManufacture : NSObject {
}
- (id)initWithCObject:(uEchoManufacture*)cobj;
- (NSString*)name;
- (Byte)code;
@end
