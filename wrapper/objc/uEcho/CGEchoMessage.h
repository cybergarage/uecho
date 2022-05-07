/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <Foundation/NSArray.h>
#import <Foundation/NSString.h>

#import "CGEchoESV.h"

typedef void uEchoMessage;

@interface CGEchoMessage : NSObject {
}
- (id)init;
- (id)initWithCObject:(uEchoMessage*)cobj;
- (BOOL)setESV:(CGEchoESV)esv;
- (CGEchoESV)ESV;
@end
