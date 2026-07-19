#!/bin/bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
TARGET="$BUILD_DIR/minicompiler"

function ensure_build() {
  if [ ! -x "$TARGET" ]; then
    echo "Compiler not found. Building now..."
    make
  fi

  echo "Do you want to rebuild from scratch (make clean && make)? [y/N]"
  read -r REBUILD
  if [[ "$REBUILD" =~ ^[Yy]$ ]]; then
    make clean
    make
  fi
}

function run_all_tests() {
  echo "Running all test files..."
  local failed=0
  for f in tests/*.ml tests/testcases/*.ml; do
    if [ ! -f "$f" ]; then
      continue
    fi
    echo "\n=== $f ==="
    OUTPUT="$BUILD_DIR/test_$(basename "$f").out"
    "$TARGET" < "$f" > "$OUTPUT" 2>&1
    if grep -E "SYNTAX ERROR|SEMANTIC ERROR|TYPE ERROR" "$OUTPUT" >/dev/null; then
      echo "FAIL"
      failed=1
    else
      echo "PASS"
    fi
  done

  if [ $failed -ne 0 ]; then
    echo "\nSome tests failed. Check the output files in $BUILD_DIR for details."
  else
    echo "\nAll tests passed."    
  fi
}

function run_input_file() {
  input_file="$ROOT_DIR/input.ml"
  if [ ! -f "$input_file" ]; then
    echo "Default input file not found: $input_file"
    echo "Please create input.ml in the project root."
    return
  fi
  echo "Running $input_file..."
  "$TARGET" < "$input_file"
}

function run_direct_input() {
  echo "Enter your MiniLang code. Finish with a line containing only EOF."
  tmpfile="$BUILD_DIR/direct_input.ml"
  mkdir -p "$BUILD_DIR"
  > "$tmpfile"
  while IFS= read -r line; do
    if [[ "$line" == "EOF" ]]; then
      break
    fi
    printf "%s\n" "$line" >> "$tmpfile"
  done
  echo "Running direct input..."
  "$TARGET" < "$tmpfile"
}

function main_menu() {
  echo "MiniCompiler helper"
  echo "===================="
  echo "1) Run all test files"
  echo "2) Run code from default input.ml"
  echo "3) Type code directly"
  echo "4) Build only"
  echo "5) Exit"
  echo "Choose an option [1-5]:"
  read -r choice
  case "$choice" in
    1) run_all_tests ;;
    2) run_input_file ;;
    3) run_direct_input ;;
    4) make ;;
    5) exit 0 ;;
    *) echo "Invalid option." ;;
  esac
}

cd "$ROOT_DIR"

if [ ! -x "$TARGET" ]; then
  echo "Compiler executable not found. Running make first..."
  make
fi

while true; do
  main_menu
  echo "\nDo you want to perform another action? [y/N]"
  read -r again
  if [[ ! "$again" =~ ^[Yy]$ ]]; then
    break
  fi
done
