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

#if !defined(_UECHO_CONTROLLER_H_)
typedef void uEchoController;
#endif

@interface CGEchoController : NSObject {
}
@property (assign, readonly) uEchoController* cObject;
- (id)init;
- (NSArray*)nodes;
- (BOOL)start;
- (BOOL)stop;
- (BOOL)search;
@end
