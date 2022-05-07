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

@implementation CGEchoController {
  uEchoController* cObject;
}
- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_controller_new();
  if (!cObject) {
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

- (CGEchoMessage*)postMessage:(CGEchoNode*)node object:(CGEchoObject*)obj message:(CGEchoMessage*)msg
{
  if (!cObject)
    return nil;

  uEchoMessage* res_msg = uecho_message_new();
  if (!res_msg)
    return nil;

  msg.destination = obj.code;
  if (!uecho_controller_postmessage(cObject, node.cObject, msg.cObject, res_msg)) {
    uecho_message_delete(res_msg);
    return nil;
  }

  return [[CGEchoMessage alloc] initWithCObject:res_msg];
}

@end
