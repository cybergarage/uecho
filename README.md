![logo](doc/img/logo.png)

[![Build Status](https://travis-ci.org/cybergarage/uecho-c.svg?branch=master)](https://travis-ci.org/cybergarage/uecho-c)

uEcho for C, `uecho-c`,  is a portable and cross platform development framework for creating controller applications and devices of [ECHONET Lite][enet]. [ECHONET][enet] is an open standard specification for IoT devices in Japan, it specifies more than 100 IoT devices such as crime prevention sensor, air conditioner and refrigerator.

## What is uEcho ?

The `uecho-c` supports to control any [ECHONET Lite][enet] devices and create the standard devices of the specification easily. The `uecho-c` is designed in object-oriented programming, and the functions are object-oriented in their naming convention, and are grouped into classes such as `Controller`, `Node`, `Class` and `Object`.

![framwork](doc/img/framework.png)

To implement IoT controller or devices of [ECHONET Lite][enet], the developer had to understand and implement the communication middleware specification such as the message format and base sequences.

Using The `uecho-c`, all developer has only to set basic listeners to implement the devices and controllers because The `uecho-c` handles other requests such as request and notification requests automatically.

# Table of Contents

- [Setup](doc/setup.md)
- Controller
  - [Overview of Controller](doc/controller_overview.md)
  - [Inside of Controller](doc/controller_inside.md)
- Device
  - [Overview of Device](doc/device_overview.md)
  - [Inside of Device](doc/device_inside.md)
- [Examples](doc/examples.md)

[enet]:http://echonet.jp/english/
