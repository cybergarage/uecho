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

public class uEchoObject {
  
  var cobj: UnsafeMutablePointer<Void>? = nil
  
  func new() {
    cobj = uecho_controller_new()
  }

  func search() {
    uecho_controller_searchallobjects(cobj!)
  }
  
  func delete() {
    uecho_controller_delete(cobj!)
  }
}

