//
//  ControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import SwiftUI

@main
struct ControllerApp: App {
  @StateObject var controller = Controller()
  @Environment(\.scenePhase) private var scenePhase

  var body: some Scene {
    WindowGroup {
      ContentView().environmentObject(controller)
    }
    .onChange(of: scenePhase) { phase in
      switch phase {
      case .active:
        // Background reception is not guaranteed; refresh the session when returning.
        controller.start()
      case .background:
        controller.stop()
      default:
        break
      }
    }
  }
}
