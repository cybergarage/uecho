# Building and Installation

The `uecho` is a framework that consists of header and library files. Currently, `uecho` supports macOS and Linux platforms such as Ubuntu and Raspbian.

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
