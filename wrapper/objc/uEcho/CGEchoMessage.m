/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoMessage.h"

@implementation CGEchoMessage

@synthesize cObject;

- (id)initWithCObject:(uEchoMessage*)cobj
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
