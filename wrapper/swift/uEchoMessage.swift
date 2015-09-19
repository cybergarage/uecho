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

public class uEchoMessage {
  
  var cobj: UnsafeMutablePointer<Void>! = nil

  public init() {
    self.cobj = uecho_message_new();
  }

  deinit {
    uecho_message_delete(self.cobj)
  }
  
  public var ESV : UInt8 {
    get {
      return uecho_message_getesv(self.cobj);
    }
    set {
      uecho_message_setesv(self.cobj, newValue);
    }
  }

  public var properties : [uEchoProperty] {
    get {
      var props = [uEchoProperty]()
      let opc = uecho_message_getopc(self.cobj)
      for var n = 0; n<Int(opc); n++ {
        let cprop = uecho_message_getproperty(self.cobj, n);
        if cprop == nil {
          continue
        }
        let prop = uEchoProperty(cobj: cprop)
        props.append(prop)
      }
      return props
    }
    set {
      uecho_message_clearproperties(self.cobj)
      for prop in newValue {
        let cprop = uecho_property_copy(prop.cobj);
        if cprop == nil {
          continue
        }
        uecho_message_addproperty(self.cobj, cprop)
      }
    }
  }
}
