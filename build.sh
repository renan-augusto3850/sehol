#!/bin/bash
OUTPUT_DIR="bin"
OUTPUT_FILE="sehol2"
MAIN="src/test/parser_test.cpp"

[ ! -d "$OUTPUT_DIR" ] && mkdir -p "$OUTPUT_DIR"

g++ -Wall -Wextra $MAIN -o $OUTPUT_DIR/$OUTPUT_FILE

if [ $? -eq 0 ]; then
    echo "COMPILATION DONE. TARGET: $OUTPUT_DIR/$OUTPUT_FILE"
else
    echo "COMPILATION ERROR"
fi