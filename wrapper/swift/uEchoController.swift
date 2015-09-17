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
  
  var cobj: UnsafeMutablePointer<Void>? = nil
  
  init() {
    self.cobj = uecho_controller_new()
  }

  deinit {
    if self.cobj != nil {
      uecho_controller_delete(self.cobj!)
    }
  }
  
  public func search() {
    if self.cobj != nil {
      uecho_controller_searchallobjects(self.cobj!)
    }
  }

  public func getNodes() -> [uEchoNode] {
    var nodes = [uEchoNode]()
    if self.cobj != nil {
      return nodes
    }
    for var cnode = uecho_controller_getnodes(self.cobj!); cnode != nil; cnode = uecho_node_next(cnode) {
      let node = uEchoNode()
      node.cobj = cnode
      nodes.append(node)
    }
    return nodes
  }
}
