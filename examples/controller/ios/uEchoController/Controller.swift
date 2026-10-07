//
//  Controller.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho
import Foundation

/// Observable wrapper around CGEchoController for SwiftUI.
///
/// All CGEcho callbacks are delivered on the main queue (the default
/// callbackQueue), so published properties are updated on the main thread.
final class Controller: NSObject, ObservableObject, CGEchoControllerDelegate {
  @Published private(set) var nodes: [CGEchoRemoteNode] = []
  @Published private(set) var isRunning = false
  @Published private(set) var isDiscovering = false
  @Published var lastError: String?

  private let echo: CGEchoController

  override init() {
    echo = CGEchoController()
    super.init()
    echo.delegate = self
  }

  func start() {
    echo.start { [weak self] error in
      guard let self else { return }
      if let error {
        self.lastError = error.localizedDescription
        return
      }
      self.isRunning = true
      self.discover()
    }
  }

  func stop() {
    echo.stop { [weak self] in
      self?.isRunning = false
      self?.isDiscovering = false
    }
  }

  func discover() {
    guard isRunning, !isDiscovering else { return }
    isDiscovering = true
    echo.discover(timeout: 0) { [weak self] _, error in
      guard let self else { return }
      self.isDiscovering = false
      if let error {
        self.lastError = error.localizedDescription
      }
      self.nodes = self.echo.nodes
    }
  }

  func fetchCapabilities(of object: CGEchoRemoteObject, completion: @escaping (CGEchoRemoteObject?) -> Void) {
    echo.fetchCapabilities(of: object) { [weak self] updated, error in
      if let error {
        self?.lastError = error.localizedDescription
      }
      completion(updated)
    }
  }

  func read(_ epc: CGEchoEPC, of object: CGEchoRemoteObject, completion: @escaping (CGEchoPropertyValue?) -> Void) {
    echo.readProperty(epc, of: object, timeout: 0) { [weak self] value, error in
      if let error {
        self?.lastError = error.localizedDescription
      }
      completion(value)
    }
  }

  /// Sends SetC. Success means the device accepted the request; read the value back to confirm.
  func write(_ epc: CGEchoEPC, data: Data, of object: CGEchoRemoteObject, completion: @escaping (Bool) -> Void) {
    echo.writeProperty(epc, data: data, of: object, options: [], timeout: 0) { [weak self] error in
      if let error {
        self?.lastError = error.localizedDescription
      }
      completion(error == nil)
    }
  }

  // MARK: CGEchoControllerDelegate

  func echoController(_ controller: CGEchoController, didAdd node: CGEchoRemoteNode) {
    nodes = controller.nodes
  }

  func echoController(_ controller: CGEchoController, didUpdate node: CGEchoRemoteNode) {
    nodes = controller.nodes
  }
}
