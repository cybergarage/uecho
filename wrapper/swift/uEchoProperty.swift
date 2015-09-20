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

  public func getUInt8Data() -> UInt8 {
    var value : UInt8 = 0
    if !uecho_property_getbytedata(self.cobj, &value) {
      return 0
    }
    return value
  }

  public func getInt16Data() -> Int16 {
    var value : Int32 = 0
    if !uecho_property_getintegerdata(self.cobj, 2, &value) {
      return 0
    }
    return Int16(value)
  }

  public func getUInt32Data() -> Int32 {
    var value : Int32 = 0
    if !uecho_property_getintegerdata(self.cobj, 4, &value) {
      return 0
    }
    return Int32(value)
  }
}
