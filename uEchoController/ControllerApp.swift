//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import SwiftUI

class ControllerAppDelegate: NSObject, UIApplicationDelegate, ObservableObject {
  var controller:Optional<Controller>

  override init() {
    self.controller = nil
  }

  func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions: [UIApplication.LaunchOptionsKey: Any]?
  ) -> Bool {
    self.controller?.start()
    self.controller?.search()
    return true
  }

  func applicationDidBecomeActive(_ application: UIApplication) {
    self.controller?.search()
  }

  func applicationWillTerminate(_ application: UIApplication) {
    self.controller?.stop()
  }
}

@main
struct ControllerApp: App {
  @UIApplicationDelegateAdaptor private var appDelegate: ControllerAppDelegate
  @StateObject var controller = Controller()
  var body: some Scene {
    WindowGroup {
      ContentView()
    }
  }

  init() {
    self.appDelegate.controller = self.controller
  }
}
