//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho

public class Controller: CGEchoController, CGEchoControllerObserver, ObservableObject {
  @Published var foundNodes: [CGEchoNode]
  @Published var foundNodeCount: Int

  override init() {
    self.foundNodes = []
    self.foundNodeCount = 0
    super.init()
    self.observer = self
  }

  deinit {
    self.stop()
  }

  public func nodeAdded(_ controller: CGEchoController, node: CGEchoNode, message: CGEchoMessage) {
    var newFoundNodes: [CGEchoNode] = []
    for node in self.nodes() {
      newFoundNodes.append(node as! CGEchoNode)
      //self.foundNodes.append(node as! CGEchoNode)
    }
    self.foundNodes = newFoundNodes
    self.foundNodeCount = foundNodes.count
  }
}
