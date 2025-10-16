# Overview

![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/cybergarage/uecho)
[![Build Status](https://github.com/cybergarage/uecho/actions/workflows/make.yml/badge.svg)](https://github.com/cybergarage/uecho/actions/workflows/make.yml)
[![doxygen](https://github.com/cybergarage/uecho/actions/workflows/doxygen.yml/badge.svg)](http://cybergarage.github.io/uecho/)

uEcho for C (`uecho`) is a portable and cross-platform development framework for creating controller applications and devices that support [ECHONET Lite](http://echonet.jp/english/). [ECHONET][enet] is an open standard specification for IoT devices in Japan that defines more than 100 IoT device types, including crime prevention sensors, air conditioners, and refrigerators.

## What is uEcho?

The `uecho` framework enables developers to easily control any [ECHONET Lite](http://echonet.jp/english/) devices or create standard-compliant devices. The `uecho` is designed using object-oriented programming principles, with functions following object-oriented naming conventions and organized into classes such as `Controller`, `Node`, `Class`, and `Object`.

![framework](https://raw.githubusercontent.com/cybergarage/uecho/master/doc/img/framework.png)

Traditionally, implementing IoT controllers or devices for [ECHONET Lite](http://echonet.jp/english/) required developers to understand and implement complex communication middleware specifications, including message formats and base sequences.

With `uecho`, developers only need to set up basic listeners to implement devices and controllers, as the framework automatically handles other requests such as standard request and notification sequences.
