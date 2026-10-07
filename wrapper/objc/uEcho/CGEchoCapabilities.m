/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoCapabilities.h"
#import "CGEchoError.h"
#import "Private/CGEchoInternal.h"

@implementation CGEchoPropertyMap

- (instancetype)initWithMapEPC:(CGEchoEPC)mapEPC state:(CGEchoCapabilityState)state properties:(NSIndexSet*)properties rawData:(NSData*)rawData error:(NSError*)error
{
  if ((self = [super init]) == nil)
    return nil;
  _mapEPC = mapEPC;
  _state = state;
  _properties = [properties copy];
  _rawData = [rawData copy];
  _error = error;
  return self;
}

+ (instancetype)unknownMapWithEPC:(CGEchoEPC)mapEPC
{
  return [[self alloc] initWithMapEPC:mapEPC state:CGEchoCapabilityStateUnknown properties:nil rawData:nil error:nil];
}

+ (instancetype)mapWithEPC:(CGEchoEPC)mapEPC rawData:(NSData*)rawData
{
  NSError* error = nil;
  NSIndexSet* props = [self decodePropertyMapData:rawData error:&error];
  if (!props)
    return [[self alloc] initWithMapEPC:mapEPC state:CGEchoCapabilityStateFailed properties:nil rawData:rawData error:error];
  return [[self alloc] initWithMapEPC:mapEPC state:CGEchoCapabilityStateAvailable properties:props rawData:rawData error:nil];
}

+ (instancetype)failedMapWithEPC:(CGEchoEPC)mapEPC error:(NSError*)error rawData:(NSData*)rawData
{
  return [[self alloc] initWithMapEPC:mapEPC state:CGEchoCapabilityStateFailed properties:nil rawData:rawData error:error];
}

- (id)copyWithZone:(NSZone*)zone
{
  return self;
}

- (BOOL)containsProperty:(CGEchoEPC)epc
{
  return self.state == CGEchoCapabilityStateAvailable && [self.properties containsIndex:epc];
}

static NSError* CGEchoMapError(NSString* reason)
{
  return CGEchoMakeError(CGEchoErrorMalformedResponse, [NSString stringWithFormat:@"Invalid property map: %@", reason], nil);
}

+ (NSIndexSet*)decodePropertyMapData:(NSData*)data error:(NSError**)error
{
  NSError* failure = nil;
  NSMutableIndexSet* props = [NSMutableIndexSet indexSet];
  const uint8_t* bytes = data.bytes;
  NSUInteger length = data.length;

  if (length < 1) {
    failure = CGEchoMapError(@"empty EDT");
  }
  else {
    NSUInteger count = bytes[0];
    if (128 < count) {
      failure = CGEchoMapError(@"declared count exceeds 128");
    }
    else if (count < 16) {
      if (length != 1 + count) {
        failure = CGEchoMapError(@"format 1 length does not match the declared count");
      }
      else {
        for (NSUInteger n = 1; n < length; n++) {
          uint8_t epc = bytes[n];
          if (epc < 0x80) {
            failure = CGEchoMapError(@"EPC below 0x80");
            break;
          }
          if ([props containsIndex:epc]) {
            failure = CGEchoMapError(@"duplicate EPC");
            break;
          }
          [props addIndex:epc];
        }
      }
    }
    else {
      if (length != 17) {
        failure = CGEchoMapError(@"format 2 must be 17 bytes");
      }
      else {
        // Byte n (1..16) bit b (0..7) represents EPC 0x80 + (n - 1) + 0x10 * b.
        for (NSUInteger n = 1; n <= 16; n++) {
          for (NSUInteger b = 0; b < 8; b++) {
            if (bytes[n] & (1u << b))
              [props addIndex:0x80 + (n - 1) + 0x10 * b];
          }
        }
        if (props.count != count)
          failure = CGEchoMapError(@"format 2 bitmap does not match the declared count");
      }
    }
  }

  if (failure) {
    if (error)
      *error = failure;
    return nil;
  }
  return props;
}

- (NSString*)description
{
  static NSString* const states[] = { @"unknown", @"available", @"failed" };
  return [NSString stringWithFormat:@"<CGEchoPropertyMap 0x%02X %@ %lu>", self.mapEPC, states[self.state], (unsigned long)self.properties.count];
}

@end

@implementation CGEchoCapabilities

- (instancetype)initWithReadable:(CGEchoPropertyMap*)readable writable:(CGEchoPropertyMap*)writable notifiable:(CGEchoPropertyMap*)notifiable
{
  if ((self = [super init]) == nil)
    return nil;
  _readable = readable;
  _writable = writable;
  _notifiable = notifiable;
  return self;
}

+ (instancetype)unknownCapabilities
{
  return [[self alloc] initWithReadable:[CGEchoPropertyMap unknownMapWithEPC:CGEchoEPCGetPropertyMap]
                               writable:[CGEchoPropertyMap unknownMapWithEPC:CGEchoEPCSetPropertyMap]
                             notifiable:[CGEchoPropertyMap unknownMapWithEPC:CGEchoEPCAnnouncePropertyMap]];
}

- (id)copyWithZone:(NSZone*)zone
{
  return self;
}

@end
