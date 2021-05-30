# GCSTR [![Build Status](https://travis-ci.org/xyproto/gcstr.svg?branch=main)](https://travis-ci.org/xyproto/gcstr)

Garbage collected strings for C, inspired by Go.

## Requirements

* A C11 compiler (or later).
* [gc](http://www.hboehm.info/gc/) (the `gc` package in Arch Linux)
* `gcc`, `make` and `pkg-config` (included in the `base-devel` group in Arch Linux)

## Guidelines/limitations/ideas

* `-1` should never be returned to indicate a problem or that something is not found. Return a struct instead.
* `String*` should be supported by every function that can take a string.
* The `Error*` type can be `nil` to indicate no error. Otherwise, it should contain info about the error.
* In lack of generics, use function signatures that contains the types, like `const char* ErrorToConstChar(Error* err);`. It's verbose, but clear.
* Make things correct first, then fast later.

## Building and running tests

### With [cxx](https://github.com/xyproto/cxx)

    cxx run

### With make

    make
    ./main

## General info

* Version: 0.0.1
* License: BSD
* Author: Alexander F. Rødseth &lt;xyproto@archlinux.org&gt;
