# emb_lib

emb_lib is a small library collection with some useful implementations that we can always found at embedded system projects

## Project Status

Active development - Thread safety support added in 2025

## Libraries implemented

* circular buffer
* queue
* stack
* deque
* list
* set
* string builder
* utilities

## Thread Safety Support

This library now supports optional thread safety with multiple backends:

- **C11 Atomics** (recommended for modern systems)
- **POSIX Threads** (pthread)
- **FreeRTOS** (embedded RTOS)
- **Windows Threads**

### Quick Start with Thread Safety

```bash
# Enable C11 atomics (best performance)
cmake -DEMBLIB_ATOMICS=ON ..

# Enable pthread-based thread safety
cmake -DEMBLIB_THREAD_SAFETY=ON -DEMBLIB_THREAD_BACKEND=PTHREAD ..

# Disable thread safety (default, zero overhead)
cmake ..
```

See `docs/THREAD_SAFETY_GUIDE.md` for detailed information.
