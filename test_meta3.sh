#!/usr/bin/env bash
# Correr todos os testes Meta 3: compara saída do jucompiler com testes/meta3/*.out
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
JUC="${ROOT}/Meta3/src/jucompiler"

if [[ ! -f "$JUC" ]]; then
    echo "Erro: não existe executável ${JUC}" >&2
    echo "Compila primeiro: ./build_meta3.sh" >&2
    exit 2
fi

PASS=0
FAIL=0
TOTAL=0
TMPDIR="${TMPDIR:-/tmp}"

for f in "${ROOT}/testes/meta3"/*.java; do
    [[ -f "$f" ]] || continue
    name=$(basename "$f" .java)
    expected="${ROOT}/testes/meta3/${name}.out"
    if [[ -f "$expected" ]]; then
        TOTAL=$((TOTAL + 1))
        "$JUC" < "$f" > "${TMPDIR}/meta3_${name}.myout" 2>&1 || true
        if diff -q "$expected" "${TMPDIR}/meta3_${name}.myout" > /dev/null 2>&1; then
            PASS=$((PASS + 1))
        else
            FAIL=$((FAIL + 1))
            echo "FAIL: $name"
        fi
    fi
done

echo "Meta 3: passaram ${PASS} de ${TOTAL}; falharam ${FAIL}"
if [[ "$FAIL" -gt 0 ]]; then
    exit 1
fi
