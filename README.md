# Tetris

Good old tetris, written in C using raylib.
Heavily inspired by <link> and <link>

### Dependencies

- `gcc` compiler
- `make` for building and running
- raylib 5.5 built and added to path

### Building and running

```sh
make build # or just make
```

for release builds:
```sh
make build BUILD=release
```
The built executable will be in the `bin/` directory.

You can also run directly using:
```sh
make run
```

### Todos:

- Fix the leaking Music object
- Add pause and resume logic
- Refine the UI
- Add menus and settings