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

public class uEchoNode {

  var cobj: UnsafeMutablePointer<Void>? = nil

  public init() {
  }

  public func getObjects() -> [uEchoObject] {
    var objs = [uEchoObject]()
    if self.cobj != nil {
      return objs
    }
    for var co = uecho_node_getobjects(self.cobj!); co != nil; co = uecho_object_next(co) {
      let sobj = uEchoObject()
      sobj.cobj = co
      objs.append(sobj)
    }
    return objs
  }
}

