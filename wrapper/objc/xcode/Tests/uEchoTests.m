//
//  uEchoTests.m
//  uEchoTests
//
//  Created by The uecho Authors on 2022/05/02.
//

#import <CGEcho/CGEcho.h>
#import <XCTest/XCTest.h>

@interface uEchoTests : XCTestCase

@end

@implementation uEchoTests

- (void)setUp
{
}

- (void)tearDown
{
}

- (void)testDatabase
{
  CGEchoDatabase* db = [[CGEchoDatabase alloc] init];
  XCTAssertNotNil(db);
}

- (void)testController
{
  CGEchoController* ctrl = [[CGEchoController alloc] init];

  XCTAssertTrue([ctrl start]);
  XCTAssertTrue([ctrl search]);

  sleep(2);

  for (CGEchoNode* node in [ctrl nodes]) {
    XCTAssertTrue(node);
    NSString* addr = [node address];
    XCTAssertNotNil(addr);
    XCTAssertGreaterThan([addr length], 0);
    for (CGEchoObject* obj in [node objects]) {
      XCTAssertNotNil(obj);
      for (CGEchoProperty* prop in [obj properties]) {
        XCTAssertNotNil(prop);
      }
    }
  }

  XCTAssertTrue([ctrl stop]);
}

- (void)testProperty
{
  CGEchoProperty* prop = [[CGEchoProperty alloc] init];
  XCTAssertNotNil(prop);
  XCTAssertEqual(prop.code, 0x00);
  XCTAssertEqual(prop.size, 0);
  XCTAssertEqual(prop.data, NULL);

  prop.code = 0xFF;
  XCTAssertEqual(prop.code, 0xFF);

  Byte data[] = { 0x01, 0x02 };
  XCTAssertTrue([prop setData:data size:2]);
  XCTAssertEqual(prop.size, 2);
  for (int n = 0; n < prop.size; n++) {
    XCTAssertEqual(prop.data[n], data[n]);
  }
}

- (void)testMessage
{
  CGEchoMessage* msg = [[CGEchoMessage alloc] init];
  XCTAssertNotNil(msg);

  msg.ESV = CGEchoESVReadRequest;
  XCTAssertEqual(msg.ESV, CGEchoESVReadRequest);

  XCTAssertTrue([msg addProperty:0x8A data:nil size:0]);
  XCTAssertEqual(msg.properties.count, 1);
  XCTAssertEqual(((CGEchoProperty*)msg.properties[0]).code, 0x8A);

  XCTAssertTrue([msg addGetProperty:0x8B data:nil size:0]);
  XCTAssertEqual(msg.getproperties.count, 1);
  XCTAssertEqual(((CGEchoProperty*)msg.getproperties[0]).code, 0x8B);

  XCTAssertTrue([msg addSetProperty:0x8C data:nil size:0]);
  XCTAssertEqual(msg.setproperties.count, 1);
  XCTAssertEqual(((CGEchoProperty*)msg.setproperties[0]).code, 0x8C);
}

@end
