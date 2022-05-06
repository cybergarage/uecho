/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>

#import "CGEchoNode.h"
#import "CGEchoObject.h"

@implementation CGEchoNode

@synthesize cObject;

- (id)initWithCObject:(uEchoNode*)cobj
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = cobj;
  return self;
}

- (void)dealloc
{
}

- (NSString*)address
{
  if (!cObject)
    return nil;
  const char* addr = uecho_node_getaddress(cObject);
  if (!addr)
    return nil;
  return [NSString stringWithUTF8String:addr];
}

- (NSArray*)objects;
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* objs = [NSMutableArray array];
  for (uEchoNode* cObj = uecho_node_getobjects(cObject); cObj; cObj = uecho_object_next(cObj)) {
    CGEchoObject* obj = [[CGEchoObject alloc] initWithCObject:cObj];
    [objs addObject:obj];
  }
  return objs;
}

@end
