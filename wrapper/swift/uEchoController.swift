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
}

