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

public enum uEchoMessageEsv : UInt8 {
  case WriteRequest = 0x60
  case WriteRequestResponseRequired = 0x61
  case ReadRequest = 0x62
  case NotificationRequest = 0x63
  case WriteReadRequest = 0x6E
  case WriteResponse = 0x71
  case ReadResponse = 0x72
  case Notification = 0x73
  case NotificationResponseRequired = 0x74
  case NotificationResponse = 0x7A
  case WriteReadResponse = 0x7E
}

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
  
  public func getProperties() -> [uEchoProperty] {
    return self.properties
  }
  
  public func getPropertyByCode(code : UInt8) -> uEchoProperty! {
    for prop in self.properties {
      if prop.code == code {
        return prop
      }
    }
    return nil
  }
}
