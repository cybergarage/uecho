# ESP32 (ESP-IDF)

The `uecho` repository can be used directly as an [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) component. On ESP-IDF the library runs on the ESP-IDF POSIX layer (pthreads over FreeRTOS) and lwIP sockets, so the same `src/uecho` sources are used as on Unix; no separate platform library is needed.

## Requirements

- ESP-IDF v5.5 or later (v5.x)
- A target with Wi-Fi or Ethernet (tested build target: `esp32`)
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

## Example: uecholight

[`examples/espidf/uecholight`](../examples/espidf/uecholight) is a mono functional lighting device (`0x029101`). Writing the operation status property (EPC `0x80`) switches an LED GPIO.

```sh
cd examples/espidf/uecholight
idf.py set-target esp32
idf.py menuconfig      # uEcho light example: Wi-Fi SSID/password, LED GPIO
idf.py build flash monitor
```

Then discover the device from a host on the same network with the `uechosearch` example:

```
$ uechosearch
192.168.xxx.yyy [0] 0EF001 [1] 029101
```

## Memory usage

Measured on ESP32 (ESP-IDF v5.5.1) with the `uecholight` object:

- About 130 KB of heap for the first node. Most of it (about 124 KB) is the standard object database loaded from the MRA, which is shared and kept for the lifetime of the application; it is not freed by `uecho_node_delete()`, and later nodes reuse it.
- One worker thread (stack `CONFIG_UECHO_THREAD_STACK_SIZE`) per unicast and per multicast receiver, that is two per network interface.
- The `uecholight` example image is about 880 KB, so the default 1 MB application partition has little headroom left; use a larger partition table for bigger applications.

## Platform differences

- IPv4 only. IPv6 multicast is not joined on ESP-IDF.
- POSIX signals are not available; worker threads poll a stop flag and their sockets use a 1 second receive timeout, so `uecho_node_stop()` can take up to about one second.
- Network interfaces are enumerated with `esp_netif`, and only interfaces that are up and have an IPv4 address are used.
