#!/usr/bin/env sh
set -e

CLIBS="-lraylib -lm"
CFLAGS="-Wall -Wextra -pedantic -std=c11"

gcc $CFLAGS -o tet ./src/tt.c ./src/main.c $CLIBS

if [ "$1" = "run" ]; then
    shift
    ./tet "$@"
fi
