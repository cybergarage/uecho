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

public class uEchoObject {
  
  var cobj: UnsafeMutablePointer<Void>? = nil

  public init() {
  }
  
  public func getProperties() -> [uEchoProperty] {
    var props = [uEchoProperty]()
    if self.cobj != nil {
      return props
    }
    for var cprop = uecho_object_getproperties(self.cobj!); cprop != nil; cprop = uecho_property_next(cprop) {
      let prop = uEchoProperty()
      prop.cobj = cprop
      props.append(prop)
    }
    return props
  }
}

