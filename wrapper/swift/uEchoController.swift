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

public typealias uEchoControllerCListener = @convention(c) (UnsafeMutablePointer<Void>, UnsafeMutablePointer<Void>) -> Void
public typealias uEchoControllerListener = (uEchoMessage) -> Void

func uEchoFrameworkControllerMessageListener(ctrl : UnsafeMutablePointer<Void>, msg : UnsafeMutablePointer<Void>) -> Void
{
}

public class uEchoController {
  
  public var cobj: UnsafeMutablePointer<Void>! = nil
  public var listner: (uEchoControllerListener)! = nil
  
  public init() {
    self.cobj = uecho_controller_new()
    //uecho_controller_setuserdata(self.cobj, UnsafeMutablePointer(self))
    uecho_controller_setmessagelistener(self.cobj, uEchoFrameworkControllerMessageListener)
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

  public func postMessage(obj:uEchoObject, msg:uEchoMessage) -> (result:Bool, message:uEchoMessage) {
    let resMsg = uEchoMessage()
    let postResult = uecho_controller_postmessage(self.cobj, obj.cobj, msg.cobj, resMsg.cobj);
    return (postResult, msg)
  }
}
