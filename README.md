# Queue Library

A small C11 queue library implementing a fixed-capacity FIFO of `int` values.

## Layout

- `src/queue.h` – Public API.
- `src/queue.c` – Implementation.
- `test/test_queue.c` – Simple test executable (Meson test).
- `meson.build`, `test/meson.build` – Meson build definitions.

## Public API

Declared in `src/queue.h`:

- `queue_t *queue_create(size_t capacity)`
- `void queue_destroy(queue_t *q)`
- `int queue_enqueue(queue_t *q, int item)` returns `0` on success, `-1` on failure/full
- `bool queue_dequeue(queue_t *q, int *out_item)` returns `true` on success
- `size_t queue_size(const queue_t *q)`
- `bool queue_is_empty(const queue_t *q)`
- `bool queue_is_full(const queue_t *q)`

## Thread-safety

This implementation is not thread-safe. Use external locking if multiple
threads will enqueue/dequeue concurrently.

## Build & Test

```bash
meson setup build
meson compile -C build

# run all tests
meson test -C build

# run a single test
meson test -C build queue:queue_test --print-errorlogs

# run the test directly
./build/test/queue_test
```

## Formatting

```bash
clang-format -i src/*.c src/*.h test/*.c
```
