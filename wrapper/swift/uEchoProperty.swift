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
  var cobj: UnsafeMutablePointer<Void>! = nil

  init(cobj: UnsafeMutablePointer<Void>) {
    self.cobj = cobj
  }

  public var code : UInt8 {
    get {
      return uecho_property_getcode(self.cobj)
    }
    set {
      uecho_property_setcode(self.cobj, newValue)
    }
  }
}
