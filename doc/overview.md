# Overview

![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/cybergarage/uecho)
[![Build Status](https://github.com/cybergarage/uecho/actions/workflows/make.yml/badge.svg)](https://github.com/cybergarage/uecho/actions/workflows/make.yml)
[![doxygen](https://github.com/cybergarage/uecho/actions/workflows/doxygen.yml/badge.svg)](http://cybergarage.github.io/uecho/)

uEcho for C (`uecho`) is a portable and cross-platform development framework for building controller applications and devices that work with [ECHONET Lite](http://echonet.jp/english/). [ECHONET][enet] is an open standard specification for IoT devices in Japan that covers more than 100 device categories, such as security sensors, air conditioning units, and refrigerators.

## What is uEcho?

The `uecho` framework allows developers to easily control any [ECHONET Lite](http://echonet.jp/english/) devices or build specification-compliant devices. `uecho` follows object-oriented design principles, with functions using object-oriented naming conventions and organized into classes like `Controller`, `Node`, `Class`, and `Object`.

![framework](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/framework.png)

Previously, implementing IoT controllers or devices for [ECHONET Lite](http://echonet.jp/english/) required developers to understand and implement complex communication middleware specifications, including message formats and communication sequences.

With `uecho`, developers only need to configure basic listeners to implement devices and controllers, since the framework automatically manages standard requests and notification sequences.

[enet]:http://echonet.jp/english/
