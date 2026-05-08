#!/bin/bash

# Run all targeted tests and save outputs
# Usage: bash run_tests.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
COMPILER="$PROJECT_ROOT/meta3/jucompiler"

if [ ! -f "$COMPILER" ]; then
    echo "ERROR: Compiler not found at $COMPILER"
    echo "Please run 'cd meta3 && make' first"
    exit 1
fi

cd "$SCRIPT_DIR" || exit 1

COMBINED_OUTPUT="all_results.txt"

# Header in combined file
{
    echo "============================================"
    echo "Targeted Test Results"
    echo "Generated: $(date)"
    echo "============================================"
} > "$COMBINED_OUTPUT"

echo "============================================"
echo "Running targeted tests"
echo "============================================"

for java_file in test_*.java; do
    if [ ! -f "$java_file" ]; then
        continue
    fi

    output_file="${java_file%.java}.myout"

    echo ""
    echo "==> $java_file"
    echo "----"

    "$COMPILER" -s < "$java_file" > "$output_file" 2>&1

    # Show on terminal
    cat "$output_file"
    echo "----"

    # Append to combined file
    {
        echo ""
        echo "============================================"
        echo "==> $java_file"
        echo "============================================"
        echo "--- INPUT ---"
        cat "$java_file"
        echo ""
        echo "--- OUTPUT ---"
        cat "$output_file"
        echo ""
    } >> "$COMBINED_OUTPUT"
done

echo ""
echo "============================================"
echo "Individual outputs saved as .myout files"
echo "Combined output saved to: $COMBINED_OUTPUT"
echo "============================================"
