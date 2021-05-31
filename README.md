# gcstr

Safe garbage collected strings for C, inspired by Go.

## Requirements

* A C compiler that supports C11, ie. `gcc` or `clang`.
* The garbage collection library [gc](http://www.hboehm.info/gc/) (available as the `gc` package in many Linux distros).
* `make` and `pkg-config`.

## Guidelines/limitations/ideas

* `-1` should never be returned to indicate a problem or that something is not found. Return a struct instead.
* `String*` should be supported by every function that can take a string.
* The `Error*` type can be `nil` to indicate no error. Otherwise, it should contain info about the error.
* In lack of generics, use function signatures that contains the types, like `const char* ErrorToConstChar(Error* err);`. It's verbose, but clear.
* Make things correct first, then fast later.
* All functions may panic if memory can not be allocated.
* If functions may panic for other reasons, the function name must be prefixed with **Must**.
* Error messages should start with a lowercase letter and not end with punctuation.
* The API is stable for each major version number.

## Building and running tests

    make
    ./main

Alternatively, install [cxx](https://github.com/xyproto/cxx) and use `cxx run`.

## Installation

    make install

## General info

* Version: 1.0.0
* License: BSD
* Author: Alexander F. Rødseth &lt;xyproto@archlinux.org&gt;
