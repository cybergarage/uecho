![logo](img/uecho_logo.png)

uEcho for C is a portable and cross platform development framework for [ECHONETLite][enet] developers. [ECHONETLite][enet] is an open standard specification for IoT devices in Japan, it specifies more than 100 IoT devices such as crime prevention sensor, air conditioner and refrigerator.

![RaspberyPi](img/uecholight_raspberry_pi.jpg)

The uEcho supports to control devices ECHONET Lite and create the standard devices of the specification easily. The uEcho is designed in object-oriented programming and inspired by reactive programming too.

![framwork](doc/img/uecho_framework.png)

To implement IoT controller or devices of ECHONET Lite, developer had to understand and implement the communication middleware specification such as the message format and base sequences. However, developer must only to set basic listeners to implement the devices and controllers using the uEcho. The uEcho handles other requests such as request and notification requests automatically.

- [What is uEcho ?](doc/uecho_overview.md)
- [Setup](doc/uecho_setup.md)
- Controller
  - [Overview of Controller](doc/uecho_controller_overview.md)
  - [Inside of Controller](doc/uecho_controller_inside.md)
- Device
  - [Overview of Device](doc/uecho_device_overview.md)
  - [Inside of Device](doc/uecho_device_inside.md)
- [Examples](doc/uecho_examples.md)

[enet]:http://echonet.jp/english/
