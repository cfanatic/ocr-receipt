#!/usr/bin/env bash

main() {
    cmake -GNinja ..
    ninja
    ctest
}

main "$@"
