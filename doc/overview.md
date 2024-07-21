![logo](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/logo.png)

![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/cybergarage/uecho)
[![Build Status](https://github.com/cybergarage/uecho/actions/workflows/make.yml/badge.svg)](https://github.com/cybergarage/uecho/actions/workflows/make.yml)
[![doxygen](https://github.com/cybergarage/uecho/actions/workflows/doxygen.yml/badge.svg)](http://cybergarage.github.io/uecho/)

uEcho for C, `uecho`,  is a portable and cross platform development framework for creating controller applications and devices of [ECHONET Lite][enet]. [ECHONET][enet] is an open standard specification for IoT devices in Japan, it specifies more than 100 IoT devices such as crime prevention sensor, air conditioner and refrigerator.

## What is uEcho ?

The `uecho` supports to control any [ECHONET Lite][enet] devices or create the standard devices of the specification easily. The `uecho` is designed in object-oriented programming, and the functions are object-oriented in their naming convention, and are grouped into classes such as `Controller`, `Node`, `Class` and `Object`.

![framwork](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/framework.png)

To implement IoT controller or devices of [ECHONET Lite][enet], the developer had to understand and implement the communication middleware specification such as the message format and base sequences.

Using The `uecho`, all developer has only to set basic listeners to implement the devices and controllers because The `uecho` handles other requests such as request and notification requests automatically.

## Table of Contents

- [Building and Installation](https://github.com/cybergarage/uecho/blob/master/doc/setup.md)
- Controller
  - [Overview of Controller](https://github.com/cybergarage/uecho/blob/master/doc/controller_overview.md)
  - [Inside of Controller](https://github.com/cybergarage/uecho/blob/master/doc/controller_inside.md)
- Device
  - [Overview of Device](https://github.com/cybergarage/uecho/blob/master/doc/device_overview.md)
  - [Inside of Device](https://github.com/cybergarage/uecho/blob/master/doc/device_inside.md)
- [Examples](https://github.com/cybergarage/uecho/blob/master/doc/examples.md)

## References

To develop [ECHONET Lite][enet] controller applications or devices using uEcho for C, please refer to the following developer's documentation to know the functions in more detail.

* [Doxygen](http://cybergarage.github.io/uecho/)

[enet]:http://echonet.jp/english/
