/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoObject.h"
#import "CGEchoProperty.h"

#include <uecho/object.h>

@implementation CGEchoObject {
  uEchoObject* cObject;
}

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

- (NSArray*)properties;
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* props = [NSMutableArray array];
  for (uEchoProperty* cProp = uecho_object_getproperties(cObject); cProp; cProp = uecho_property_next(cProp)) {
    CGEchoProperty* prop = [[CGEchoProperty alloc] initWithCObject:cProp];
    [props addObject:prop];
  }
  return props;
}

- (NSString*)name
{
  if (!cObject)
    return nil;
  const char* name = uecho_object_getname(cObject);
  if (!name)
    return nil;
  return [NSString stringWithUTF8String:name];
}

- (int)code
{
  if (!cObject)
    return 0;
  return uecho_object_getcode(cObject);
}

- (Byte)group
{
  if (!cObject)
    return 0;
  return uecho_object_getgroupcode(cObject);
}

- (Byte)class
{
  if (!cObject)
    return 0;
  return uecho_object_getclasscode(cObject);
}

@end
