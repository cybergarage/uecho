//
//  Node.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho

extension CGEchoRemoteNode: @retroactive Identifiable {
  public var id: String {
    return address
  }
}

extension CGEchoRemoteObject: @retroactive Identifiable {
  public var id: String {
    return "\(address)/\(eojString)"
  }

  var eojString: String {
    return String(format: "%06X", eoj)
  }
}
