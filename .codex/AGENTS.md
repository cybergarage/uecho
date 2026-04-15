# Repository Guidelines

## Project Structure & Module Organization

`uecho` is an Autotools-based C library for ECHONET Lite controller and device development. Public headers live in `include/uecho/`, while implementation files are under `src/uecho/`. Platform library glue is in `lib/`, including `lib/unix/`, `lib/esp32/`, and Xcode projects for macOS and iOS. Example controller and device programs are in `examples/`. Tests are C++ Boost.Test sources in `test/`, with the Unix test runner configured in `test/unix/`. Documentation is in `doc/`, and Objective-C wrapper code is in `wrapper/objc/`.

## Build, Test, and Development Commands

- `./bootstrap`: regenerate Autotools metadata after changing `Makefile.am` inputs.
- `./configure`: configure a normal local build with examples enabled by default.
- `./configure --enable-test`: configure test builds; requires Boost.System and Boost.Test.
- `make`: build headers, libraries, and enabled subdirectories.
- `make check`: run configured tests, including `test/unix/uechotest` when tests are enabled.
- `./configure --enable-coverage && make check`: build with gcov coverage flags and run tests.
- `sudo make install`: install the built library and headers after validation.

## Coding Style & Naming Conventions

Use the repository formatter before submitting C, C++, Objective-C, and header changes:

```sh
./clang-format
```

Formatting is based on WebKit style with 2-space indentation, Stroustrup braces, and non-binned arguments/parameters. Follow existing `uecho_*` public API naming and keep C source functions lower case. `.clang-tidy` enforces naming checks: globals/constants in `UPPER_CASE`, classes/structs/enums in `CamelCase`, members and parameters in `camelBack`, and functions in `lower_case`.

## Testing Guidelines

Tests use Boost.Test and feature-oriented filenames such as `NodeTest.cpp`, `PropertyTest.cpp`, and `SocketTest.cpp`. Add new test files under `test/` and register them in `test/unix/Makefile.am` so they build into `uechotest`. Prefer focused tests for public API behavior, protocol message handling, and platform abstractions. Run `./configure --enable-test && make check` before opening a pull request.

## Commit & Pull Request Guidelines

Recent history uses both conventional prefixes, such as `docs:` and `chore:`, and short imperative messages like `Update Makefile`. Prefer concise, scoped messages: `fix: handle empty property maps` or `docs: update setup notes`. Pull requests should describe the behavior changed, list validation commands run, and link related issues. Include screenshots only for documentation or Xcode/UI-facing changes.

## Security & Configuration Tips

Do not commit generated local logs, coverage outputs, or machine-specific build artifacts. Keep generated Autotools files synchronized when changing build inputs, and avoid changing platform project files unless the corresponding platform build is affected.
