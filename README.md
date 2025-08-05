# Tetris

Good old tetris, written in C using raylib.
Heavily inspired by <https://github.com/KineticTactic/Tetris-C.git> and <https://github.com/educ8s/Cpp-Tetris-Game-with-raylib>

## Dependencies

- a C compiler compiler (`gcc` or `clang`)
- `make` for building and running
- `clang-format` for formatting code
- `clang-tidy` for linting
- raylib v5.5 built (as a shared library) and added to path

## Building and running

It is possible to set the 'CC' (C compiler), 'OS', 'ENABLE_SAN' and 'BUILD' variables in a .env file in the root of the project as such:

```env
CC=            # gcc or clang
BUILD=         # debug or release
OS=            # Linux, Darwin or Windows_NT
ENABLE_SAN=    # true or false; whether to enable sanitizers
```

or set them in environment variables. Then build using:

```sh
make build
```

The default build target produces a debug build, with ASAN and UBSAN disabled. If no input is supplied through .env file or environment variables, It will default to using `clang` and build using debug profile. and for release builds:

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