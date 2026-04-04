*This project has been created as part of the 42 curriculum by qrios.*

# Codexion

## Description

Codexion is a multithreaded C project that simulates coders competing for shared
USB dongles before reaching their burnout deadline.

Each coder is represented by a thread. To compile, a coder must hold two
dongles at the same time. After compiling, the coder debugs, then refactors,
then tries to compile again. The simulation stops when every coder has compiled
`number_of_compiles_required` times, or when one coder burns out.

Two scheduling strategies are supported for dongle arbitration:

- `fifo`: requests are served in arrival order
- `edf`: requests are served by earliest deadline first

## Instructions

### Build

```sh
make
```

### Run

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
	time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```sh
./codexion 3 800 200 200 200 3 10 fifo
```

Arguments:

- `number_of_coders`: number of coder threads and dongles
- `time_to_burnout`: maximum delay in ms between the start of two compiles
- `time_to_compile`: compile duration in ms
- `time_to_debug`: debug duration in ms
- `time_to_refactor`: refactor duration in ms
- `number_of_compiles_required`: compile target for each coder
- `dongle_cooldown`: mandatory cooldown in ms after a dongle is released
- `scheduler`: `fifo` or `edf`

## Resources

Useful references:

- POSIX threads manual pages: `pthread_create`, `pthread_mutex_lock`,
  `pthread_cond_wait`, `pthread_cond_timedwait`
- `gettimeofday(2)` and `usleep(3)`
- Priority queue / binary heap documentation
- The Codexion subject PDF

AI usage:

AI was used as a support tool for review, debugging, and documentation drafting.
All final code and explanations were manually checked and adjusted.

## Blocking Cases Handled

- Deadlock prevention: coder 1 does not take dongles in the same order as the
  others, which breaks the circular wait condition.
- Fair access to a dongle: each dongle maintains a waiting queue managed by a
  heap and ordered with the selected scheduler.
- Cooldown enforcement: a released dongle cannot be taken again before
  `dongle_cooldown` milliseconds.
- Burnout detection: a dedicated monitor thread checks deadlines and stops the
  simulation when a coder burns out.
- Single coder case: the single coder can only take one dongle, waits, and then
  burns out as expected.

## Thread Synchronization Mechanisms

- One `pthread_mutex_t` per dongle protects its state and its waiting queue.
- One `pthread_cond_t` per dongle wakes waiting coders when the dongle becomes
  available again or when the simulation stops.
- One mutex per coder protects `compile_count` and `last_compile_start`.
- `stop_mutex` protects the global stop flag.
- `print_mutex` serializes logs so lines are not interleaved.
