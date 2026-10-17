#import <CGEcho/CGEcho.h>
#import <XCTest/XCTest.h>

@interface CGEchoValueTests : XCTestCase
@end
@implementation CGEchoValueTests
- (void)testScalarConversion
{
  for (uint8_t code = 0x40; code <= 0x45; code++) {
    NSData* data = [NSData dataWithBytes:&code length:1];
    CGEchoValue* value = [CGEchoValue decodeData:data groupCode:1 classCode:0x30 epc:0xB0 error:nil];
    XCTAssertEqual(value.status, CGEchoValueState);
    XCTAssertEqualObjects(value.rawData, data);
    if (code == 0x40)
      XCTAssertEqualObjects(value.name, @"other");
    XCTAssertEqualObjects([CGEchoValue encodeInteger:value.code kind:value.status groupCode:1 classCode:0x30 epc:0xB0 error:nil], data);
  }
  uint8_t sentinel = 0xFD;
  CGEchoValue* value = [CGEchoValue decodeData:[NSData dataWithBytes:&sentinel length:1] groupCode:1 classCode:0x30 epc:0xB3 error:nil];
  XCTAssertEqual(value.status, CGEchoValueSpecial);
  XCTAssertEqualObjects(value.name, @"undefined");
  XCTAssertEqualObjects(value.unit, @"Celsius");
  XCTAssertTrue(value.readOnly);
  value = [CGEchoValue decodeData:[NSData data] groupCode:1 classCode:0x30 epc:0xB3 error:nil];
  XCTAssertEqual(value.status, CGEchoValueInvalid);
  value = [CGEchoValue decodeData:[NSData data] groupCode:0xFF classCode:0xFF epc:0xFF error:nil];
  XCTAssertEqual(value.status, CGEchoValueUnsupported);
  NSError* error;
  XCTAssertNil([CGEchoValue encodeInteger:51 kind:CGEchoValueNumber groupCode:1 classCode:0x30 epc:0xB3 error:&error]);
  XCTAssertEqual(error.code, CGEchoErrorInvalidArgument);
}
@end
