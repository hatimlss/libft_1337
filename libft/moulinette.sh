#!/usr/bin/env bash
# ----------------------------------------------------------------------------
#  libft moulinette
#  Usage: put moulinette.sh and moul_main.c next to your ft_*.c and libft.h,
#         then run:   bash moulinette.sh
#  Options (env):  CC=clang bash moulinette.sh     (change compiler)
#                  ASAN_OPTIONS=detect_leaks=0 bash moulinette.sh   (no leak check)
# ----------------------------------------------------------------------------
cd "$(dirname "$0")" || exit 1

CC="${CC:-cc}"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

PART1="ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint ft_strlen ft_memset ft_bzero
ft_memcpy ft_memmove ft_strlcpy ft_strlcat ft_toupper ft_tolower ft_strchr ft_strrchr
ft_strncmp ft_memchr ft_memcmp ft_strnstr ft_atoi ft_calloc ft_strdup"
PART2="ft_substr ft_strjoin ft_strtrim ft_split ft_itoa ft_strmapi ft_striteri
ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd"
BONUS="ft_lstnew ft_lstadd_front ft_lstsize ft_lstlast ft_lstadd_back ft_lstdelone
ft_lstclear ft_lstiter ft_lstmap"

G=$'\033[32m'; R=$'\033[31m'; Y=$'\033[33m'; B=$'\033[1m'; N=$'\033[0m'
SAN="-fsanitize=address,undefined -fno-sanitize-recover=undefined"

[ -f libft.h ]     || { echo "${R}libft.h not found in $(pwd)${N}"; exit 1; }
[ -f moul_main.c ] || { echo "${R}moul_main.c not found in $(pwd)${N}"; exit 1; }

# ---------------------------------------------------------------- 1. files
echo "${B}== 1. Required files ==${N}"
missing=0
for n in $PART1 $PART2 $BONUS; do
	if [ ! -f "$n.c" ]; then
		echo "  ${Y}missing${N} $n.c"
		missing=$((missing + 1))
	fi
done
[ $missing -eq 0 ] && echo "  ${G}all 43 files present${N}"

# ------------------------------------------------------------- 2. norminette
if command -v norminette >/dev/null 2>&1; then
	echo
	echo "${B}== 2. Norminette ==${N}"
	out="$(norminette ft_*.c libft.h 2>&1 | grep -v ': OK!$')"
	if [ -z "$out" ]; then echo "  ${G}Norm OK${N}"; else echo "$out"; fi
else
	echo
	echo "${B}== 2. Norminette ==${N}"
	echo "  ${Y}norminette not installed, skipped${N}"
fi

# --------------------------------------------------------------- 3. compile
echo
echo "${B}== 3. Compilation (-Wall -Wextra -Werror) ==${N}"
cfail=0
for f in ft_*.c; do
	n="${f%.c}"
	extra=""
	# a leftover main() is renamed so it can't clash with the tester
	if grep -Eq '(^|[^_[:alnum:]])main[[:space:]]*\(' "$f"; then
		echo "  ${Y}WARN${N} $f contains a main(): delete it before submitting"
		extra="-Wno-error=return-type"
	fi
	if $CC -Wall -Wextra -Werror $extra -g $SAN -Dmain=moul_ignored_main_$n -c "$f" -o "$TMP/$n.o" 2>"$TMP/$n.err"; then
		# the object must really define the function (nm: T = defined, global)
		if ! nm "$TMP/$n.o" 2>/dev/null | grep -Eq " T _?$n\$"; then
			cfail=$((cfail + 1))
			echo "  ${R}KO${N} $f compiles but does not define $n()"
			echo "      (empty/commented-out file, misspelled name, or declared static?)"
			rm -f "$TMP/$n.o"
		fi
	else
		cfail=$((cfail + 1))
		echo "  ${R}KO${N} $f"
		sed 's/^/      /' "$TMP/$n.err" | head -n 12
	fi
done
[ $cfail -eq 0 ] && echo "  ${G}all files compile${N}"

# ------------------------------------------------------------------ 4. test
echo
echo "${B}== 4. Tests (sanitizers on, leaks checked) ==${N}"
defs=""
for n in $PART1 $PART2 $BONUS; do
	if [ -f "$TMP/$n.o" ]; then defs="$defs -DHAVE_$n=1"; else defs="$defs -DHAVE_$n=0"; fi
done

if ! $CC -g $SAN -Wall -Wextra -Wno-unused-function -Wno-unused-variable \
	-Wno-misleading-indentation -I. $defs moul_main.c "$TMP"/*.o -o "$TMP/tester" 2>"$TMP/link.err"; then
	echo "${R}Could not build the tester:${N}"
	head -n 20 "$TMP/link.err"
	echo
	echo "Hint: 'undefined reference' means a function calls a helper that lives in a"
	echo "file not named ft_*.c, or libft.h is missing a prototype."
	exit 1
fi

"$TMP/tester"
status=$?
echo
if [ $status -eq 0 ] && [ $missing -eq 0 ] && [ $cfail -eq 0 ]; then
	echo "${G}${B}Everything passed.${N}"
fi
exit $status
