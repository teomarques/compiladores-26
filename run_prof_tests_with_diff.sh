#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPILER="${COMPILER:-$SCRIPT_DIR/jucompiler}"
TEST_ROOT="${TEST_ROOT:-$SCRIPT_DIR/../repo-prof/java/meta2}"
OUT_ROOT="${OUT_ROOT:-$SCRIPT_DIR/prof-results/meta2}"

if [[ ! -x "$COMPILER" ]]; then
  echo "Erro: compilador nao encontrado ou sem permissao de execucao: $COMPILER"
  echo "Sugestao: execute 'make' em $SCRIPT_DIR"
  exit 1
fi

if [[ ! -d "$TEST_ROOT" ]]; then
  echo "Erro: diretorio de testes nao encontrado: $TEST_ROOT"
  exit 1
fi

mkdir -p "$OUT_ROOT"

mode_for_test() {
  local file="$1"
  local header
  header="$(head -n 8 "$file" | tr '[:upper:]' '[:lower:]')"

  if grep -q -- "-t" <<< "$header"; then
    echo "-t"
  elif grep -q -- "-e2" <<< "$header"; then
    echo "-e2"
  elif grep -q -- "-e1" <<< "$header"; then
    echo "-e1"
  elif grep -q -- "-l" <<< "$header"; then
    echo "-l"
  else
    echo "-t"
  fi
}

pass=0
fail=0
skip=0

declare -a test_files=()
if (( $# > 0 )); then
  for name in "$@"; do
    if [[ -f "$name" ]]; then
      test_files+=("$name")
    elif [[ -f "$TEST_ROOT/$name.java" ]]; then
      test_files+=("$TEST_ROOT/$name.java")
    else
      echo "========================================"
      echo "SKIP $name (teste nao encontrado)"
      skip=$((skip + 1))
    fi
  done
else
  while IFS= read -r -d '' test_file; do
    test_files+=("$test_file")
  done < <(find "$TEST_ROOT" -type f -name '*.java' -print0 | sort -z)
fi

for test_file in "${test_files[@]}"; do
  expected_file="${test_file%.java}.out"
  rel_path="${test_file#"$TEST_ROOT"/}"
  if [[ "$rel_path" == "$test_file" ]]; then
    rel_path="$(basename "$test_file")"
  fi
  got_file="$OUT_ROOT/${rel_path%.java}.got"
  diff_file="$OUT_ROOT/${rel_path%.java}.diff"
  mode="$(mode_for_test "$test_file")"

  mkdir -p "$(dirname "$got_file")"

  if [[ ! -f "$expected_file" ]]; then
    echo "========================================"
    echo "SKIP $rel_path (sem .out esperado)"
    skip=$((skip + 1))
    continue
  fi

  "$COMPILER" "$mode" < "$test_file" > "$got_file" 2>&1

  echo "========================================"
  echo "TESTE: $rel_path"
  echo "MODO : $mode"

  if diff -u "$expected_file" "$got_file" > "$diff_file"; then
    echo "RESULTADO: PASS"
    echo "DIFF: sem diferencas"
    pass=$((pass + 1))
  else
    echo "RESULTADO: FAIL"
    echo "DIFF:"
    cat "$diff_file"
    fail=$((fail + 1))
  fi
done

echo "========================================"
echo "Resumo: PASS=$pass FAIL=$fail SKIP=$skip TOTAL=$((pass + fail + skip))"
echo "Outputs em: $OUT_ROOT"

if (( fail > 0 )); then
  exit 1
fi
