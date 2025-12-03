#!/bin/sh

echo "Building project..."

for file in src/*.c; do
    if [ -f "$file" ]; then
        clang "$file" -o "out/$(basename "${file%.c}")"
    fi

done