/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoController.h"
#import "CGEchoNode.h"

#include <uecho/controller.h>

static void CGEchoControllerObserver(uEchoController*, uEchoNode*, uEchoNodeStatus, uEchoMessage*);

@implementation CGEchoController {
  uEchoController* cObject;
}
@synthesize observer;
- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_controller_new();
  if (cObject) {
    uecho_controller_setnodelistener(cObject, CGEchoControllerObserver);
    uecho_controller_setuserdata(cObject, (__bridge void*)self);
  }
  else {
    self = nil;
  }
  return self;
}

- (void)dealloc
{
  if (cObject) {
    uecho_controller_delete(cObject);
  }
}

- (NSArray*)nodes
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* nodes = [NSMutableArray array];
  for (uEchoNode* cNode = uecho_controller_getnodes(cObject); cNode; cNode = uecho_node_next(cNode)) {
    CGEchoNode* node = [[CGEchoNode alloc] initWithCObject:cNode];
    [nodes addObject:node];
  }
  return nodes;
}

- (BOOL)start
{
  if (!cObject)
    return NO;
  return uecho_controller_start(cObject);
}

- (BOOL)stop
{
  if (!cObject)
    return NO;
  return uecho_controller_stop(cObject);
}

- (BOOL)search
{
  if (!cObject)
    return NO;
  uecho_controller_search(cObject);
  return YES;
}

- (BOOL)announceMessage:(CGEchoNode*)node message:(CGEchoMessage*)msg
{
  if (!cObject)
    return NO;
  return uecho_controller_announcemessage(cObject, msg.cObject);
}

- (BOOL)sendMessage:(CGEchoNode*)node object:(CGEchoObject*)obj message:(CGEchoMessage*)msg
{
  if (!cObject)
    return NO;

  uecho_message_setdestinationobjectcode(msg.cObject, obj.code);
  return uecho_controller_sendmessage(cObject, node.cObject, msg.cObject);
}

- (CGEchoMessage*)postMessage:(CGEchoNode*)node object:(CGEchoObject*)obj message:(CGEchoMessage*)msg
{
  if (!cObject)
    return nil;

  uEchoMessage* res_msg = uecho_message_new();
  if (!res_msg)
    return nil;

  uecho_message_setdestinationobjectcode(msg.cObject, obj.code);
  if (!uecho_controller_postmessage(cObject, node.cObject, msg.cObject, res_msg)) {
    uecho_message_delete(res_msg);
    return nil;
  }

  return [[CGEchoMessage alloc] initWithCObject:res_msg];
}

@end

static void CGEchoControllerObserver(uEchoController* cObject, uEchoNode* cNode, uEchoNodeStatus status, uEchoMessage* cMsg)
{
  CGEchoController* ctrl = (__bridge CGEchoController*)(uecho_controller_getuserdata(cObject));
  if (ctrl == nil)
    return;

  @autoreleasepool {
    CGEchoNode* node = [[CGEchoNode alloc] initWithCObject:cNode];
    CGEchoMessage* msg = [[CGEchoMessage alloc] initWithCObject:cMsg];

    switch (status) {
    case uEchoNodeStatusAdded: {
      if ([[ctrl observer] respondsToSelector:@selector(controller:nodeAdded:message:)])
        [[ctrl observer] controller:ctrl nodeAdded:node message:msg];
    } break;
    case uEchoNodeStatusUpdated: {
      if ([[ctrl observer] respondsToSelector:@selector(controller:nodeUpdated:message:)])
        [[ctrl observer] controller:ctrl nodeUpdated:node message:msg];
    } break;
    case uEchoNodeStatusAnnounced: {
      if ([[ctrl observer] respondsToSelector:@selector(controller:nodeAnnounced:message:)])
        [[ctrl observer] controller:ctrl nodeAnnounced:node message:msg];
    } break;
    case uEchoNodeStatusResponded: {
      if ([[ctrl observer] respondsToSelector:@selector(controller:nodeResponsed:message:)])
        [[ctrl observer] controller:ctrl nodeResponsed:node message:msg];
    } break;
    }
  }
}
