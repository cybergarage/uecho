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

public class uEchoNode {

  var cobj: UnsafeMutablePointer<Void>? = nil

  init(cobj: UnsafeMutablePointer<Void>?) {
    self.cobj = cobj
  }

  public func getObjects() -> [uEchoObject] {
    var objs = [uEchoObject]()
    if self.cobj != nil {
      return objs
    }
    for var cobj = uecho_node_getobjects(self.cobj!); cobj != nil; cobj = uecho_object_next(cobj) {
      let sobj = uEchoObject(cobj: cobj)
      objs.append(sobj)
    }
    return objs
  }
}

