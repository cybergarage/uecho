//
//  uEchoTests.m
//  uEchoTests
//
//  Created by The uecho Authors on 2022/05/02.
//

#import <XCTest/XCTest.h>
#import <CGEcho/CGEchoController.h>

@interface uEchoTests : XCTestCase

@end

@implementation uEchoTests

- (void)setUp {
}

- (void)tearDown {
}

- (void)testController {
  CGEchoController *ctrl = [[CGEchoController alloc] init];
  XCTAssertTrue([ctrl start]);
  XCTAssertTrue([ctrl stop]);
}

@end
