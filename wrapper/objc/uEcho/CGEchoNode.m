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

#import "CGEchoNode.h"

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

@end
