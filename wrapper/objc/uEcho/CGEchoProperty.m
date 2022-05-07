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

- (BOOL)setCode:(Byte)code
{
  if (!cObject)
    return NO;
  uecho_property_setcode(cObject, code);
  return YES;
}

- (BOOL)setData:(Byte*)data size_t:data_size
{
  if (!cObject)
    return NO;
  uecho_property_setdata(cObject, (const byte* )data, data_size);
  return YES;
}

- (Byte)code
{
  if (!cObject)
    return 0;
  return uecho_property_getcode(cObject);
}

- (size_t)size
{
  if (!cObject)
    return 0;
  return uecho_property_getdatasize(cObject);
}

- (Byte*)data;
{
  if (!cObject)
    return NULL;
  return uecho_property_getdata(cObject);
}

@end
