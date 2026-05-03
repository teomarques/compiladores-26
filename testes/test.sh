#!/usr/bin/env bash

# examples:
#   ./test.sh -l meta1
#   ./test.sh -t meta2

flag="$1"
suite="$2"

if [ -z "$flag" ] || [ -z "$suite" ]; then
  echo "Usage: $0 <flag> <suite_dir>" >&2
  exit 2
fi

total=0
failed=0

for java_file in "$suite"/*.java; do
  [ -e "$java_file" ] || continue
  total=$((total + 1))

  out_file="${java_file%.java}.out"
  if [ ! -f "$out_file" ]; then
    echo "MISSING: ${out_file##*/}"
    failed=$((failed + 1))
    continue
  fi

  # Compare expected .out with compiler stdout (suppress stderr).
  if ! diff -q "$out_file" <(../jucompiler "$flag" < "$java_file" 2>/dev/null) >/dev/null; then
    echo "DIFF: ${java_file##*/}"
    failed=$((failed + 1))
  fi
done

echo "Total: $total"
echo "Failed: $failed"

exit $((failed != 0))
