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

## ESP32 (ESP-IDF)

The repository can be used directly as an ESP-IDF component. See [ESP32 (ESP-IDF)](espidf.md).
