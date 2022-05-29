//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import SwiftUI
import CGEcho

extension CGEchoController: ObservableObject {
}

@main
struct ControllerApp: App {
    @StateObject var controller = CGEchoController()
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
