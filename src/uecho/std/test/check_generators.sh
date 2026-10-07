#!/bin/bash
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
# Checks objects_mra.pl and manufacturers.pl against small fixtures, so that a
# generator change or an MRA / manufacturer code list update can be verified
# without the full source data:
#
#   src/uecho/std/test/check_generators.sh
#
# It checks that
#   - the fixtures generate the expected outputs (*.expected),
#   - the outputs compile with and without UECHO_DATABASE_NONE,
#   - the generators fail on input they do not understand.
#
# After an intended generator change, update the expected outputs with:
#
#   src/uecho/std/test/check_generators.sh --update

set -u

TEST_DIR=$(cd "$(dirname "$0")" && pwd)
STD_DIR=$(cd "$TEST_DIR/.." && pwd)
ROOT_DIR=$(cd "$STD_DIR/../../.." && pwd)
CC=${CC:-cc}

WORK_DIR=$(mktemp -d)
trap 'rm -rf "$WORK_DIR"' EXIT

failures=0

fail() {
  echo "FAIL: $*"
  failures=$((failures + 1))
}

pass() {
  echo "ok: $*"
}

generate() {
  perl "$STD_DIR/objects_mra.pl" "$TEST_DIR/mra" > "$WORK_DIR/objects_mra.c" || return 1
  perl "$STD_DIR/manufacturers.pl" "$TEST_DIR/manufacturers.csv" > "$WORK_DIR/manufacturers.c" 2> "$WORK_DIR/manufacturers.err" || return 1
}

if ! generate; then
  echo "FAIL: generators failed on the fixtures"
  exit 1
fi

if [ "${1:-}" = "--update" ]; then
  cp "$WORK_DIR/objects_mra.c" "$TEST_DIR/objects_mra.c.expected"
  cp "$WORK_DIR/manufacturers.c" "$TEST_DIR/manufacturers.c.expected"
  echo "updated expected outputs"
  exit 0
fi

# Expected outputs

for name in objects_mra.c manufacturers.c; do
  if diff -u "$TEST_DIR/$name.expected" "$WORK_DIR/$name"; then
    pass "$name matches $name.expected"
  else
    fail "$name differs from $name.expected"
  fi
done

if grep -q "duplicated manufacturer code 0x00000B" "$WORK_DIR/manufacturers.err"; then
  pass "duplicated manufacturer code is reported"
else
  fail "duplicated manufacturer code is not reported"
fi

# Compilation

for name in objects_mra.c manufacturers.c "$STD_DIR/objects_mra.c" "$STD_DIR/manufacturers.c"; do
  case "$name" in
  /*) src="$name" ;;
  *) src="$WORK_DIR/$name" ;;
  esac
  for defs in "" "-DUECHO_DATABASE_NONE"; do
    if $CC -Wall -Werror -fsyntax-only -I "$ROOT_DIR/include" -I "$ROOT_DIR/src" $defs "$src"; then
      pass "$src compiles ${defs:-(full)}"
    else
      fail "$src does not compile ${defs:-(full)}"
    fi
  done
done

# Invalid input must fail instead of generating a wrong database.

expect_objects_failure() {
  local desc="$1"
  local file="$2"
  local from="$3"
  local to="$4"
  rm -rf "$WORK_DIR/bad"
  cp -r "$TEST_DIR/mra" "$WORK_DIR/bad"
  if [ -n "$from" ]; then
    perl -0pi -e "s/\Q$from\E/$to/" "$WORK_DIR/bad/$file"
  else
    rm -f "$WORK_DIR/bad/$file"
  fi
  if perl "$STD_DIR/objects_mra.pl" "$WORK_DIR/bad" > /dev/null 2> "$WORK_DIR/bad.err"; then
    fail "objects_mra.pl accepted $desc"
  else
    pass "objects_mra.pl rejects $desc: $(head -n 1 "$WORK_DIR/bad.err")"
  fi
}

expect_objects_failure "an unknown access rule" "mraData/devices/0x0291.json" '"get": "optional"' '"get": "required_x"'
expect_objects_failure "a missing access rule" "mraData/devices/0x0291.json" '"set": "optional", ' ''
expect_objects_failure "an invalid EPC" "mraData/devices/0x0291.json" '"epc": "0xB0"' '"epc": "0xBG"'
expect_objects_failure "an EPC below 0x80" "mraData/devices/0x0291.json" '"epc": "0xB0"' '"epc": "0x70"'
expect_objects_failure "an invalid EOJ" "mraData/devices/0x0291.json" '"eoj": "0x0291"' '"eoj": "0x291"'
expect_objects_failure "a missing class name" "mraData/devices/0x0291.json" '"en": "Mono functional lighting"' '"fr": "Mono functional lighting"'
expect_objects_failure "broken JSON" "mraData/devices/0x0291.json" '"elProperties": [' '"elProperties": [,'
expect_objects_failure "a missing node profile" "mraData/nodeProfile/0x0EF0.json" "" ""
expect_objects_failure "a missing super class" "mraData/superClass/0x0000.json" "" ""

printf 'Code,Name\nxyz,No codes\n' > "$WORK_DIR/empty.csv"
if perl "$STD_DIR/manufacturers.pl" "$WORK_DIR/empty.csv" > /dev/null 2>&1; then
  fail "manufacturers.pl accepted a list without codes"
else
  pass "manufacturers.pl rejects a list without codes"
fi

if [ "$failures" -ne 0 ]; then
  echo "$failures check(s) failed"
  exit 1
fi
echo "all checks passed"
