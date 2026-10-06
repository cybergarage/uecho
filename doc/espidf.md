# ESP32 (ESP-IDF)

The `uecho` repository can be used directly as an [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) component. On ESP-IDF the library runs on the ESP-IDF POSIX layer (pthreads over FreeRTOS) and lwIP sockets, so the same `src/uecho` sources are used as on Unix; no separate platform library is needed.

## Requirements

- ESP-IDF v5.5 or later (v5.x)
- A target with Wi-Fi or Ethernet
  - Tested on hardware: ESP32 (Wi-Fi)
  - Build-tested in CI: `esp32`, `esp32s3`, `esp32c3`
- IPv4 networking: ECHONET Lite uses the IPv4 multicast group `224.0.23.0:3610`

## Using uecho in your project

Point `EXTRA_COMPONENT_DIRS` at a checkout of this repository in your project's top-level `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS "path/to/uecho")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(myproject)
```

Alternatively, add the repository as a git submodule under your project's `components/` directory. In both cases ESP-IDF names the component after the checkout directory (normally `uecho`), so list that name in `REQUIRES` of your `main` component:

```cmake
idf_component_register(SRCS "main.c" REQUIRES uecho)
```

The public headers are included the same way as on other platforms:

```c
#include <uecho/node.h>
#include <uecho/device.h>
```

## Required configuration

Add the following to your project's `sdkconfig.defaults`:

```
CONFIG_LWIP_IPV4=y
CONFIG_LWIP_SO_REUSE=y
CONFIG_LWIP_SO_REUSE_RXTOALL=y
CONFIG_LWIP_MAX_SOCKETS=10
```

- `CONFIG_LWIP_IPV4` and `CONFIG_LWIP_SO_REUSE` are mandatory; the component configuration fails without them.
- `CONFIG_LWIP_SO_REUSE_RXTOALL` delivers each multicast datagram to every socket bound to port 3610. Without it, only one of the unicast/multicast receivers sees a search request.
- A node opens a unicast and a multicast socket per network interface. Raise `CONFIG_LWIP_MAX_SOCKETS` if your application uses other sockets.

### uEcho options (`idf.py menuconfig` → *Component config* → *uEcho*)

| Option | Default | Description |
|---|---|---|
| `CONFIG_UECHO_THREAD_STACK_SIZE` | 8192 | Stack size in bytes for each uEcho worker thread. Node message listeners and property request handlers run on these threads; increase it if your callbacks use a lot of stack. |

## Starting a node

Start the node only after the network interface has an IPv4 address, because the servers bind to the addresses found at `uecho_node_start()`. If the address changes (for example after a Wi-Fi reconnect), stop and start the node again:

```c
uEchoNode* node = uecho_node_new();
uecho_node_addobject(node, obj);

/* after IP_EVENT_STA_GOT_IP */
uecho_node_start(node);

/* after WIFI_EVENT_STA_DISCONNECTED */
uecho_node_stop(node);
```

Wi-Fi power save delays multicast delivery to the station. Disable it with `esp_wifi_set_ps(WIFI_PS_NONE)` if discovery from controllers is slow or unreliable.

## Writing a device

A device is built the same way as on Unix: create an object, declare its properties, and register request handlers. Handlers run on uEcho worker threads, so keep them short and do not block; hand longer work to your own task.

```c
#define LIGHT_OBJECT_CODE 0x029101
#define LIGHT_POWER 0x80

static bool light_power_handler(uEchoObject* obj, uEchoProperty* prop, uEchoEsv esv, size_t pdc, byte* edt)
{
  if ((pdc != 1) || !edt)
    return false;                          /* rejected: SetC_SNA (0x51) */
  switch (edt[0]) {
  case 0x30: gpio_set_level(LED_GPIO, 1); return true;  /* accepted: Set_Res (0x71) */
  case 0x31: gpio_set_level(LED_GPIO, 0); return true;
  default: return false;
  }
}

