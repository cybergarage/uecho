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

@implementation CGEchoController

@synthesize cObject;

- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_controller_new();
  if (cObject) {
    if (![self start])
      self = nil;
  }
  else
    self = nil;
  return self;
}

- (void)dealloc
{
  if (cObject) {
    uecho_controller_delete(cObject);
  }
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
  uecho_controller_searchallobjects(cObject);
  return YES;
}

@end
