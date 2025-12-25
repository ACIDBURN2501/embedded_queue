# AGENTS.md

## Build / Test
- Configure: `meson setup build` (use `--wipe` to reset)
- Build: `meson compile -C build`
- All tests: `meson test -C build`
- Single test (overwrite): `meson test -C build queue:queue_test_overwrite --print-errorlogs`
- Single test (fail-on-full): `meson test -C build queue:queue_test_fail --print-errorlogs`

## Lint / Format
- Lint: `clang-tidy -p build src/queue.c test/*.c` (uses `build/compile_commands.json`)
- Format: `clang-format -i src/*.c src/*.h test/*.c` (uses `.clang-format`)

## Code Style
- C11 APIs; prefer `size_t` and `bool`; be explicit about thread-safety
- Naming: functions `snake_case`; generated queue types `foo_queue_t`
- Includes: project headers first, then standard headers; keep includes minimal
- Errors: return status codes (`queue_status_t`); avoid `abort()` in library code
- Headers: `#ifndef/#define/#endif` guards; keep public API in `src/queue.h`. Note that `src/queue_version.h` is a generated file.
- Versioning: Managed via `project(version: ...)` in `meson.build`.

## Agent Rules
- No Cursor/Copilot rules found (`.cursor/rules/`, `.cursorrules`, `.github/copilot-instructions.md`).
