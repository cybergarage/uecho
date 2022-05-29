//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho

public class Controller: CGEchoController, CGEchoControllerObserver, ObservableObject {
  @Published var foundNodes: [CGEchoNode]

  override init() {
    self.foundNodes = []
    super.init()
    self.observer = self
  }

  deinit {
    self.stop()
  }

  public func nodeAdded(_ controller: CGEchoController, node: CGEchoNode, message: CGEchoMessage) {
    for node in self.nodes() {
      self.foundNodes.append(node as! CGEchoNode)
    }
  }
}
