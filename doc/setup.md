# Building and Installation

![logo](img/logo.png)

The `uecho` is a framework which consist of the header and library files. Currently the `uecho` supports MacOSX and Linux platforms such as Ubuntu and Raspbian.

## Homebrew (macOS, Linux)

For any platforms which support [Homebrew](https://brew.sh/), you can easily install using Homebrew with the following `brew` commands:

```
brew tap cybergarage/homebrew
brew install uecho
```

## Installing from source

To install on your platform from the Github repository, run the following in a terminal:

```
git clone https://github.com/cybergarage/uecho.git
cd uecho
./bootstrap && ./configure && make && sudo make install
```
