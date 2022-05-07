/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) Satoshi Konno 2015
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
