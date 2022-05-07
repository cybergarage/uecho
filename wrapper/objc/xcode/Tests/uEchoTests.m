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
}

@end
