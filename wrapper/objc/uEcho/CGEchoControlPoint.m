/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoControlPoint.h"

@implementation CGUpnpControlPoint

- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  return self;
}

- (void)dealloc
{
}

- (BOOL)start
{
  return TRUE;
}

- (BOOL)stop
{
  return TRUE;
}

- (void)search
{
}

@end
