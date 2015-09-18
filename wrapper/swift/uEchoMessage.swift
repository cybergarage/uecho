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

public class uEchoMessage {
  
  var cobj: UnsafeMutablePointer<Void>? = nil

  public init() {
    self.cobj = uecho_message_new();
  }

  init(cobj: UnsafeMutablePointer<Void>?) {
    self.cobj = cobj
  }
}
