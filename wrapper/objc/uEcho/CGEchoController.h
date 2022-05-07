/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <Foundation/NSArray.h>
#import <Foundation/NSString.h>

@interface CGEchoController : NSObject {
}
- (id)init;
- (NSArray*)nodes;
- (BOOL)start;
- (BOOL)stop;
- (BOOL)search;
@end
