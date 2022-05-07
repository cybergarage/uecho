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

#import <CGEcho/CGEchoMessage.h>
#import <CGEcho/CGEchoNode.h>
#import <CGEcho/CGEchoObject.h>

@interface CGEchoController : NSObject {
}
- (id)init;
- (NSArray*)nodes;
- (BOOL)start;
- (BOOL)stop;
- (BOOL)search;
- (CGEchoMessage*)postMessage:(CGEchoNode*)node object:(CGEchoObject*)object message:(CGEchoMessage*)message;
@end
