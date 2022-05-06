/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoProperty.h"

#include <uecho/property.h>

@implementation CGEchoProperty

@synthesize cObject;

- (id)initWithCObject:(uEchoProperty*)cobj
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
