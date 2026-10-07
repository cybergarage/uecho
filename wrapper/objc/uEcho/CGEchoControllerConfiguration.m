/******************************************************************
 *
 * uEcho for ObjC
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#import "CGEchoControllerConfiguration.h"
#import "CGEchoError.h"
#import "Private/CGEchoInternal.h"

@implementation CGEchoControllerConfiguration

+ (instancetype)defaultConfiguration
{
  return [[self alloc] init];
}

- (instancetype)init
{
  if ((self = [super init]) == nil)
    return nil;
  _callbackQueue = dispatch_get_main_queue();
  _discoveryTimeout = 3.0;
  _requestTimeout = 5.0;
  _maxConcurrentRequests = 8;
  _staleInterval = 300.0;
  _lateResponseRetention = 10.0;
  return self;
}

- (id)copyWithZone:(NSZone*)zone
{
  CGEchoControllerConfiguration* copy = [[[self class] allocWithZone:zone] init];
  copy.callbackQueue = self.callbackQueue;
  copy.discoveryTimeout = self.discoveryTimeout;
  copy.requestTimeout = self.requestTimeout;
  copy.maxConcurrentRequests = self.maxConcurrentRequests;
  copy.staleInterval = self.staleInterval;
  copy.lateResponseRetention = self.lateResponseRetention;
  return copy;
}

static BOOL CGEchoInRange(double v, double lo, double hi)
{
  return !isnan(v) && lo <= v && v <= hi;
}

- (BOOL)validate:(NSError**)error
{
  NSString* problem = nil;
  if (!self.callbackQueue)
    problem = @"callbackQueue must not be nil";
  else if (!CGEchoInRange(self.discoveryTimeout, 0.5, 60))
    problem = @"discoveryTimeout must be between 0.5 and 60 seconds";
  else if (!CGEchoInRange(self.requestTimeout, 0.1, 60))
    problem = @"requestTimeout must be between 0.1 and 60 seconds";
  else if (self.maxConcurrentRequests < 1 || 64 < self.maxConcurrentRequests)
    problem = @"maxConcurrentRequests must be between 1 and 64";
  else if (!CGEchoInRange(self.staleInterval, 1, 86400))
    problem = @"staleInterval must be between 1 and 86400 seconds";
  else if (!CGEchoInRange(self.lateResponseRetention, 0, 120))
    problem = @"lateResponseRetention must be between 0 and 120 seconds";
  if (!problem)
    return YES;
  if (error)
    *error = CGEchoMakeError(CGEchoErrorInvalidArgument, problem, @{ CGEchoErrorOperationKey : @"configure" });
  return NO;
}

@end
