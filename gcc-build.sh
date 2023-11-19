#!/bin/sh
mkdir -p ./build
gcc ./src/main.c ./src/args.c ./src/color.c ./src/file_io.c ./src/file_path.c -o ./build/image2brick
