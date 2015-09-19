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

class uEchoLightObject : uEchoObject {
  
  init(obj: uEchoObject) {
    super.init(cobj: obj.cobj)
  }
}
