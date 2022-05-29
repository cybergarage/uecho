//
//  ContentView.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho
import SwiftUI

struct ItemView: View, Identifiable {
  var id: String
  let node: CGEchoNode
  var body: some View {
    HStack {
      Text(node.address())
        .padding()
    }
  }
}

struct ContentView: View {
  @EnvironmentObject var controller: Controller

  init() {
  }

  var body: some View {
    VStack {
      Text("Hello, world!")
        .padding()
      NavigationView {
        Form {
          Section {
            List(self.controller.foundNodes) { node in
              NavigationLink(destination: Text(node.address())) {
                Text(node.address())
              }
            }
          }
        }
      }
    }.onAppear {
    }.onDisappear {
    }
  }
}

struct ContentView_Previews: PreviewProvider {
  static var previews: some View {
    ContentView()
  }
}
