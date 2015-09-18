/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) Satoshi Konno 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import uEchoC

public class uEchoProperty {
  var cobj: UnsafeMutablePointer<Void>? = nil  

  init(cobj: UnsafeMutablePointer<Void>?) {
    self.cobj = cobj
  }

  public func getCode() -> Int {
    if self.cobj != nil {
      return 0
    }
    return Int(uecho_property_getcode(self.cobj!))
  }
}
