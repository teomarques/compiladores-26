#!/usr/bin/env bash
# Meta 4 regression harness (IR goldens, lli checks, argv/exec harness, compiler flags).
#
# Phase A — IR diff (strict): For each testes/meta4/*.java with a matching .ll, compile with
#   stdin and require empty stderr; LLVM IR must byte-match the golden file. Any failure
#   exits the script with status 1 and prints up to 15 diff lines.
#
# Phase B — LLI validate: If `lli` is on PATH, compile each Java that has a golden .ll and run
#   `lli` with empty stdin and no extra argv (programs that need CLI args may exit non-zero).
#   Exit 0 counts as PARSE_OK. Non-zero: show up to 5 lines of combined stderr/stdout.
#   By default lli failures do not fail the script; set META4_STRICT_LLI=1 to exit 1 if any fail.
#
# Phase C — Exec goldens: For each basename with both .in and .out, compile to /tmp, then run lli.
#   This dialect has no stdin reads; whitespace-separated tokens from .in are passed as argv to lli
#   (stdin is /dev/null). Set META4_EXEC_STDIN=1 to use `< .in` as stdin instead and pass no argv.
#   Diff program stdout against .out. Default: report PASS/FAIL counts only; set META4_STRICT_EXEC=1
#   to exit 1 on any exec mismatch.
#
# Phase D — Flag smoke: Ensure Meta4/jucompiler still supports earlier modes using testes/meta4/ok_minimal.java:
#   `jucompiler -l` (lexer), `jucompiler -e2` (semantic), `jucompiler -t` (AST dump). All must exit 0;
#   stderr must be empty for -l and -e2; -t may print the AST to stdout.
#
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
JUC="${ROOT}/Meta4/src/jucompiler"
TMPDIR="${TMPDIR:-/tmp}"

STRICT_LLI="${META4_STRICT_LLI:-0}"
STRICT_EXEC="${META4_STRICT_EXEC:-0}"
EXEC_STDIN="${META4_EXEC_STDIN:-0}"

if [[ ! -f "$JUC" ]]; then
	echo "error: missing compiler at ${JUC}" >&2
	echo "run ./build_meta4.sh first" >&2
	exit 2
fi

declare -i PASS_IR=0 FAIL_IR=0 TOTAL_IR=0
declare -i PASS_LLI=0 FAIL_LLI=0 TOTAL_LLI=0
declare -i PASS_EX=0 FAIL_EX=0 TOTAL_EX=0

meta4_compile_to() {
	local src_java="$1"
	local out_ll="$2"
	local err_file="$3"
	"$JUC" <"$src_java" >"$out_ll" 2>"$err_file"
}

