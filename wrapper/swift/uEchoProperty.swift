/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) The uecho Authors 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import uEchoC

public class uEchoProperty {
  var cobj: UnsafeMutablePointer<Void>? = nil  

  public func getCode() -> Int {
    if self.cobj != nil {
      return 0
    }
    return Int(uecho_property_getcode(self.cobj!))
  }
}
