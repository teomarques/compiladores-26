#!/bin/bash

pass_count=0
total_count=0
fail_count=0

rm -f fail_*.log

for file in ../testes/meta4/*.java; do
    if [ ! -f "$file" ]; then continue; fi
    
    total_count=$((total_count+1))
    
    base="${file%.java}"
    in_file="${base}.in"
    out_file="${base}.out"
    
    # touch missing files
    if [ ! -f "$in_file" ]; then touch "$in_file"; fi
    if [ ! -f "$out_file" ]; then touch "$out_file"; fi
    
    # Run compiler
    ./jucompiler < "$file" > /tmp/t.ll 2>/tmp/t.comp_err
    
    # Check if compiler produced anything
    if [ ! -s /tmp/t.ll ]; then
        echo "FAIL: $file (compiler produced no IR)"
        fail_count=$((fail_count+1))
        echo "=== FAILED: $file ===" > "fail_${total_count}.log"
        cat /tmp/t.comp_err >> "fail_${total_count}.log"
        continue
    fi
    
    # Run lli
    rm -f /tmp/t.out /tmp/t.err
    lli /tmp/t.ll < "$in_file" > /tmp/t.out 2>/tmp/t.err
    
    # Compare
    diff -w -b "$out_file" /tmp/t.out > /tmp/t.diff
    
    if [ $? -eq 0 ]; then
        echo "PASS: $file"
        pass_count=$((pass_count+1))
    else
        echo "FAIL: $file"
        fail_count=$((fail_count+1))
        echo "=== FAILED: $file ===" > "fail_${total_count}.log"
        cat /tmp/t.diff >> "fail_${total_count}.log"
        echo "--- LLVM IR ---" >> "fail_${total_count}.log"
        cat /tmp/t.ll >> "fail_${total_count}.log"
        echo "--- STDERR ---" >> "fail_${total_count}.log"
        cat /tmp/t.err >> "fail_${total_count}.log"
    fi
done

echo "TOTAL: $total_count, PASS: $pass_count, FAIL: $fail_count"
