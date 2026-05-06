#!/bin/bash


META=${1:-3}
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

case $META in
    1)
        COMPILER="$BASE_DIR/meta1/jucompiler"
        TESTES=("$BASE_DIR/testes/meta1")
        FLAGS=""
        ;;
    2)
        COMPILER="$BASE_DIR/meta2/jucompiler"
        TESTES=("$BASE_DIR/testes/meta2")
        FLAGS="-t"
        ;;
    3)
        COMPILER="$BASE_DIR/meta3/jucompiler"
        TESTES=("$BASE_DIR/testes/meta3")
        FLAGS="-s"
        ;;
    *)
        echo "Usage: $0 [1|2|3]"
        exit 1
        ;;
esac

if [ ! -f "$COMPILER" ]; then
    echo "Error: $COMPILER not found"
    exit 1
fi

PASSED=0
FAILED=0
TOTAL=0

OUTPUT_FILE="test_results_meta${META}.txt"

echo "Testing Meta $META - Tests in progress..."
echo "Testing Meta $META" > "$OUTPUT_FILE"
echo "==================================" >> "$OUTPUT_FILE"

for test_dir in "${TESTES[@]}"; do
    if [ ! -d "$test_dir" ]; then
        continue
    fi

    for java_file in "$test_dir"/*.java; do
        if [ ! -f "$java_file" ]; then
            continue
        fi

        test_name=$(basename "$java_file")
        expected_file="${java_file%.java}.out"
        ((TOTAL++))

        # Run compiler
        if ! timeout 5 "$COMPILER" $FLAGS < "$java_file" > /tmp/test_out.txt 2>&1; then
            echo "[FAIL] $test_name (compiler crashed)" >> "$OUTPUT_FILE"
            ((FAILED++))
            continue
        fi

        # Check if expected output file exists
        if [ ! -f "$expected_file" ]; then
            ((TOTAL--))
            continue
        fi

        # Compare output
        if diff -q "$expected_file" /tmp/test_out.txt > /dev/null 2>&1; then
            echo "[PASS] $test_name" >> "$OUTPUT_FILE"
            ((PASSED++))
        else
            echo "[FAIL] $test_name" >> "$OUTPUT_FILE"
            ((FAILED++))
        fi
    done
done

echo "" >> "$OUTPUT_FILE"
echo "==================================" >> "$OUTPUT_FILE"
echo "Total: $TOTAL | Passed: $PASSED | Failed: $FAILED" >> "$OUTPUT_FILE"
echo "==================================" >> "$OUTPUT_FILE"

echo "Test output saved to $OUTPUT_FILE"

if [ $FAILED -gt 0 ]; then
    exit 1
fi
exit 0
