//
//  ContentView.swift
//  uEchoController
//
//  Created by Satoshi Konno on 2022/05/02.
//

import CGEcho
import SwiftUI

struct ContentView: View {
  @EnvironmentObject var controller: Controller

  var body: some View {
    NavigationView {
      List {
        if let error = controller.lastError {
          Section {
            Text(error).foregroundStyle(.red)
          }
        }
        Section(controller.isDiscovering ? "Searching..." : "Nodes (\(controller.nodes.count))") {
          ForEach(controller.nodes) { node in
            NavigationLink(destination: NodeView(node: node)) {
              VStack(alignment: .leading) {
                Text(node.address)
                Text(node.objects.map { $0.eojString }.joined(separator: ", "))
                  .font(.caption)
                  .foregroundStyle(.secondary)
              }
            }
          }
        }
      }
      .navigationTitle("uEcho Controller")
      .refreshable {
        controller.discover()
      }
      .toolbar {
        Button("Search") {
          controller.discover()
        }
        .disabled(!controller.isRunning || controller.isDiscovering)
      }
    }
  }
}

/// LabeledContent is iOS 16+; keep the sample on the framework's iOS 15.4 minimum.
struct InfoRow: View {
  let title: String
  let value: String

  init(_ title: String, value: String) {
    self.title = title
    self.value = value
  }

  var body: some View {
    HStack {
      Text(title)
      Spacer()
      Text(value).foregroundStyle(.secondary)
    }
  }
}

struct NodeView: View {
  let node: CGEchoRemoteNode

  var body: some View {
    List(node.objects) { object in
      NavigationLink(destination: ObjectView(object: object)) {
        Text(object.eojString)
      }
    }
    .navigationTitle(node.address)
  }
}

struct ObjectView: View {
  @EnvironmentObject var controller: Controller
  @State var object: CGEchoRemoteObject
  @State private var operationStatus: String = "-"
  @State private var loaded = false

  private static let operationStatusEPC: CGEchoEPC = 0x80

  var body: some View {
    List {
      Section("Object") {
        InfoRow("Address", value: object.address)
        InfoRow("EOJ", value: object.eojString)
      }
      Section("Capabilities") {
        InfoRow("Get", value: describe(object.capabilities.readable))
        InfoRow("Set", value: describe(object.capabilities.writable))
        InfoRow("Announce", value: describe(object.capabilities.notifiable))
      }
      Section("Operation status (0x80)") {
        InfoRow("Value", value: operationStatus)
        Button("Read") { readStatus() }
        if object.capabilities.writable.contains(Self.operationStatusEPC) {
          Button("ON") { writeStatus(0x30) }
          Button("OFF") { writeStatus(0x31) }
        }
      }
    }
    .navigationTitle(object.eojString)
    .onAppear {
      // onAppear can run more than once during navigation; load only once.
      guard !loaded else { return }
      loaded = true
      controller.fetchCapabilities(of: object) { updated in
        if let updated {
          object = updated
        }
        readStatus()
      }
    }
  }

  private func describe(_ map: CGEchoPropertyMap) -> String {
    switch map.state {
    case .available:
      return "\(map.properties?.count ?? 0) properties"
    case .failed:
      return "failed"
    default:
      return "unknown"
    }
  }

  private func readStatus() {
    controller.read(Self.operationStatusEPC, of: object) { value in
      guard let byte = value?.data.first else { return }
      switch byte {
      case 0x30: operationStatus = "ON (0x30)"
      case 0x31: operationStatus = "OFF (0x31)"
      default: operationStatus = String(format: "0x%02X", byte)
      }
    }
  }

  private func writeStatus(_ byte: UInt8) {
    controller.write(Self.operationStatusEPC, data: Data([byte]), of: object) { accepted in
      if accepted {
        readStatus()  // SetC success is only acceptance; read back to confirm.
      }
    }
  }
}

struct ContentView_Previews: PreviewProvider {
  static var previews: some View {
    ContentView().environmentObject(Controller())
  }
}
