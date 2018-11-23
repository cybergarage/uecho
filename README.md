![logo](doc/img/logo.png)

uEcho for C is a portable and cross platform development framework for [ECHONET Lite][enet] developers. [ECHONET Lite][enet] is an open standard specification for IoT devices in Japan, it specifies more than 100 IoT devices such as crime prevention sensor, air conditioner and refrigerator.

# What is uEcho ?

The uEcho supports to control devices of [ECHONET Lite][enet] and create the standard devices of the specification easily. The uEcho is designed in object-oriented programming and inspired by reactive programming too.

![framwork](doc/img/framework.png)

To implement IoT controller or devices of ECHONET Lite, developer had to understand and implement the communication middleware specification such as the message format and base sequences. However, developer must only to set basic listeners to implement the devices and controllers using the uEcho. The uEcho handles other requests such as request and notification requests automatically.

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
