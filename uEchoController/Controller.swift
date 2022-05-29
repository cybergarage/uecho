//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho

extension CGEchoController: ObservableObject {
}

public class Controller : CGEchoController
{
  override init() {
    super.init()
  }

  deinit {
    self.stop()
  }
}
