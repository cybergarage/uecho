/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoStandard.h"

#include <uecho/std/database.h>

@implementation CGEchoDatabase {
  uEchoDatabase* cObject;
}

- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_standard_getdatabase();
  return self;
}

- (void)dealloc
{
}

- (NSString*)findManufactureName:(int)code
{
  if (!cObject)
    return nil;
  uEchoManufacture* man = uecho_database_getmanufacture(cObject, code);
  if (!man)
    return nil;
  return [NSString stringWithUTF8String:uecho_manufacture_getname(man)];
}

@end
