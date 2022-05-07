/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoMessage.h"
#import "CGEchoProperty.h"

#include <uecho/message.h>

@implementation CGEchoMessage {
  BOOL isWeakObject;
}

@synthesize cObject;
- (id)init
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = uecho_message_new();
  isWeakObject = NO;
  return self;
}

- (id)initWithCObject:(uEchoMessage*)cobj
{
  if ((self = [super init]) == nil)
    return nil;
  cObject = cobj;
  isWeakObject = YES;
  return self;
}

- (void)dealloc
{
  if (!isWeakObject) {
    uecho_message_delete(cObject);
  }
}

- (NSArray*)properties;
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* props = [NSMutableArray array];
  for (int n = 0; n < (int)uecho_message_getopc(cObject); n++) {
    uEchoProperty* cProp = uecho_message_getproperty(cObject, n);
    if (!cProp)
      continue;
    [props addObject:[[CGEchoProperty alloc] initWithCObject:cProp]];
  }
  return props;
}

- (NSArray*)setproperties;
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* props = [NSMutableArray array];
  for (int n = 0; n < (int)uecho_message_getopcset(cObject); n++) {
    uEchoProperty* cProp = uecho_message_getpropertyset(cObject, n);
    if (!cProp)
      continue;
    [props addObject:[[CGEchoProperty alloc] initWithCObject:cProp]];
  }
  return props;
}

- (NSArray*)getproperties;
{
  if (!cObject)
    return [NSArray array];
  NSMutableArray* props = [NSMutableArray array];
  for (int n = 0; n < (int)uecho_message_getopcget(cObject); n++) {
    uEchoProperty* cProp = uecho_message_getpropertyget(cObject, n);
    if (!cProp)
      continue;
    [props addObject:[[CGEchoProperty alloc] initWithCObject:cProp]];
  }
  return props;
}

- (BOOL)setESV:(CGEchoESV)esv
{
  if (!cObject)
    return NO;
  uecho_message_setesv(cObject, esv);
  return YES;
}

- (CGEchoESV)ESV;
{
  if (!cObject)
    return 0;
  return uecho_message_getesv(cObject);
}

- (BOOL)setDestination:(int)code
{
  if (!cObject)
    return NO;
  uecho_message_setdestinationobjectcode(cObject, code);
  return YES;
}

- (int)destination
{
  if (!cObject)
    return 0;
  return uecho_message_getdestinationobjectcode(cObject);
}

- (BOOL)addProperty:(Byte)code data:(Byte*)data size:(size_t)size
{
  if (!cObject)
    return 0;
  return uecho_message_setproperty(cObject, code, data, size);
}

- (BOOL)addGetProperty:(Byte)code data:(Byte*)data size:(size_t)size
{
  if (!cObject)
    return 0;
  return uecho_message_setpropertyget(cObject, code, data, size);
}

- (BOOL)addSetProperty:(Byte)code data:(Byte*)data size:(size_t)size
{
  if (!cObject)
    return 0;
  return uecho_message_setpropertyset(cObject, code, data, size);
}

@end
