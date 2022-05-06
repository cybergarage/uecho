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

  for (CGEchoNode* node in [ctrl nodes]) {
    XCTAssertTrue(node);
  }

  XCTAssertTrue([ctrl stop]);
}

@end
