#!/usr/bin/env bash
set -euo pipefail

for f in testes/meta4/*.java; do
	n=$(basename "$f" .java)
	./Meta4/src/jucompiler < "$f" > "/tmp/$n.ll" 2> "/tmp/$n.err"
	if [ -s "/tmp/$n.err" ]; then
		echo "FAIL $n (stderr)"
		continue
	fi
	if [ -f "testes/meta4/$n.ll" ]; then
		diff -q "testes/meta4/$n.ll" "/tmp/$n.ll" || echo "IR MISMATCH $n"
	fi
	if [ -f "testes/meta4/$n.in" ] && [ -f "testes/meta4/$n.out" ]; then
		if [ -s "testes/meta4/$n.in" ]; then
			lli "/tmp/$n.ll" $(cat "testes/meta4/$n.in") > "/tmp/$n.out" 2> "/tmp/$n.lli_err"
		else
			lli "/tmp/$n.ll" > "/tmp/$n.out" 2> "/tmp/$n.lli_err"
		fi
		diff -q "testes/meta4/$n.out" "/tmp/$n.out" || echo "EXEC FAIL $n"
	fi
done
