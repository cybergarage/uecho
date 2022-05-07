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

#include <uecho/message.h>

@implementation CGEchoMessage {
  uEchoMessage* cObject;
  BOOL isWeakObject;
}

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
  if (isWeakObject) {
    uecho_message_delete(cObject);
  }
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

@end
