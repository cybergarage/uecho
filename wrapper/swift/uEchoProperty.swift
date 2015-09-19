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

public class uEchoProperty {
  
  private var isOwner : Bool = false
  public var cobj : UnsafeMutablePointer<Void>! = nil
  
  public init(cobj: UnsafeMutablePointer<Void>) {
    self.cobj = cobj
    self.isOwner = false
  }

  public init() {
    self.cobj = uecho_property_new()
    self.isOwner = true
  }

  deinit {
    if isOwner {
      uecho_property_delete(self.cobj)
    }
  }
  
  public var code : UInt8 {
    get {
      return uecho_property_getcode(self.cobj)
    }
    set {
      uecho_property_setcode(self.cobj, newValue)
    }
  }
  
  public var data : [UInt8] {
    get {
      var bytes = [UInt8]()
      let propBytes = uecho_property_getdata(self.cobj)
      let propByteSize = uecho_property_getdatasize(self.cobj)
      for var n=0; n<Int(propByteSize); n++ {
        bytes.append(UInt8(propBytes[n]))
      }
      return bytes
    }
    set {
      uecho_property_cleardata(self.cobj)
      for b in newValue {
        uecho_property_addbytedata(self.cobj, b)
      }
    }
  }
}
