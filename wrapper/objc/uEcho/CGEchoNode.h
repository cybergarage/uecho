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

#import "CGEchoNode.h"

typedef void uEchoNode;

@interface CGEchoNode : NSObject {
}
@property (assign) uEchoNode* cObject;
- (id)initWithCObject:(uEchoNode*)cobj;
- (NSString*)address;
- (NSArray*)objects;
@end