uEchoObject* obj = uecho_device_new();
byte off = 0x31;
uecho_object_setmanufacturercode(obj, 0xFFFFF0);  /* experimental code; use your assigned code for products */
uecho_object_setcode(obj, LIGHT_OBJECT_CODE);
uecho_object_setproperty(obj, LIGHT_POWER, uEchoPropertyAttrReadWrite);
uecho_object_setpropertydata(obj, LIGHT_POWER, &off, 1);
uecho_object_setpropertywriterequesthandler(obj, LIGHT_POWER, light_power_handler);
```

When a handler returns `true` for a write request, uEcho stores the new value, so later read requests return it.

## Example: uecholight

[`examples/espidf/uecholight`](../examples/espidf/uecholight) is a mono functional lighting device (`0x029101`) on Wi-Fi. Writing the operation status property (EPC `0x80`) switches an optional LED GPIO.

### Configure

Run `idf.py menuconfig` and open *uEcho light example*:

| Option | Default | Description |
|---|---|---|
| `CONFIG_UECHO_EXAMPLE_WIFI_SSID` | (empty) | Wi-Fi SSID. The example stops at startup if it is empty. |
| `CONFIG_UECHO_EXAMPLE_WIFI_PASSWORD` | (empty) | Wi-Fi password. Stored in plaintext in `sdkconfig` and the firmware, so do not commit it. |
| `CONFIG_UECHO_EXAMPLE_LED_GPIO` | -1 | GPIO driven high while the light is ON. `-1` only logs the status. Many ESP32 DevKit boards have an LED on GPIO 2. |
| `CONFIG_UECHO_EXAMPLE_MANUFACTURER_CODE` | 0xFFFFF0 | ECHONET Lite manufacturer code (`0xFFFFF0` is for experimental use). |

### Build, flash and monitor

```sh
. $IDF_PATH/export.sh
cd examples/espidf/uecholight
idf.py set-target esp32
idf.py menuconfig
idf.py -p /dev/ttyUSB0 flash monitor
```

The serial monitor shows the address the node is bound to:

```
I (2272) uecholight: Got IP 192.168.100.56
I (2282) uecholight: uEcho node started
```

Exit the monitor with `Ctrl+]`.

### Verify from a host

Build the controller examples on a host in the same network (`./configure --enable-examples && make`), then run them from `examples/controller`:

```
$ ./uechosearch/unix/uechosearch
192.168.100.56  [0] 0EF001 [1] 029101

$ ./uechopost/unix/uechopost 192.168.100.56 029101 62 8000      # Get: OFF (0x31)
192.168.100.56 029101 72 800131

$ ./uechopost/unix/uechopost 192.168.100.56 029101 61 800130    # SetC: ON
192.168.100.56 029101 71 8000

$ ./uechopost/unix/uechopost 192.168.100.56 029101 61 800131    # SetC: OFF
192.168.100.56 029101 71 8000
```

The monitor logs `POWER = ON` / `POWER = OFF` for each write. `71` means the write was accepted; `51` means it was rejected.

## Memory usage

Measured on ESP32 (ESP-IDF v5.5.1) with the `uecholight` object:

- About 130 KB of heap for the first node. Most of it (about 124 KB) is the standard object database loaded from the MRA, which is shared and kept for the lifetime of the application; it is not freed by `uecho_node_delete()`, and later nodes reuse it.
- One worker thread (stack `CONFIG_UECHO_THREAD_STACK_SIZE`) per unicast and per multicast receiver, that is two per network interface.
- The `uecholight` example image is about 880 KB, so the default 1 MB application partition has little headroom left; use a larger partition table for bigger applications.

## Platform differences

- IPv4 only. IPv6 multicast is not joined on ESP-IDF.
- POSIX signals are not available; worker threads poll a stop flag and their sockets use a 1 second receive timeout, so `uecho_node_stop()` can take up to about one second.
- Network interfaces are enumerated with `esp_netif`, and only interfaces that are up and have an IPv4 address are used.

## Troubleshooting

- **`Could not exclusively lock port /dev/ttyUSB0`**: another program has the serial port open, usually an earlier `idf.py monitor`. Exit it with `Ctrl+]`, or find it with `fuser -v /dev/ttyUSB0` (or `lsof /dev/ttyUSB0`). Serial monitors in editors and ModemManager can also hold the port.
- **The device is not found by `uechosearch`**: check that the host and the board are on the same subnet, that the router forwards multicast to Wi-Fi clients (IGMP snooping can block `224.0.23.0`), and that `CONFIG_LWIP_SO_REUSE_RXTOALL=y` is set. Wi-Fi power save can also delay multicast; the example disables it.
- **Writes return `51` unexpectedly**: make sure you are talking to the right node. Other ECHONET Lite devices on the network may expose the same object code; use the address from the `Got IP` log line.
- **Host examples fail to link with `undefined reference to __gcov_init`**: the host library was built earlier with `--enable-coverage`. Run `make clean` before building again with different `configure` options.

