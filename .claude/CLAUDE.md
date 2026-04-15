# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What is uEcho

`uecho` is a portable C library (v1.2.2) for creating [ECHONET Lite](http://echonet.jp/english/) controllers and IoT devices. ECHONET Lite is an open Japanese IoT standard covering 100+ device types (sensors, air conditioners, etc.). The library handles the communication middleware automatically so callers only need to implement listeners.

## Build Commands

```sh
# Initial setup (regenerate Autotools files)
./bootstrap && ./configure

# Build with tests
./configure --enable-test && make

# Run all tests
make check

# Build with examples
./configure --enable-examples && make

# Build for ESP32
./configure --with-esp32 && make

# Build with coverage instrumentation
./configure --enable-test --enable-coverage && make check
./coverage_summary.sh

# Format code (runs clang-tidy --fix then clang-format)
./clang-format
```

To run a single test binary directly after building:
```sh
./test/unix/uechotest --run_test=NodeTest
```

## Architecture

The object model forms a strict hierarchy:

```
uEchoController
  └── uEchoNode (local node, owns a uEchoServer)
  └── uEchoNodeList (discovered remote nodes)
        └── uEchoObject (ECHONET Lite object; group+class+instance code)
              └── uEchoProperty (EPC code, attribute flags, byte data)
```

**Controller** (`include/uecho/controller.h`, `src/uecho/controller.c`) — top-level entry point. Owns one local `uEchoNode` and a list of discovered remote nodes. Provides `search`, `sendmessage`, `postmessage` (synchronous request/response), and listener callbacks for incoming messages and node status changes.

**Node** (`include/uecho/node.h`, `src/uecho/node.c`) — represents one ECHONET Lite node (IP address). Wraps a `uEchoServer` for networking and holds a list of `uEchoObject`s. The node profile object (code `0x0EF001`) is created automatically.

**Server / Frame layer** (`src/uecho/frame/`) — networking internals hidden from public API. A `uEchoServer` owns a list of `uEchoUdpServer`s (one per network interface) and `uEchoMcastServer`s for the ECHONET Lite multicast group. Each server runs a dedicated `uEchoThread`. Message routing goes through `uEchoMessageObserverManager`.

**Platform layer** (`lib/unix/`, `lib/esp32/`) — provides `uEchoSocket`, `uEchoThread`, `uEchoMutex`, and `uEchoCond`. Select platform via `./configure --with-esp32`; the default is POSIX/Unix.

**Standard database** (`include/uecho/std/`, `src/uecho/std/`) — pre-populated object/property definitions sourced from the MRA (Machine Readable Appendix). Used by `uecho_node_setobject` to set up standard device properties automatically.

## Public vs Internal Headers

The public API uses opaque types (`typedef void uEchoNode`). Internal structs are defined in files prefixed with `_` (e.g., `src/uecho/_node.h`, `src/uecho/_controller.h`) and guarded by `_UECHO_*_INTERNAL_H_` macros. Implementation files include the `_*.h` internal header, not the public one directly.

## Coding Style

- Two-space indentation; braces on their own line for functions.
- Public API: `uecho_` prefix, object-grouped names — e.g., `uecho_node_new`, `uecho_node_delete`, `uecho_controller_start`.
- Pointer types written as `uEchoNode*` (no space before `*`).
- New source files must include the BSD license banner.
- Private headers go in `src/uecho/`; public declarations in `include/uecho/`.

## Tests

Tests use Boost.Test (C++) and live in `test/`. All sources compile into a single binary `test/unix/uechotest`. To add a test: create `test/YourTest.cpp` and add it to `test/unix/Makefile.am` under `uechotest_SOURCES`.

## Commit Style

Use conventional commit prefixes: `feat:`, `fix:`, `refactor:`, `test:`, `docs:`, `chore:`, `perf:`, `style:`. Keep subjects under 72 characters. Do not commit `config.log`, coverage artifacts, or build outputs.
