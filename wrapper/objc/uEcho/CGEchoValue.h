/******************************************************************
 * uEcho for ObjC
 * Copyright (C) The uecho Authors 2026
 * This is licensed under BSD-style license, see file COPYING.
 ******************************************************************/
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/* Values mirror the C result. Unsupported and invalid results retain rawData. */
typedef NS_ENUM(NSInteger, CGEchoValueStatus) {
  CGEchoValueUnsupported,
  CGEchoValueInvalid,
  CGEchoValueUnknown,
  CGEchoValueNumber,
  CGEchoValueState,
  CGEchoValueSpecial
};

@interface CGEchoValue : NSObject
- (instancetype)init NS_UNAVAILABLE;
@property (nonatomic, readonly) CGEchoValueStatus status;
@property (nonatomic, readonly, copy) NSData* rawData;
@property (nonatomic, readonly) int64_t integer;
@property (nonatomic, readonly) double number;
@property (nonatomic, readonly) uint32_t code;
@property (nonatomic, readonly) BOOL readOnly;
@property (nonatomic, readonly, copy, nullable) NSString* name;
@property (nonatomic, readonly, copy, nullable) NSString* unit;
/* Pure local conversion; never sends a request or changes access policy. */
+ (nullable instancetype)decodeData:(NSData*)data groupCode:(uint8_t)groupCode classCode:(uint8_t)classCode epc:(uint8_t)epc error:(NSError* _Nullable* _Nullable)error;
+ (nullable NSData*)encodeInteger:(int64_t)integer kind:(CGEchoValueStatus)kind groupCode:(uint8_t)groupCode classCode:(uint8_t)classCode epc:(uint8_t)epc error:(NSError* _Nullable* _Nullable)error;
@end
NS_ASSUME_NONNULL_END
