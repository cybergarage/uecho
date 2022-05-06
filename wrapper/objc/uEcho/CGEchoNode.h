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

#import "CGEchoNode.h"

#if !defined(_UECHO_NODE_H_)
typedef void uEchoNode;
#endif

@interface CGEchoNode : NSObject {
}
@property (assign) uEchoNode* cObject;
- (id)initWithCObject:(uEchoNode*)cobj;
- (NSString*)address;
@end
