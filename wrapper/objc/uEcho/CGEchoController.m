/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>

#import "CGEchoController.h"
#import "CGEchoNode.h"

@implementation CGEchoController

@synthesize cObject;

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

@end
