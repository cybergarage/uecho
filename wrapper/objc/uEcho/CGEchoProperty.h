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

typedef void uEchoProperty;

@interface CGEchoProperty : NSObject {
}
@property (assign) uEchoProperty* cObject;
- (id)initWithCObject:(uEchoProperty*)cobj;
@end
