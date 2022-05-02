/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import <Foundation/NSString.h>
#import <Foundation/NSArray.h>

@interface CGUpnpControlPoint : NSObject {
}
- (BOOL)start;
- (BOOL)stop;
- (void)search;
@end
