# AGENTS.md

## Build / Test
- Configure: `meson setup build` (use `--wipe` to reset)
- Build: `meson compile -C build`
- All tests: `meson test -C build`
- Single test: `meson test -C build queue:queue_test --print-errorlogs`
- Direct run: `./build/test/queue_test`

## Lint / Format
- Lint: `clang-tidy -p build src/queue.c test/test_queue.c` (uses `build/compile_commands.json`)
- Format: `clang-format -i src/*.c src/*.h test/*.c` (uses `.clang-format`)

## Code Style
- C11 APIs; prefer `size_t` and `bool`; be explicit about thread-safety
- Naming: functions `snake_case`; public typedefs like `queue_t`; struct members `snake_case`
- Includes: project headers first, then standard headers; keep includes minimal
- Errors: return `NULL`/`-1`/`false`; avoid `abort()` in library code (tests may `assert`)
- Headers: `#ifndef/#define/#endif` guards; keep public API in `src/queue.h`

## Agent Rules
- No Cursor/Copilot rules found (`.cursor/rules/`, `.cursorrules`, `.github/copilot-instructions.md`).
