/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <CGEcho/CGEcho.h>
#import <XCTest/XCTest.h>

#import "CGEchoFrame.h"

@interface CGEchoPropertyMapTests : XCTestCase
@end

static NSData* Bytes(NSArray<NSNumber*>* values)
{
  NSMutableData* data = [NSMutableData data];
  for (NSNumber* v in values) {
    uint8_t b = v.unsignedCharValue;
    [data appendBytes:&b length:1];
  }
  return data;
}

static NSData* Format2(NSIndexSet* epcs)
{
  NSMutableData* data = [NSMutableData dataWithLength:17];
  uint8_t* b = data.mutableBytes;
  b[0] = (uint8_t)epcs.count;
  for (NSUInteger epc = epcs.firstIndex; epc != NSNotFound; epc = [epcs indexGreaterThanIndex:epc]) {
    NSUInteger offset = epc - 0x80;
    b[1 + (offset & 0x0F)] |= (uint8_t)(1u << (offset >> 4));
  }
  return data;
}

@implementation CGEchoPropertyMapTests

- (void)testFormat1EmptyIsValid
{
  NSError* error = nil;
  NSIndexSet* set = [CGEchoPropertyMap decodePropertyMapData:Bytes(@[ @0 ]) error:&error];
  XCTAssertNotNil(set);
  XCTAssertEqual(set.count, 0u);
  XCTAssertNil(error);
}

- (void)testFormat1List
{
  NSIndexSet* set = [CGEchoPropertyMap decodePropertyMapData:Bytes(@[ @3, @0x80, @0x81, @0xFF ]) error:NULL];
  XCTAssertEqual(set.count, 3u);
  XCTAssertTrue([set containsIndex:0xFF]);
}

- (void)testFormat1FifteenEntries
{
  NSMutableArray* values = [NSMutableArray arrayWithObject:@15];
  for (int n = 0; n < 15; n++)
    [values addObject:@(0x80 + n)];
  XCTAssertEqual([CGEchoPropertyMap decodePropertyMapData:Bytes(values) error:NULL].count, 15u);
}

- (void)testFormat1Rejected
{
  NSError* error = nil;
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:Bytes(@[ @2, @0x80 ]) error:&error], @"count mismatch");
  XCTAssertEqual(error.code, CGEchoErrorMalformedResponse);
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:Bytes(@[ @1, @0x7F ]) error:NULL], @"EPC below 0x80");
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:Bytes(@[ @2, @0x80, @0x80 ]) error:NULL], @"duplicate");
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:[NSData data] error:NULL], @"empty");
}

- (void)testFormat2SixteenEntries
{
  NSIndexSet* epcs = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0x80, 16)];
  XCTAssertEqualObjects([CGEchoPropertyMap decodePropertyMapData:Format2(epcs) error:NULL], epcs);
}

- (void)testFormat2HighEPCs
{
  // 0xC0-0xFF are carried by the upper four bits; the C helper masks them off.
  NSMutableIndexSet* epcs = [NSMutableIndexSet indexSetWithIndexesInRange:NSMakeRange(0x80, 14)];
  [epcs addIndex:0xC0];
  [epcs addIndex:0xFF];
  NSIndexSet* decoded = [CGEchoPropertyMap decodePropertyMapData:Format2(epcs) error:NULL];
  XCTAssertEqualObjects(decoded, epcs);
  XCTAssertTrue([decoded containsIndex:0xC0]);
  XCTAssertTrue([decoded containsIndex:0xFF]);
}

- (void)testFormat2AllEntries
{
  NSIndexSet* epcs = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0x80, 128)];
  XCTAssertEqual([CGEchoPropertyMap decodePropertyMapData:Format2(epcs) error:NULL].count, 128u);
}

- (void)testFormat2Rejected
{
  NSIndexSet* epcs = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0x80, 16)];
  NSMutableData* shortMap = [Format2(epcs) mutableCopy];
  shortMap.length = 16;
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:shortMap error:NULL], @"length");
  NSMutableData* wrongCount = [Format2(epcs) mutableCopy];
  ((uint8_t*)wrongCount.mutableBytes)[0] = 17;
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:wrongCount error:NULL], @"count mismatch");
  NSMutableData* tooMany = [Format2(epcs) mutableCopy];
  ((uint8_t*)tooMany.mutableBytes)[0] = 200;
  XCTAssertNil([CGEchoPropertyMap decodePropertyMapData:tooMany error:NULL], @"count above 128");
}

#pragma mark Frames

- (void)testFrameRoundTrip
{
  uint8_t on = 0x30;
  CGEchoFrame* frame = [CGEchoFrame frameWithTID:0x1234 SEOJ:0x05FF01 DEOJ:0x013001 ESV:CGEchoESVSetC properties:@[ [CGEchoFrameProperty propertyWithEPC:0x80 data:[NSData dataWithBytes:&on length:1]] ]];
  const uint8_t expected[] = { 0x10, 0x81, 0x12, 0x34, 0x05, 0xFF, 0x01, 0x01, 0x30, 0x01, 0x61, 0x01, 0x80, 0x01, 0x30 };
  XCTAssertEqualObjects(frame.rawData, [NSData dataWithBytes:expected length:sizeof(expected)]);

  CGEchoFrame* parsed = [CGEchoFrame frameWithData:frame.rawData address:@"192.0.2.1"];
  XCTAssertEqual(parsed.TID, 0x1234);
  XCTAssertEqual(parsed.SEOJ, 0x05FF01u);
  XCTAssertEqual(parsed.DEOJ, 0x013001u);
  XCTAssertEqual(parsed.ESV, CGEchoESVSetC);
  XCTAssertEqual(parsed.properties.count, 1u);
  XCTAssertEqualObjects(parsed.address, @"192.0.2.1");
}

- (void)testFrameRejectsMalformed
{
  const uint8_t truncated[] = { 0x10, 0x81, 0x00, 0x01, 0x05, 0xFF, 0x01, 0x01, 0x30, 0x01, 0x72, 0x01, 0x80, 0x02, 0x30 };
  XCTAssertNil([CGEchoFrame frameWithData:[NSData dataWithBytes:truncated length:sizeof(truncated)] address:nil]);
  const uint8_t trailing[] = { 0x10, 0x81, 0x00, 0x01, 0x05, 0xFF, 0x01, 0x01, 0x30, 0x01, 0x72, 0x01, 0x80, 0x01, 0x30, 0x00 };
  XCTAssertNil([CGEchoFrame frameWithData:[NSData dataWithBytes:trailing length:sizeof(trailing)] address:nil]);
  const uint8_t format2[] = { 0x10, 0x82, 0x00, 0x01, 0x05, 0xFF, 0x01, 0x01, 0x30, 0x01, 0x72, 0x00 };
  XCTAssertNil([CGEchoFrame frameWithData:[NSData dataWithBytes:format2 length:sizeof(format2)] address:nil]);
}

@end
