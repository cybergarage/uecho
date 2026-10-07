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

#import <uecho/std/_standard.h>

@interface CGEchoStandardProperty ()
- (instancetype)initWithDefinition:(const uEchoStdProperty*)definition;
@end

@implementation CGEchoStandardProperty
- (instancetype)initWithDefinition:(const uEchoStdProperty*)definition
{
  if ((self = [super init]) == nil)
    return nil;
  _epc = definition->code;
  _name = [[NSString alloc] initWithUTF8String:definition->name];
  _standardAttributes = definition->attr;
#if defined(UECHO_DATABASE_VALUE_METADATA) && UECHO_DATABASE_VALUE_METADATA
  if (definition->valueSchema) {
    NSData* data = [[NSString stringWithUTF8String:definition->valueSchema] dataUsingEncoding:NSUTF8StringEncoding];
    id schema = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    if ([schema isKindOfClass:NSDictionary.class])
      _valueSchema = schema;
  }
#endif
  return self;
}
- (BOOL)hasValueMetadata
{
  return self.valueSchema != nil;
}
- (id)copyWithZone:(NSZone*)zone
{
  return self;
}
@end

@interface CGEchoStandardClass ()
- (instancetype)initWithDefinition:(const uEchoStdObject*)definition;
@end

@implementation CGEchoStandardClass
+ (instancetype)classWithGroupCode:(uint8_t)groupCode classCode:(uint8_t)classCode
{
  const uEchoStdObject* definition = uecho_std_getobject(groupCode, classCode);
  return definition ? [[self alloc] initWithDefinition:definition] : nil;
}
- (instancetype)initWithDefinition:(const uEchoStdObject*)definition
{
  if ((self = [super init]) == nil)
    return nil;
  _groupCode = definition->grpCode;
  _classCode = definition->clsCode;
  _name = [[NSString alloc] initWithUTF8String:definition->name];
  NSMutableDictionary* properties = [NSMutableDictionary dictionary];
  // MRA guidebook section 3.3: device definitions override superclass metadata.
  // Node profiles have their own definitions. This does not alter C device defaults.
  const uEchoStdObject* superclass = definition->grpCode == 0x0E ? NULL : uecho_std_getobject(0, 0);
  const uEchoStdObject* sources[] = { definition, superclass };
  for (NSUInteger source = 0; source < 2; source++) {
    const uEchoStdObject* object = sources[source];
    if (!object)
      continue;
    for (size_t index = 0; index < object->propCnt; index++) {
      NSNumber* key = @(object->props[index].code);
      if (!properties[key])
        properties[key] = [[CGEchoStandardProperty alloc] initWithDefinition:&object->props[index]];
    }
  }
  _properties = [properties copy];
  return self;
}
- (NSDictionary*)readableDefinitionsForMap:(CGEchoPropertyMap*)map
{
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  if (map.mapEPC != CGEchoEPCGetPropertyMap || map.state != CGEchoCapabilityStateAvailable)
    return [result copy];
  for (NSNumber* key in self.properties) {
    CGEchoStandardProperty* property = self.properties[key];
    if ((property.standardAttributes & (uEchoPropertyAttrRead | uEchoPropertyAttrReadRequired)) && [map containsProperty:property.epc])
      result[key] = property;
  }
  return [result copy];
}
+ (NSString*)sourceDescription
{
  return [NSString stringWithFormat:@"Compiled uEcho C tables generated from %s; MRA is reference data, not certification. See source reconciliation and schema limitations in doc/objc.md.", uecho_std_source_version];
}
+ (NSDictionary*)valueDefinitions
{
  static NSDictionary* definitions;
  static dispatch_once_t once;
  dispatch_once(&once, ^{
      if (uecho_std_value_definitions) {
        NSData* data = [[NSString stringWithUTF8String:uecho_std_value_definitions] dataUsingEncoding:NSUTF8StringEncoding];
        id value = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
        if ([value isKindOfClass:NSDictionary.class])
          definitions = value;
      }
  });
  return definitions;
}
+ (BOOL)hasFullDatabase
{
  return uecho_std_objectcount > 2;
}
- (id)copyWithZone:(NSZone*)zone
{
  return self;
}
@end
