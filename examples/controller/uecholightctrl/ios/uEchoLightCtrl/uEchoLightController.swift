/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) The uecho Authors 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import uEcho

public class uEchoLightController : uEchoController {
  
  let LightClassGroupCode = 0x02
  let LightClassCode = 0x91
  let LightOperatingStatusCode : UInt8 = 0x80
  let LightOperatingStatusOn : UInt8 = 0x30
  let LightOperatingStatusOff : UInt8 = 0x31
  
  override init() {
    super.init()
  }

  deinit {
  }
  
  var objects : [uEchoObject] {
    var objs = [uEchoObject]()
    for node in nodes {
      for obj in node.objects {
        if (obj.classGroupCode == LightClassGroupCode) && (obj.classCode == LightClassCode) {
          objs.append(obj)
        }
      }
    }
    return objs
  }
  
  func setOperationStatus(obj : uEchoObject, status : Bool) -> Bool {
    let prop = uEchoProperty()
    prop.code = LightOperatingStatusCode
    var data = [UInt8]()
    data.append(status ? LightOperatingStatusOn : LightOperatingStatusOff)
    
    let msg = uEchoMessage()
    msg.ESV = 0x60
    msg.properties = [prop]
    
    return super.sendMessage(obj, msg:msg)
  }

  func getOperationStatus(obj : uEchoObject, status : Bool) -> Bool {
    let prop = uEchoProperty()
    prop.code = LightOperatingStatusCode
    
    let msg = uEchoMessage()
    msg.ESV = 0x62
    msg.properties = [prop]
    
    let res = super.postMessage(obj, msg:msg)
    if !res.result {
        return false
    }

    let resProp = res.message.getPropertyByCode(LightOperatingStatusCode)
    if resProp == nil {
      return false
    }
    
    return (resProp.getUInt8Data() == LightOperatingStatusOn) ? true : false
  }
}
