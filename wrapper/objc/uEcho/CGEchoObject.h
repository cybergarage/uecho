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
@property (assign) uEchoObject* cObject;
- (id)initWithCObject:(uEchoObject*)cobj;
@end
