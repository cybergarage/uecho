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

  public var cobj: UnsafeMutablePointer<Void>? = nil

  public init(cobj: UnsafeMutablePointer<Void>?) {
    self.cobj = cobj
  }

  deinit {
  }

  public var objects : [uEchoObject] {
    var objs = [uEchoObject]()
    if self.cobj == nil {
      return objs
    }
    for var cobj = uecho_node_getobjects(self.cobj!); cobj != nil; cobj = uecho_object_next(cobj) {
      let sobj = uEchoObject(cobj: cobj)
      objs.append(sobj)
    }
    return objs
  }
}

