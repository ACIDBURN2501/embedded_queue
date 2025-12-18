# Queue Library

A small C11, embedded-oriented queue implemented as a typed ring buffer.

- Single-producer / single-consumer (SPSC)
- No dynamic allocation (storage is embedded in the queue instance)
- Configurable full-policy: overwrite oldest (default) or fail-on-full

## Layout

- `src/queue.h` – Public API (macro-generated typed queues).
- `src/queue_version.h.in` – Template for generating the version header.
- `src/queue.c` – Stub for building `libqueue`.

## Versioning

The project version is managed in `meson.build`. During the build process, a `queue_version.h` file is generated in the build directory.

Both static (`libqueue.a`) and shared (`libqueue.so`) libraries are generated. For embedded targets, you typically want to link against the static library.

You can check the version in your code:

```c
#include "queue.h"

#if QUEUE_VERSION_MAJOR == 0
    // handle legacy behavior
#endif
```

## Usage

Define a queue type for your application payload:

```c
#include "queue.h"
#include <stdint.h>

typedef struct {
        uint32_t id;
        uint8_t dlc;
        uint8_t data[8];
} can_msg_t;

QUEUE_DEFINE(can_msg_queue, can_msg_t, 16)

void example(void)
{
        can_msg_queue_t q;
        can_msg_queue_init(&q);

        can_msg_t tx = {.id = 0x123U, .dlc = 8U, .data = {0}};
        (void)can_msg_queue_enqueue(&q, &tx);

        can_msg_t rx;
        if (can_msg_queue_dequeue(&q, &rx) == QUEUE_STATUS_OK) {
                /* use rx */
        }
}
```

## Configuration

- `QUEUE_OVERWRITE_ON_FULL` (default `1`)
  - `1`: enqueue returns `QUEUE_STATUS_OVERWROTE` when it drops the oldest item
  - `0`: enqueue returns `QUEUE_STATUS_FULL` and does not modify the queue
- `QUEUE_ENTER_CRITICAL()` / `QUEUE_EXIT_CRITICAL()` (default no-op)
  - In overwrite mode, the producer may advance the consumer index on full.
  - Define these macros (e.g., disable/enable interrupts) if producer/consumer
    can preempt each other (ISR vs main loop).

## Build & Test

```bash
meson setup build
meson compile -C build
meson test -C build

# run a single test (overwrite)
meson test -C build queue:queue_test_overwrite --print-errorlogs

# run a single test (fail-on-full)
meson test -C build queue:queue_test_fail --print-errorlogs
```

## Formatting

```bash
clang-format -i src/*.c src/*.h test/*.c
```
