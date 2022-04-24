#!/usr/bin/env bash

main() {
    cmake ..
    make
    ctest
}

main "$@"
