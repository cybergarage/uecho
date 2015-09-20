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

public class uEchoController {
  
  public var cobj: UnsafeMutablePointer<Void>! = nil
  
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

  public func sendMessage(obj:uEchoObject, msg:uEchoMessage) -> Bool {
    return uecho_controller_sendmessage(self.cobj, obj.cobj, msg.cobj);
  }

  public func postMessage(obj:uEchoObject, msg:uEchoMessage) -> (result:Bool, response:uEchoMessage) {
    let resMsg = uEchoMessage()
    let postResult = uecho_controller_postmessage(self.cobj, obj.cobj, msg.cobj, resMsg.cobj);
    return (postResult, msg)
  }
}
