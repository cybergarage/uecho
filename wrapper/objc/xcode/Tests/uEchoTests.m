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
      XCTAssertTrue(obj);
      for (CGEchoProperty* prop in [obj properties]) {
        XCTAssertTrue(prop);
      }
    }
  }

  XCTAssertTrue([ctrl stop]);
}

@end
