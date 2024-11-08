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

@class CGEchoController;

@protocol CGEchoControllerObserver <NSObject>
@optional
- (void)nodeAdded:(CGEchoController*)controller node:(CGEchoNode*)node message:(CGEchoMessage*)msg;
- (void)nodeUpdated:(CGEchoController*)controller node:(CGEchoNode*)node message:(CGEchoMessage*)msg;
- (void)nodeAnnounced:(CGEchoController*)controller node:(CGEchoNode*)node message:(CGEchoMessage*)msg;
- (void)nodeResponsed:(CGEchoController*)controller node:(CGEchoNode*)node message:(CGEchoMessage*)msg;
@end

@interface CGEchoController : NSObject {
}
@property (assign) id<CGEchoControllerObserver> observer;
- (id)init;
- (NSArray*)nodes;
- (BOOL)start;
- (BOOL)stop;
- (BOOL)search;
- (BOOL)announceMessage:(CGEchoNode*)node message:(CGEchoMessage*)msg;
- (BOOL)sendMessage:(CGEchoNode*)node object:(CGEchoObject*)obj message:(CGEchoMessage*)msg;
- (CGEchoMessage*)postMessage:(CGEchoNode*)node object:(CGEchoObject*)obj message:(CGEchoMessage*)msg;
@end
