//
//  ContentView.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho

extension CGEchoNode: Identifiable {
  public var id: String {
    return self.address()
  }
}
