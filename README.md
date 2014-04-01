## mruby-embed

Proof-of-Concept for embedding [mruby] and Ruby code into a statically linked 
executable.

All Ruby code in the `lib/` directory is compiled into mruby IR code using the
`mrbc` utility. The IR code is then embedded in `src/lib.c` as an array of
`uint8_t` data, which is loaded via `mruby_load_ireps`. All C code within
`src/` is then compiled and linked against `libmruby.a`, resulting in
a static binary that will run Ruby code.

### Build

    rake

### Run

    $ ./bin
    >> 1 + 1
    => 2

[mruby]: https://github.com/mruby/mruby
