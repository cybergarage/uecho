//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import SwiftUI

class ControllerAppDelegate: NSObject, UIApplicationDelegate, ObservableObject {
  func application(
    _ application: UIApplication,
    didFinishLaunchingWithOptions: [UIApplication.LaunchOptionsKey: Any]?
  ) -> Bool {
    return true
  }

  func applicationDidBecomeActive(_ application: UIApplication) {
  }

  func applicationWillTerminate(_ application: UIApplication) {
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
}
