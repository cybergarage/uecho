/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoRemoteNode.h"
#import "Private/CGEchoInternal.h"

@implementation CGEchoRemoteObject

- (instancetype)initWithAddress:(NSString*)address EOJ:(CGEchoEOJ)eoj capabilities:(CGEchoCapabilities*)capabilities generation:(uint64_t)generation
{
  if ((self = [super init]) == nil)
    return nil;
  _address = [address copy];
  _EOJ = eoj & 0xFFFFFF;
  _capabilities = capabilities ?: [CGEchoCapabilities unknownCapabilities];
  _generation = generation;
  return self;
}

- (uint8_t)classGroupCode
{
  return (self.EOJ >> 16) & 0xFF;
}

- (uint8_t)classCode
{
  return (self.EOJ >> 8) & 0xFF;
}

- (uint8_t)instanceCode
{
  return self.EOJ & 0xFF;
}

- (id)copyWithZone:(NSZone*)zone
{
  return self;
}

- (BOOL)isEqual:(id)object
{
  if (self == object)
    return YES;
  if (![object isKindOfClass:[CGEchoRemoteObject class]])
    return NO;
  CGEchoRemoteObject* other = object;
  return other.EOJ == self.EOJ && other.generation == self.generation && [other.address isEqualToString:self.address];
}

- (NSUInteger)hash
{
  return self.address.hash ^ self.EOJ ^ (NSUInteger)(self.generation << 24);
}

- (NSString*)description
{
  return [NSString stringWithFormat:@"<CGEchoRemoteObject %@ %06X gen=%llu>", self.address, self.EOJ, self.generation];
}

@end

@implementation CGEchoRemoteNode

- (instancetype)initWithAddress:(NSString*)address objects:(NSArray<CGEchoRemoteObject*>*)objects hasObjectList:(BOOL)hasObjectList lastSeen:(NSDate*)lastSeen availability:(CGEchoNodeAvailability)availability generation:(uint64_t)generation
{
  if ((self = [super init]) == nil)
    return nil;
  _address = [address copy];
  _objects = [objects copy];
  _hasObjectList = hasObjectList;
  _lastSeen = [lastSeen copy];
  _availability = availability;
  _generation = generation;
  return self;
}

- (CGEchoRemoteObject*)objectWithEOJ:(CGEchoEOJ)eoj
{
  for (CGEchoRemoteObject* obj in self.objects) {
    if (obj.EOJ == eoj)
      return obj;
  }
  return nil;
}

- (id)copyWithZone:(NSZone*)zone
{
  return self;
}

- (NSString*)description
{
  return [NSString stringWithFormat:@"<CGEchoRemoteNode %@ objects=%lu %@>", self.address, (unsigned long)self.objects.count, self.availability == CGEchoNodeAvailabilityActive ? @"active" : @"stale"];
}

@end

@implementation CGEchoPropertyValue

- (instancetype)initWithAddress:(NSString*)address EOJ:(CGEchoEOJ)eoj EPC:(CGEchoEPC)epc data:(NSData*)data receivedAt:(NSDate*)receivedAt source:(CGEchoPropertyValueSource)source
{
  if ((self = [super init]) == nil)
    return nil;
  _address = [address copy];
  _EOJ = eoj;
  _EPC = epc;
  _data = [data copy];
  _receivedAt = [receivedAt copy];
  _source = source;
  return self;
}

- (id)copyWithZone:(NSZone*)zone
{
  return self;
}

- (NSString*)description
{
  return [NSString stringWithFormat:@"<CGEchoPropertyValue %@ %06X EPC=%02X %@>", self.address, self.EOJ, self.EPC, self.data];
}

@end
