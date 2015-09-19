/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) Satoshi Konno 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import uEcho

public class uEchoLightController : uEchoController {
  
  override init() {
    super.init()
  }

  deinit {
  }
  
  var objects : [uEchoObject] {
    var objs = [uEchoObject]()
    for node in nodes {
      for obj in node.objects {
        if (obj.classGroupCode == 0x02) && (obj.classCode == 0x91) {
          objs.append(obj)
        }
      }
    }
    return objs
  }
  
  func setOperationStatus(obj : uEchoObject, status : Bool) -> Bool {
    let prop = uEchoProperty()
    prop.code = 0x80
    var data = [UInt8]()
    data.append(status ? 0x30 : 0x31)
    
    var props = [uEchoProperty]()
    props.append(prop)
    
    let msg = uEchoMessage()
    msg.ESV = 0x60
    msg.properties = props
    
    return super.sendMessage(obj, msg:msg)
  }
}
