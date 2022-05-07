/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoManufacture.h"

#include <uecho/manufacture.h>

@implementation CGEchoManufacture {
  uEchoManufacture* cObject;
}

- (id)initWithCObject:(uEchoManufacture*)cobj
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = cobj;
  return self;
}

- (void)dealloc
{
}

- (NSString*)name
{
  if (!cObject)
    return nil;
  const char* name = uecho_manufacture_getname(cObject);
  if (!name)
    return nil;
  return [NSString stringWithUTF8String:name];
}

- (Byte)code
{
  if (!cObject)
    return 0;
  return uecho_manufacture_getcode(cObject);
}

@end
