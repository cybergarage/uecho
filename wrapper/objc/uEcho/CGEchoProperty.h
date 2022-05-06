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

#if !defined(_UECHO_PROPERTY_INTERNAL_H_)
typedef void uEchoProperty;
#endif

@interface CGEchoProperty : NSObject {
}
@property (assign) uEchoProperty* cObject;
- (id)initWithCObject:(uEchoProperty*)cobj;
@end
