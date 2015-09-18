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

public class uEchoController {
  
  var cobj: UnsafeMutablePointer<Void>! = nil
  
  public init() {
    self.cobj = uecho_controller_new()
  }

  deinit {
    uecho_controller_delete(self.cobj)
  }
  
  public func search() {
    uecho_controller_searchallobjects(self.cobj)
  }

  public var nodes : [uEchoNode] {
    var nodes = [uEchoNode]()
    for var cnode = uecho_controller_getnodes(self.cobj); cnode != nil; cnode = uecho_node_next(cnode) {
      let node = uEchoNode(cobj: cnode)
      nodes.append(node)
    }
    return nodes
  }
}
