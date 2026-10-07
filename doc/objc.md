# Objective-C and Swift (CGEcho.framework)

`CGEcho.framework` is a client-side [ECHONET Lite][enet] controller for macOS and iOS apps, built on top of uEcho for C. It discovers remote nodes, fetches their property maps, sends Get and SetC requests, and receives INF notifications. Device hosting is not part of the framework; use the C API for devices.

## Requirements

- macOS 12 or later, iOS 15.4 or later
- Xcode (the project is a single multiplatform framework target)

## Project layout

| Path | Contents |
|------|----------|
| `wrapper/objc/uEcho/` | Public headers and implementation (`CGEcho*.h/m`) |
| `wrapper/objc/uEcho/Private/` | Frame codec and the transport to the C controller |
| `wrapper/objc/xcode/CGEcho.xcodeproj` | Framework, tests, and the `uecholight` tool for macOS |
| `wrapper/objc/xcode/generate_xcodeproj.py` | Generator for `CGEcho.xcodeproj` |
| `wrapper/objc/xcode/Tests/` | XCTest suites |
| `examples/controller/ios/` | SwiftUI sample app for iOS |

The framework compiles the C sources in `src/` directly, so no prebuilt `libuecho.a` is needed. The Xcode project is generated; regenerate it after adding or removing source files instead of editing `project.pbxproj` by hand:

```
python3 wrapper/objc/xcode/generate_xcodeproj.py
```

## Building and testing

Open `wrapper/objc/xcode/CGEcho.xcodeproj` in Xcode. The generated project has three schemes:

| Scheme | Purpose |
|--------|---------|
| `CGEcho` | Builds the framework and runs the unit tests on My Mac or an iOS Simulator. The tests use an in-memory transport and do not touch the network. |
| `CGEchoSmoke` | Runs `CGEchoSmokeTests` against a real node on the network (sets `CGECHO_SMOKE=1`). |
| `uecholight` | Runs the [uecholight](examples.md) lighting device on macOS. |

### Network smoke test

The smoke test discovers nodes, fetches the property maps of a mono functional lighting object (`0x029101`), reads `0x80`, writes it with SetC, and reads it back. Because it writes to the target, it only uses a node running on this host, or the address given explicitly:

```
python3 wrapper/objc/xcode/generate_xcodeproj.py --smoke-address 192.168.1.10
```

Start `uecholight -v` on **another host** (see [Examples](examples.md) for building it on Linux), then run the `CGEchoSmoke` scheme. Do not commit a scheme generated with `--smoke-address`.

> A node and a controller on the same host cannot exchange unicast requests, because both bind UDP port 3610. Discovery (multicast) still works, but Get and SetC do not reach the node.

## Usage

### Swift

```swift
import CGEcho

let controller = CGEchoController()

controller.start { error in
  guard error == nil else { return }
  controller.discover(timeout: 0) { nodes, error in
    // An empty array is a valid result.
    guard let object = nodes.first?.objects.first else { return }
    controller.fetchCapabilities(of: object) { object, error in
      guard let object else { return }
      controller.readProperty(0x80, of: object, timeout: 0) { value, error in
        print(value?.data as Any)
      }
      if object.capabilities.writable.contains(0x80) {
        controller.writeProperty(0x80, data: Data([0x30]), of: object, options: [], timeout: 0) { error in
          // Success means the device accepted the request. Read it back to confirm.
        }
      }
    }
  }
}
```

### Objective-C

```objc
#import <CGEcho/CGEcho.h>

CGEchoController* controller = [[CGEchoController alloc] init];
[controller startWithCompletion:^(NSError* error) {
  [controller discoverWithTimeout:0 completion:^(NSArray<CGEchoRemoteNode*>* nodes, NSError* error) {
    CGEchoRemoteObject* object = nodes.firstObject.objects.firstObject;
    if (!object)
      return;
    [controller readProperty:0x80 ofObject:object timeout:0 completion:^(CGEchoPropertyValue* value, NSError* error) {
      NSLog(@"%@", value.data);
    }];
  }];
}];
```

Keep the controller alive for the whole session. A timeout of `0` uses the value from `CGEchoControllerConfiguration`.

### Notifications

`observeNotifications(of:handler:)` registers a local listener for INF (`0x73`) notifications from one object, or from every object when `nil` is passed. Call `invalidate()` on the returned token to stop observing. Instance list notifications (`0xD5`) are handled as discovery and reported through the delegate.

```swift
let observation = controller.observeNotifications(of: object) { value in
  print(value.epc, value.data)
}
```

`CGEchoControllerDelegate` reports added and updated nodes and all received notifications.

## Behavior

- **Callbacks.** Completions and delegate methods run on `configuration.callbackQueue` (the main queue by default), never on a native receive thread. The queue must be serial.
- **Completion.** Every accepted request completes exactly once. `cancel()` completes it with `CGEchoErrorCancelled`, but a datagram that was already sent is not recalled.
- **Matching.** A response completes a request only if the address, TID, SEOJ/DEOJ, ESV and EPC all match. Duplicates and late responses are ignored.
- **No automatic retry.** Requests are never re-sent. A timeout (`CGEchoErrorTimeout`) means the device state is unknown, in particular after SetC.
- **SetC and capabilities.** SetC is refused locally with `CGEchoErrorCapabilityUnknown` until the Set property map (`0x9E`) has been fetched, unless `CGEchoWriteOptionsAllowUnknownCapability` is given. If the map is known and does not list the EPC, SetC fails with `CGEchoErrorUnsupportedProperty` without sending anything.
- **Property maps.** Maps are validated (length, declared count, range, duplicates). A malformed map is reported as `CGEchoCapabilityStateFailed` with its raw data, not as an empty map.
- **Snapshots.** `CGEchoRemoteNode`, `CGEchoRemoteObject` and `CGEchoPropertyValue` are immutable snapshots. A snapshot taken before `stop` stays readable, but requests with it fail with `CGEchoErrorDeviceUnavailable` in the next session.
- **Nodes.** A node is identified by its address and objects by address and EOJ. ECHONET Lite has no removal message, so nodes are reported as stale after `staleInterval` rather than removed; `forget(_:)` removes one. The controller's own host can appear as a node without objects.
- **Defaults.** Discovery window 3 s, request timeout 5 s, 8 pending requests at most (`CGEchoErrorBusy` beyond that). These are initial values and can be changed in `CGEchoControllerConfiguration`.
- **One controller per process.** Each controller binds UDP port 3610.

## iOS apps

- Add `NSLocalNetworkUsageDescription` to the app's Info.plist. The first network access shows the local network permission prompt.
- Sending multicast on a real device requires the `com.apple.developer.networking.multicast` entitlement, which must be requested from Apple. The iOS Simulator does not need it.
- Reception in the background is not guaranteed. Stop the controller when the app moves to the background, then start and discover again when it becomes active.

## SwiftUI sample

`examples/controller/ios/uEchoController.xcodeproj` is a SwiftUI app for iOS that uses `CGEcho.framework`. It references `CGEcho.xcodeproj` by a relative path, so open it from this repository; no other setup is needed. It discovers nodes, lists their objects, and turns lighting objects on and off. Run it on an iOS Simulator, or on a device with the multicast entitlement described above.

[enet]:http://echonet.jp/english/
