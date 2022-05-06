/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/object.h>

#import "CGEchoObject.h"

@implementation CGEchoObject

@synthesize cObject;

- (id)initWithCObject:(uEchoObject*)cobj
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = cobj;
  return self;
}

- (void)dealloc
{
}

@end
