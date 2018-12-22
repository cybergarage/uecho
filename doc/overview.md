![logo](img/logo.png)

[![Build Status](https://travis-ci.org/cybergarage/uecho.svg?branch=master)](https://travis-ci.org/cybergarage/uecho)

uEcho for C, `uecho`,  is a portable and cross platform development framework for creating controller applications and devices of [ECHONET Lite][enet]. [ECHONET][enet] is an open standard specification for IoT devices in Japan, it specifies more than 100 IoT devices such as crime prevention sensor, air conditioner and refrigerator.

## What is uEcho ?

The `uecho` supports to control any [ECHONET Lite][enet] devices or create the standard devices of the specification easily. The `uecho` is designed in object-oriented programming, and the functions are object-oriented in their naming convention, and are grouped into classes such as `Controller`, `Node`, `Class` and `Object`.

![framwork](img/framework.png)

To implement IoT controller or devices of [ECHONET Lite][enet], the developer had to understand and implement the communication middleware specification such as the message format and base sequences.

Using The `uecho`, all developer has only to set basic listeners to implement the devices and controllers because The `uecho` handles other requests such as request and notification requests automatically.

# Table of Contents

- [What is uEcho ?](overview.md)
- [Setup](setup.md)
- Controller
  - [Overview of Controller](controller_overview.md)
  - [Inside of Controller](controller_inside.md)
- Device
  - [Overview of Device](device_overview.md)
  - [Inside of Device](device_inside.md)
- [Examples](examples.md)

[enet]:http://echonet.jp/english/
