//
//  uEchoControllerApp.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import SwiftUI

@main
struct ControllerApp: App {
    @StateObject var controller = Controller()
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