## Phase A + prepare tmp IR paths used later
shopt -s nullglob
JAVA_FILES=("${ROOT}/testes/meta4"/*.java)
for f in "${JAVA_FILES[@]}"; do
	[[ -f "$f" ]] || continue
	name=$(basename "$f" .java)
	expected="${ROOT}/testes/meta4/${name}.ll"
	tmp_ll="${TMPDIR}/meta4_${name}.ll"
	tmp_err="${TMPDIR}/meta4_${name}.err"
	if [[ ! -f "$expected" ]]; then
		continue
	fi
	TOTAL_IR=$((TOTAL_IR + 1))
	meta4_compile_to "$f" "$tmp_ll" "$tmp_err"
	if [[ -s "$tmp_err" ]]; then
		FAIL_IR=$((FAIL_IR + 1))
		echo "IR FAIL (stderr): $name — compiler wrote to stderr:"
		head -n 15 "$tmp_err"
		continue
	fi
	if diff -q "$expected" "$tmp_ll" >/dev/null 2>&1; then
		PASS_IR=$((PASS_IR + 1))
	else
		FAIL_IR=$((FAIL_IR + 1))
		echo "IR FAIL (diff): $name"
		diff -u "$expected" "$tmp_ll" | head -n 15 || true
	fi
done

echo "Phase A IR: PASS=${PASS_IR} FAIL=${FAIL_IR} TOTAL=${TOTAL_IR}"

## Phase B
if command -v lli >/dev/null 2>&1; then
	for f in "${JAVA_FILES[@]}"; do
		[[ -f "$f" ]] || continue
		name=$(basename "$f" .java)
		expected="${ROOT}/testes/meta4/${name}.ll"
		[[ -f "$expected" ]] || continue
		TOTAL_LLI=$((TOTAL_LLI + 1))
		tmp_ll="${TMPDIR}/meta4_${name}.ll"
		tmp_err="${TMPDIR}/meta4_${name}.err"
		# tmp_ll from Phase A should exist; re-compile if missing (paranoia)
		if [[ ! -f "$tmp_ll" ]]; then
			meta4_compile_to "$f" "$tmp_ll" "$tmp_err"
			if [[ -s "$tmp_err" ]]; then
				FAIL_LLI=$((FAIL_LLI + 1))
				echo "LLI SKIP/BAD-IR (stderr): $name"
				continue
			fi
		fi
		set +e
		lli "$tmp_ll" </dev/null >"${TMPDIR}/meta4_${name}.lli.stdout" 2>"${TMPDIR}/meta4_${name}.lli.stderr"
		lli_ec=$?
		set -e
		if [[ "$lli_ec" -eq 0 ]]; then
			PASS_LLI=$((PASS_LLI + 1))
		else
			FAIL_LLI=$((FAIL_LLI + 1))
			echo "LLI FAIL (exit ${lli_ec}): $name"
			{
				head -n 5 "${TMPDIR}/meta4_${name}.lli.stderr" 2>/dev/null || true
				head -n 5 "${TMPDIR}/meta4_${name}.lli.stdout" 2>/dev/null || true
			} | sed 's/^/  | /'
		fi
	done
	echo "Phase B LLI: PASS=${PASS_LLI} FAIL=${FAIL_LLI} TOTAL=${TOTAL_LLI}"
else
	echo "Phase B LLI: skipped (lli not found)"
fi

## Phase C — .in / .out pairs
for f in "${JAVA_FILES[@]}"; do
	[[ -f "$f" ]] || continue
	name=$(basename "$f" .java)
	infile="${ROOT}/testes/meta4/${name}.in"
	outfile="${ROOT}/testes/meta4/${name}.out"
	[[ -f "$infile" && -f "$outfile" ]] || continue
	TOTAL_EX=$((TOTAL_EX + 1))
	tmp_ll="${TMPDIR}/meta4_${name}.ll"
	tmp_err="${TMPDIR}/meta4_${name}.err"
	meta4_compile_to "$f" "$tmp_ll" "$tmp_err"
	if [[ -s "$tmp_err" ]]; then
		FAIL_EX=$((FAIL_EX + 1))
		echo "EXEC FAIL (compile stderr): $name"
		head -n 5 "$tmp_err" || true
		continue
	fi
	tmp_exeout="${TMPDIR}/meta4_${name}.exeout"
	tmp_exeerr="${TMPDIR}/meta4_${name}.exeerr"
	declare -a exec_argv=()
	if [[ "$EXEC_STDIN" != "1" ]]; then
		while IFS= read -r line || [[ -n "$line" ]]; do
			[[ -z "${line//[$'\t ']/}" ]] && continue
			read -r -a row <<<"$line"
			exec_argv+=("${row[@]}")
		done <"$infile"
	fi
	set +e
	if [[ "$EXEC_STDIN" == "1" ]]; then
		lli "$tmp_ll" <"$infile" >"$tmp_exeout" 2>"$tmp_exeerr"
	else
		lli "$tmp_ll" "${exec_argv[@]}" </dev/null >"$tmp_exeout" 2>"$tmp_exeerr"
	fi
	ex_ec=$?
	set -e
	if [[ "$ex_ec" -ne 0 ]]; then
		FAIL_EX=$((FAIL_EX + 1))
		echo "EXEC FAIL (lli exit ${ex_ec}): $name"
		head -n 5 "$tmp_exeerr" || true
		head -n 5 "$tmp_exeout" || true
		continue
	fi
	if diff -q "$outfile" "$tmp_exeout" >/dev/null 2>&1; then
		PASS_EX=$((PASS_EX + 1))
	else
		FAIL_EX=$((FAIL_EX + 1))
		echo "EXEC FAIL (stdout diff): $name"
		diff -u "$outfile" "$tmp_exeout" | head -n 15 || true
	fi
done

echo "Phase C exec (.in/.out): PASS=${PASS_EX} FAIL=${FAIL_EX} TOTAL=${TOTAL_EX}"

## Phase D — compiler flags (stdin tests)
MINJAVA="${ROOT}/testes/meta4/ok_minimal.java"
if [[ ! -f "$MINJAVA" ]]; then
	echo "Phase D FAIL: missing ${MINJAVA}" >&2
	exit 1
fi
tmp_d="${TMPDIR}/meta4_flag_smoke.err"
set +e
"$JUC" -l <"$MINJAVA" >/dev/null 2>"$tmp_d"
ec_l=$?
set -e
if [[ "$ec_l" -ne 0 ]] || [[ -s "$tmp_d" ]]; then
	echo "Phase D FAIL: -l (exit=${ec_l})" >&2
	[[ -s "$tmp_d" ]] && cat "$tmp_d" >&2
	exit 1
fi

set +e
"$JUC" -e2 <"$MINJAVA" >/dev/null 2>"$tmp_d"
ec_e2=$?
set -e
if [[ "$ec_e2" -ne 0 ]] || [[ -s "$tmp_d" ]]; then
	echo "Phase D FAIL: -e2 (exit=${ec_e2})" >&2
	[[ -s "$tmp_d" ]] && cat "$tmp_d" >&2
	exit 1
fi

set +e
"$JUC" -t <"$MINJAVA" >/tmp/meta4_ast_dump.out 2>"$tmp_d"
ec_t=$?
set -e
if [[ "$ec_t" -ne 0 ]] || [[ -s "$tmp_d" ]]; then
	echo "Phase D FAIL: -t (exit=${ec_t})" >&2
	[[ -s "$tmp_d" ]] && cat "$tmp_d" >&2
	exit 1
fi

echo "Phase D flags: OK (-l, -e2, -t on ok_minimal.java)"

ec=0
if [[ "$FAIL_IR" -gt 0 ]]; then ec=1; fi
if [[ "$STRICT_LLI" == "1" ]] && [[ "$FAIL_LLI" -gt 0 ]]; then ec=1; fi
if [[ "$STRICT_EXEC" == "1" ]] && [[ "$FAIL_EX" -gt 0 ]]; then ec=1; fi

exit "$ec"
