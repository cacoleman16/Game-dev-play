#!/bin/bash
# Syntax/type-check pokefirered C files with the host gcc (front end only, no
# code generation, nothing from the decomp is executed).
# Usage: syntax_check.sh <pokefirered root> <generated-headers dir> file.c...
root=$1; gen=$2; shift 2
cd "$root" || exit 1
status=0
for f in "$@"; do
    out=$(gcc -m32 -fsyntax-only -std=gnu89 -D__INTELLISENSE__ -DFIRERED -DREVISION=0 -DENGLISH -DMODERN=0 \
        -iquote include -iquote "$gen" -iquote gflib -Wimplicit -Wparentheses -Wdeclaration-after-statement -Werror=implicit-function-declaration \
        -Wno-builtin-declaration-mismatch -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-attributes \
        -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-overflow -Wno-multichar \
        "$f" 2>&1 | grep -v -E "unknown escape sequence|initializer-string for array of .unsigned char. is too long|^ +[0-9]+ \\||^ +\\||In file included from|^ +from |: note: |In function|At top level")
    if [ -n "$out" ]; then
        echo "=== $f"; echo "$out" | grep -E "error|warning" | head -40; status=1
    else
        echo "ok  $f"
    fi
done
exit $status
