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

public class uEchoObject {
  
  public var cobj: UnsafeMutablePointer<Void>! = nil

  public init(cobj: UnsafeMutablePointer<Void>) {
    self.cobj = cobj
  }
  
  deinit {
  }
  
  public var code : Int {
    get {
      return Int(uecho_object_getcode(self.cobj))
    }
    set {
      uecho_object_setcode(self.cobj, Int32(newValue))
    }
  }

  public var classGroupCode : Int {
    return Int(uecho_object_getclassgroupcode(self.cobj))
  }
  
  public var classCode : Int {
    return Int(uecho_object_getclasscode(self.cobj))
  }
  
  public var instanceCode : Int {
    return Int(uecho_object_getinstancecode(self.cobj))
  }

  public var properties : [uEchoProperty] {
    var props = [uEchoProperty]()
    if self.cobj == nil {
      return props
    }
    for var cprop = uecho_object_getproperties(self.cobj!); cprop != nil; cprop = uecho_property_next(cprop) {
      let prop = uEchoProperty(cobj: cprop)
      props.append(prop)
    }
    return props
  }
}
