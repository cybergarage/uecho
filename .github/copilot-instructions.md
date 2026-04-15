# Repository Guidelines

## Project Structure & Module Organization

uEcho is a C library for ECHONET Lite devices and controllers. Public headers live in `include/uecho/`, with utilities under `include/uecho/util/` and standard database headers under `include/uecho/std/`. Core implementation files are in `src/uecho/`. Platform support is split under `lib/unix/` and `lib/esp32/`. Tests are C++ Boost unit tests in `test/`, with the Unix test runner in `test/unix/`. Documentation lives in `doc/`, and samples are under `examples/controller/` and `examples/device/`.

## Build, Test, and Development Commands

- `./bootstrap && ./configure`: regenerate Autotools files and configure a local build.
- `./configure --enable-test`: configure with the test subdirectory enabled.
- `./configure --enable-examples`: configure with example programs enabled.
- `./configure --with-esp32`: configure for ESP32-specific sources.
- `make`: build headers, libraries, and any enabled subdirectories.
- `make check`: run configured tests, including `test/unix/uechotest` when tests are enabled.
- `./coverage_summary.sh`: summarize coverage data when coverage artifacts are present.
- `./clang-format`: run `clang-tidy --fix` when available, then format C, C++, Objective-C, and header files.

## Coding Style & Naming Conventions

Keep the existing C style: two-space indentation, braces on their own line for functions, compact guard clauses, and pointer types such as `uEchoNode*`. Public API names use the `uecho_` prefix and object grouping, for example `uecho_node_new`, `uecho_node_delete`, and `uecho_controller_*`. Place private headers with leading underscores in `src/uecho/`, and public declarations in `include/uecho/`. Preserve the project license banner in new source files.

## Testing Guidelines

Tests use Boost.Test and are named by feature, for example `NodeTest.cpp`, `PropertyTest.cpp`, and `SocketTest.cpp`. Add new test sources under `test/` and register them in `test/unix/Makefile.am` so they compile into `uechotest`. Prefer focused tests around public API behavior and platform abstractions. Run `./configure --enable-test && make check` before submitting changes.

## Commit & Pull Request Guidelines

Recent history uses short subjects, often with prefixes such as `docs:` and `chore:`; examples include `docs: update documents` and `chore: update manufacturer codes to latest version`. Keep commits scoped to one concern and mention generated Autotools changes when included. Pull requests should describe the behavior changed, list test commands run, link related issues, and include screenshots only for documentation or visual asset changes.

## Security & Configuration Tips

Do not commit local build outputs, generated coverage data, or machine-specific `config.log` changes unless intentionally updating generated build files. Keep network and thread behavior portable across Linux, macOS, and ESP32 backends.
