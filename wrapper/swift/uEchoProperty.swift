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

  init(cobj: UnsafeMutablePointer<Void>?) {
    self.cobj = cobj
  }

  var code : UInt8 {
    get {
      if self.cobj != nil {
        return 0
      }
      return uecho_property_getcode(self.cobj!)
    }
    set {
      if self.cobj != nil {
        return
      }
      uecho_property_setcode(self.cobj!, newValue)
    }
  }
}
