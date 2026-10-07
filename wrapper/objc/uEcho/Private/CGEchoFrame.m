/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoFrame.h"

static const uint8_t CGEchoEHD1 = 0x10;
static const uint8_t CGEchoEHD2Format1 = 0x81;
static const NSUInteger CGEchoHeaderLength = 12;

@implementation CGEchoFrameProperty

+ (instancetype)propertyWithEPC:(CGEchoEPC)epc data:(NSData*)data
{
  CGEchoFrameProperty* prop = [[self alloc] init];
  prop->_EPC = epc;
  prop->_data = [data copy] ?: [NSData data];
  return prop;
}

@end

@implementation CGEchoFrame

static void CGEchoPutEOJ(uint8_t* p, CGEchoEOJ eoj)
{
  p[0] = (eoj >> 16) & 0xFF;
  p[1] = (eoj >> 8) & 0xFF;
  p[2] = eoj & 0xFF;
}

static CGEchoEOJ CGEchoGetEOJ(const uint8_t* p)
{
  return ((CGEchoEOJ)p[0] << 16) | ((CGEchoEOJ)p[1] << 8) | p[2];
}

+ (instancetype)frameWithTID:(CGEchoTID)tid SEOJ:(CGEchoEOJ)seoj DEOJ:(CGEchoEOJ)deoj ESV:(CGEchoESV)esv properties:(NSArray<CGEchoFrameProperty*>*)properties
{
  NSMutableData* data = [NSMutableData dataWithLength:CGEchoHeaderLength];
  uint8_t* h = data.mutableBytes;
  h[0] = CGEchoEHD1;
  h[1] = CGEchoEHD2Format1;
  h[2] = (tid >> 8) & 0xFF;
  h[3] = tid & 0xFF;
  CGEchoPutEOJ(h + 4, seoj);
  CGEchoPutEOJ(h + 7, deoj);
  h[10] = esv;
  h[11] = (uint8_t)MIN(properties.count, (NSUInteger)0xFF);
  for (NSUInteger n = 0; n < h[11]; n++) {
    CGEchoFrameProperty* prop = properties[n];
    uint8_t epcPdc[2] = { prop.EPC, (uint8_t)MIN(prop.data.length, (NSUInteger)0xFF) };
    [data appendBytes:epcPdc length:2];
    [data appendBytes:prop.data.bytes length:epcPdc[1]];
    h = data.mutableBytes; // appendBytes may reallocate
  }

  CGEchoFrame* frame = [[self alloc] init];
  frame->_TID = tid;
  frame->_SEOJ = seoj & 0xFFFFFF;
  frame->_DEOJ = deoj & 0xFFFFFF;
  frame->_ESV = esv;
  frame->_properties = [properties copy];
  frame->_rawData = [data copy];
  return frame;
}

+ (instancetype)frameWithData:(NSData*)data address:(NSString*)address
{
  const uint8_t* b = data.bytes;
  NSUInteger length = data.length;
  if (length < CGEchoHeaderLength || b[0] != CGEchoEHD1 || b[1] != CGEchoEHD2Format1)
    return nil;

  CGEchoESV esv = b[10];
  NSMutableArray* props = [NSMutableArray array];
  BOOL isSetGetFamily = (esv == CGEchoESVSetGet || esv == CGEchoESVSetGetResponse || esv == CGEchoESVSetGetSNA);
  if (!isSetGetFamily) {
    NSUInteger opc = b[11];
    NSUInteger offset = CGEchoHeaderLength;
    for (NSUInteger n = 0; n < opc; n++) {
      if (length < offset + 2)
        return nil;
      CGEchoEPC epc = b[offset];
      NSUInteger pdc = b[offset + 1];
      offset += 2;
      if (length < offset + pdc)
        return nil;
      [props addObject:[CGEchoFrameProperty propertyWithEPC:epc data:[data subdataWithRange:NSMakeRange(offset, pdc)]]];
      offset += pdc;
    }
    if (offset != length)
      return nil;
  }

  CGEchoFrame* frame = [[self alloc] init];
  frame->_address = [address copy];
  frame->_TID = (CGEchoTID)((b[2] << 8) | b[3]);
  frame->_SEOJ = CGEchoGetEOJ(b + 4);
  frame->_DEOJ = CGEchoGetEOJ(b + 7);
  frame->_ESV = esv;
  frame->_properties = [props copy];
  frame->_rawData = [data copy];
  return frame;
}

- (BOOL)isResponse
{
  uint8_t esv = self.ESV;
  if ((esv & 0xF0) == 0x50)
    return YES;
  return (esv & 0xF0) == 0x70 && esv != CGEchoESVInf && esv != CGEchoESVInfC;
}

- (NSString*)description
{
  return [NSString stringWithFormat:@"<CGEchoFrame %@ TID=%04X %06X->%06X ESV=%02X OPC=%lu>", self.address ?: @"-", self.TID, self.SEOJ, self.DEOJ, self.ESV, (unsigned long)self.properties.count];
}

@end
