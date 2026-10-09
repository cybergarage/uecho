![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/cybergarage/uecho)
[![Build Status](https://github.com/cybergarage/uecho/actions/workflows/make.yml/badge.svg)](https://github.com/cybergarage/uecho/actions/workflows/make.yml)
[![doxygen](https://github.com/cybergarage/uecho/actions/workflows/doxygen.yml/badge.svg)](http://cybergarage.github.io/uecho/)
[![codecov](https://codecov.io/gh/cybergarage/uecho/graph/badge.svg?token=106EGSA1VI)](https://codecov.io/gh/cybergarage/uecho)

![logo](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/logo.png)

uEcho for C (`uecho`) is a portable, cross-platform framework for developing [ECHONET Lite][enet] controllers and devices. ECHONET Lite is an open standard for IoT devices in Japan. It defines more than 100 device types, including security sensors, air conditioners, and refrigerators.

`uecho` runs on macOS and Linux, and on ESP32 microcontrollers as an [ESP-IDF](https://github.com/cybergarage/uecho/blob/master/doc/espidf.md) component.

## What is uEcho?

`uecho` provides APIs for controlling [ECHONET Lite][enet] devices and implementing device applications. Its C API follows object-oriented naming conventions, with functions grouped into components such as `Controller`, `Node`, `Class`, and `Object`.

![Framework](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/framework.png)

Implementing ECHONET Lite controllers and devices from scratch requires handling protocol details such as message formats and communication sequences.

`uecho` handles protocol requests and notifications, allowing developers to focus on their application logic by configuring listeners.

## Table of Contents

- [Building and Installation](https://github.com/cybergarage/uecho/blob/master/doc/setup.md)
- [ESP32 (ESP-IDF)](https://github.com/cybergarage/uecho/blob/master/doc/espidf.md)
- [Objective-C and Swift (macOS / iOS)](https://github.com/cybergarage/uecho/blob/master/doc/objc.md)
- Controller
  - [Overview of Controller](https://github.com/cybergarage/uecho/blob/master/doc/controller_overview.md)
  - [Inside of Controller](https://github.com/cybergarage/uecho/blob/master/doc/controller_inside.md)
- Device
  - [Overview of Device](https://github.com/cybergarage/uecho/blob/master/doc/device_overview.md)
  - [Inside of Device](https://github.com/cybergarage/uecho/blob/master/doc/device_inside.md)
- [Examples](https://github.com/cybergarage/uecho/blob/master/doc/examples.md)

## Related projects

[uecho-simulator](https://github.com/cybergarage/uecho-simulator) is a small ECHONET Lite development simulator with virtual lighting, air conditioning, and temperature sensing. It provides a full-screen terminal UI and a live, read-only browser preview, runs offline by default, and implements limited device profiles.

## References

For details about the C API, see the generated documentation:

* [Doxygen](http://cybergarage.github.io/uecho/)

[enet]:https://echonet.jp/english/
