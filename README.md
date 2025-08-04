# Tetris

Good old tetris, written in C using raylib.
Heavily inspired by <https://github.com/KineticTactic/Tetris-C.git> and <https://github.com/educ8s/Cpp-Tetris-Game-with-raylib>

## Dependencies

- a C compiler compiler (`gcc` or `clang`)
- `make` for building and running
- raylib v5.5 built (as a shared library) and added to path

## Building and running

```sh
make build # or just make
```

The default build target produces a debug build with ASAN and UBSAN enabled.

for release builds:

```sh
make build BUILD=release
```

The built executable will be in the `bin/` directory.

You can also run directly using:

```sh
make run
```

### Todos

- [ ] Fix the leaking Music object
- [ ] Add animation for block clearance
- [ ] Add pause and resume logic
- [ ] Refine the UI
- [ ] Add menus and settings
