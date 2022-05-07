/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoProperty.h"

#include <uecho/property.h>

@implementation CGEchoProperty {
  uEchoProperty* cObject;
  BOOL isWeakObject;
}

- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_property_new();
  isWeakObject = NO;
  return self;
}

- (id)initWithCObject:(uEchoProperty*)cobj
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = cobj;
  isWeakObject = YES;
  return self;
}

- (void)dealloc
{
  if (isWeakObject) {
    uecho_property_delete(cObject);
  }
}

- (void)setCode:(Byte)code
{
  if (!cObject)
    return;
  uecho_property_setcode(cObject, code);
}

- (Byte)code
{
  if (!cObject)
    return 0;
  return uecho_property_getcode(cObject);
}

- (int)size
{
  if (!cObject)
    return 0;
  return uecho_property_getdatasize(cObject);
}

@end
