# Building and Installation

The `uecho` is a framework that consists of header and library files. Currently, `uecho` supports macOS and Linux platforms such as Ubuntu and Raspbian, and ESP32 microcontrollers through [ESP-IDF](espidf.md).

## Homebrew (macOS, Linux)

For platforms that support [Homebrew](https://brew.sh/), you can easily install using the following `brew` commands:

```
brew tap cybergarage/homebrew
brew install uecho
```

## Installing from Source

To install on your platform from the GitHub repository, run the following commands in a terminal:

```
git clone https://github.com/cybergarage/uecho.git
cd uecho
./bootstrap && ./configure && make && sudo make install
```

CMake is also supported:

```
cmake -S . -B build && cmake --build build
```

### Standard object database

uEcho includes the standard object, property and manufacturer definitions from the ECHONET Consortium MRA as const tables. To leave them out and keep only the super class and the node profile class, which uEcho needs itself, use:

```
./configure --with-database=none
cmake -S . -B build -DUECHO_DATABASE=none
```

Objects then get only the super class properties, so add the class properties your device implements with `uecho_object_setproperty()`. See [Standard object database](espidf.md#standard-object-database) for details.

## ESP32 (ESP-IDF)

The repository can be used directly as an ESP-IDF component. See [ESP32 (ESP-IDF)](espidf.md).
