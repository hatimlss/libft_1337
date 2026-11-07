/* ************************************************************************** */
/*   moul_main.c - libft tester. Do not run directly: use `bash moulinette.sh` */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "libft.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YEL   "\033[33m"
#define BOLD  "\033[1m"
#define RST   "\033[0m"

#ifdef __linux__
__attribute__((used)) const char *__asan_default_options(void)
{
	return ("exitcode=3:detect_leaks=1");
}
#endif

/* ---------------------------- harness core ------------------------------ */

static struct { int fails; char msg[2048]; } *g_sh;

__attribute__((format(printf, 1, 2)))
static void fail(const char *fmt, ...)
{
	va_list	ap;
	size_t	len;

	g_sh->fails++;
	len = strlen(g_sh->msg);
	if (g_sh->fails > 4 || len > sizeof(g_sh->msg) - 300)
		return ;
	va_start(ap, fmt);
	vsnprintf(g_sh->msg + len, sizeof(g_sh->msg) - len, fmt, ap);
	va_end(ap);
}

#define CHECKF(c, fmt, ...) do { if (!(c)) \
	fail("  - line %d: " fmt "\n", __LINE__, ##__VA_ARGS__); } while (0)
#define CHECK(c) CHECKF(c, "%s", #c)
#define SGN(x) (((x) > 0) - ((x) < 0))

static const char *S(const char *s) { return (s ? s : "(null)"); }

/* ------------------- reference versions (not in glibc) ------------------ */

static size_t ref_strlcpy(char *d, const char *s, size_t n)
{
	size_t	i;

	i = 0;
	if (n)
	{
		while (s[i] && i + 1 < n)
		{
			d[i] = s[i];
			i++;
		}
		d[i] = 0;
	}
	return (strlen(s));
}

static size_t ref_strlcat(char *d, const char *s, size_t n)
{
	size_t	dl;
	size_t	i;

	dl = 0;
	while (dl < n && d[dl])
		dl++;
	if (dl == n)
		return (n + strlen(s));
	i = 0;
	while (s[i] && dl + i + 1 < n)
	{
		d[dl + i] = s[i];
		i++;
	}
	d[dl + i] = 0;
	return (dl + strlen(s));
}

static char *ref_strnstr(const char *h, const char *n, size_t len)
{
	size_t	nl;
	size_t	i;

	nl = strlen(n);
	if (!nl)
		return ((char *)h);
	i = 0;
	while (h[i] && i + nl <= len)
	{
		if (!strncmp(h + i, n, nl))
			return ((char *)h + i);
		i++;
	}
	return (NULL);
}

/* ================================ PART 1 ================================= */

#define CTYPE(f, ref) \
static void t_##f(void) { int c; for (c = 0; c < 256; c++) \
	CHECKF(!!f(c) == !!ref(c), #f "(%d) = %d, libc says %d", c, !!f(c), !!ref(c)); }

#if HAVE_ft_isalpha
CTYPE(ft_isalpha, isalpha)
#endif
#if HAVE_ft_isdigit
CTYPE(ft_isdigit, isdigit)
#endif
#if HAVE_ft_isalnum
CTYPE(ft_isalnum, isalnum)
#endif
#if HAVE_ft_isprint
CTYPE(ft_isprint, isprint)
#endif

#if HAVE_ft_isascii
static void t_ft_isascii(void)
{
	int	c;

	for (c = -1; c < 300; c++)
		CHECKF(!!ft_isascii(c) == !!isascii(c), "ft_isascii(%d) = %d, libc says %d",
			c, ft_isascii(c), isascii(c));
}
#endif

#if HAVE_ft_toupper
static void t_ft_toupper(void)
{
	int	c;

	for (c = 0; c < 256; c++)
		CHECKF(ft_toupper(c) == toupper(c), "ft_toupper(%d) = %d, libc says %d",
			c, ft_toupper(c), toupper(c));
}
#endif

#if HAVE_ft_tolower
static void t_ft_tolower(void)
{
	int	c;

	for (c = 0; c < 256; c++)
		CHECKF(ft_tolower(c) == tolower(c), "ft_tolower(%d) = %d, libc says %d",
			c, ft_tolower(c), tolower(c));
}
#endif

#if HAVE_ft_strlen
static void t_ft_strlen(void)
{
	char		big[2001];
	const char	*t[] = {"", "a", "hello", "hello world, 42!", "with\0hidden", big};
	size_t		i;

	memset(big, 'x', 2000);
	big[2000] = 0;
	for (i = 0; i < sizeof(t) / sizeof(*t); i++)
		CHECKF(ft_strlen(t[i]) == strlen(t[i]), "case %zu: got %zu, expected %zu",
			i, ft_strlen(t[i]), strlen(t[i]));
}
#endif

#if HAVE_ft_memset
static void t_ft_memset(void)
{
	unsigned char	a[64];
	unsigned char	b[64];
	int				cs[] = {0, 'a', 255, 321, -1};
	size_t			ns[] = {0, 1, 7, 40};
	size_t			i;
	size_t			j;
	void			*r;

	for (i = 0; i < 5; i++)
		for (j = 0; j < 4; j++)
		{
			memset(a, 'X', 64);
			memset(b, 'X', 64);
			r = ft_memset(a + 5, cs[i], ns[j]);
			memset(b + 5, cs[i], ns[j]);
			CHECKF(r == a + 5, "wrong return value (c=%d n=%zu)", cs[i], ns[j]);
			CHECKF(!memcmp(a, b, 64), "buffer differs (c=%d n=%zu)", cs[i], ns[j]);
		}
}
#endif

#if HAVE_ft_bzero
static void t_ft_bzero(void)
{
	unsigned char	a[64];
	unsigned char	b[64];
	size_t			ns[] = {0, 1, 10, 50};
	size_t			j;

	for (j = 0; j < 4; j++)
	{
		memset(a, 'X', 64);
		memset(b, 'X', 64);
		ft_bzero(a + 3, ns[j]);
		bzero(b + 3, ns[j]);
		CHECKF(!memcmp(a, b, 64), "buffer differs (n=%zu)", ns[j]);
	}
}
#endif

#if HAVE_ft_memcpy
static void t_ft_memcpy(void)
{
	char			src[] = "0123456789abcdef";
	char			zer[] = "ab\0cd\0ef";
	unsigned char	a[32];
	unsigned char	b[32];
	size_t			ns[] = {0, 1, 5, 16};
	size_t			j;
	void			*r;

	for (j = 0; j < 4; j++)
	{
		memset(a, 'X', 32);
		memset(b, 'X', 32);
		r = ft_memcpy(a + 3, src, ns[j]);
		memcpy(b + 3, src, ns[j]);
		CHECKF(r == a + 3, "wrong return value (n=%zu)", ns[j]);
		CHECKF(!memcmp(a, b, 32), "buffer differs (n=%zu)", ns[j]);
	}
	memset(a, 'X', 32);
	memset(b, 'X', 32);
	ft_memcpy(a, zer, 8);
	memcpy(b, zer, 8);
	CHECK(!memcmp(a, b, 32));
}
#endif

#if HAVE_ft_memmove
static void t_ft_memmove(void)
{
	static const struct { int d, s; size_t n; } c[] = {
		{2, 0, 10}, {0, 2, 10}, {0, 0, 10}, {5, 5, 0}, {10, 0, 5},
		{1, 0, 30}, {0, 1, 30}, {0, 20, 15}, {20, 0, 15}};
	char	a[40];
	char	b[40];
	size_t	i;
	void	*r;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
	{
		strcpy(a, "abcdefghijklmnopqrstuvwxyz0123456789");
		strcpy(b, a);
		r = ft_memmove(a + c[i].d, a + c[i].s, c[i].n);
		memmove(b + c[i].d, b + c[i].s, c[i].n);
		CHECKF(r == a + c[i].d, "wrong return value (dst+%d src+%d n=%zu)",
			c[i].d, c[i].s, c[i].n);
		CHECKF(!memcmp(a, b, 37), "dst+%d src+%d n=%zu: got \"%s\" expected \"%s\"",
			c[i].d, c[i].s, c[i].n, a, b);
	}
}
#endif

#if HAVE_ft_strlcpy
static void t_ft_strlcpy(void)
{
	const char	*src[] = {"hello world", ""};
	size_t		ns[] = {0, 1, 2, 5, 11, 12, 20};
	char		a[32];
	char		b[32];
	size_t		i;
	size_t		j;
	size_t		r1;
	size_t		r2;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 7; j++)
		{
			memset(a, 'Z', 32);
			memset(b, 'Z', 32);
			r1 = ft_strlcpy(a, src[i], ns[j]);
			r2 = ref_strlcpy(b, src[i], ns[j]);
			CHECKF(r1 == r2, "src=\"%s\" size=%zu: returned %zu, expected %zu",
				src[i], ns[j], r1, r2);
			CHECKF(!memcmp(a, b, 32), "src=\"%s\" size=%zu: dst differs", src[i], ns[j]);
		}
}
#endif

#if HAVE_ft_strlcat
static void t_ft_strlcat(void)
{
	const char	*dst[] = {"abc", "", "abcdefghij"};
	const char	*src[] = {"defgh", ""};
	size_t		ns[] = {0, 1, 3, 4, 5, 8, 11, 12, 20};
	char		a[32];
	char		b[32];
	size_t		i;
	size_t		j;
	size_t		k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 2; j++)
			for (k = 0; k < 9; k++)
			{
				memset(a, 'Z', 32);
				memset(b, 'Z', 32);
				strcpy(a, dst[i]);
				strcpy(b, dst[i]);
				CHECKF(ft_strlcat(a, src[j], ns[k]) == ref_strlcat(b, src[j], ns[k]),
					"dst=\"%s\" src=\"%s\" size=%zu: wrong return value", dst[i], src[j], ns[k]);
				CHECKF(!memcmp(a, b, 32), "dst=\"%s\" src=\"%s\" size=%zu: dst differs",
					dst[i], src[j], ns[k]);
			}
}
#endif

#if HAVE_ft_strchr
static void t_ft_strchr(void)
{
	const char	*s[] = {"hello", "", "aaa", "hello world"};
	int			cs[] = {'h', 'l', 'o', 0, 'z', 'w', 'l' + 256};
	size_t		i;
	size_t		j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 7; j++)
			CHECKF(ft_strchr(s[i], cs[j]) == strchr(s[i], cs[j]),
				"ft_strchr(\"%s\", %d) differs from libc", s[i], cs[j]);
}
#endif

#if HAVE_ft_strrchr
static void t_ft_strrchr(void)
{
	const char	*s[] = {"hello", "", "aaa", "hello world"};
	int			cs[] = {'h', 'l', 'o', 0, 'z', 'w', 'l' + 256};
	size_t		i;
	size_t		j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 7; j++)
			CHECKF(ft_strrchr(s[i], cs[j]) == strrchr(s[i], cs[j]),
				"ft_strrchr(\"%s\", %d) differs from libc", s[i], cs[j]);
}
#endif

#if HAVE_ft_strncmp
static void t_ft_strncmp(void)
{
	static const struct { const char *a, *b; size_t n; } c[] = {
		{"abc", "abc", 3}, {"abc", "abd", 3}, {"abd", "abc", 3}, {"abc", "abd", 2},
		{"abc", "ab", 3}, {"ab", "abc", 3}, {"", "", 1}, {"a", "", 1}, {"", "a", 1},
		{"abc", "abc", 0}, {"abc", "abc", 100}, {"test\0abc", "test\0xyz", 8},
		{"\xff", "\x01", 1}, {"\x01", "\xff", 1}};
	size_t	i;
	int		r1;
	int		r2;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
	{
		r1 = ft_strncmp(c[i].a, c[i].b, c[i].n);
		r2 = strncmp(c[i].a, c[i].b, c[i].n);
		CHECKF(SGN(r1) == SGN(r2), "case %zu (\"%s\",\"%s\",%zu): got %d, libc %d",
			i, c[i].a, c[i].b, c[i].n, r1, r2);
	}
}
#endif

#if HAVE_ft_memchr
static void t_ft_memchr(void)
{
	unsigned char	b[] = {'a', 'b', 0, 'c', 'd', 0, 255, 'e'};
	static const struct { int c; size_t n; } t[] = {
		{'c', 8}, {0, 8}, {'c', 3}, {'a', 0}, {'a' + 256, 8}, {255, 8},
		{'e', 7}, {'e', 8}, {'z', 8}, {0, 2}};
	size_t	i;

	for (i = 0; i < sizeof(t) / sizeof(*t); i++)
		CHECKF(ft_memchr(b, t[i].c, t[i].n) == memchr(b, t[i].c, t[i].n),
			"ft_memchr(buf, %d, %zu) differs from libc", t[i].c, t[i].n);
}
#endif

#if HAVE_ft_memcmp
static void t_ft_memcmp(void)
{
	static const struct { const void *a, *b; size_t n; } c[] = {
		{"abc", "abd", 3}, {"abd", "abc", 3}, {"a\0b", "a\0c", 3}, {"abc", "abc", 0},
		{"\xff", "\x01", 1}, {"\x01", "\xff", 1}, {"abc", "abc", 3}, {"ab\0c", "ab\0c", 4}};
	size_t	i;
	int		r1;
	int		r2;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
	{
		r1 = ft_memcmp(c[i].a, c[i].b, c[i].n);
		r2 = memcmp(c[i].a, c[i].b, c[i].n);
		CHECKF(SGN(r1) == SGN(r2), "case %zu (n=%zu): got %d, libc %d", i, c[i].n, r1, r2);
	}
}
#endif

#if HAVE_ft_strnstr
static void t_ft_strnstr(void)
{
	static const struct { const char *h, *n; size_t len; } c[] = {
		{"hello world", "world", 11}, {"hello world", "world", 10}, {"hello", "", 0},
		{"hello", "", 5}, {"", "a", 5}, {"", "", 0}, {"aaabc", "abc", 5},
		{"aaabc", "abc", 4}, {"abc", "abcd", 10}, {"hello", "lo", 5}, {"hello", "lo", 4},
		{"hello", "hello", 5}, {"hello", "hello", 4}, {"hello", "ll", 100}, {"abc", "x", 3}};
	size_t	i;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
		CHECKF(ft_strnstr(c[i].h, c[i].n, c[i].len) == ref_strnstr(c[i].h, c[i].n, c[i].len),
			"ft_strnstr(\"%s\", \"%s\", %zu): wrong result", c[i].h, c[i].n, c[i].len);
}
#endif

#if HAVE_ft_atoi
static void t_ft_atoi(void)
{
	const char	*s[] = {"0", "42", "-42", "+42", "   42", "\t\n\v\f\r 42", "  -42abc",
		"abc", "", "--42", "+-42", "-+42", "2147483647", "-2147483648", "0042",
		"42 42", "- 42", "+", "-", "12abc34", "\n-7"};
	size_t		i;

	for (i = 0; i < sizeof(s) / sizeof(*s); i++)
		CHECKF(ft_atoi(s[i]) == atoi(s[i]), "ft_atoi(\"%s\") = %d, libc says %d",
			s[i], ft_atoi(s[i]), atoi(s[i]));
}
#endif

#if HAVE_ft_calloc
static void t_ft_calloc(void)
{
	int		*p;
	char	*q;
	void	*z;
	void	*o;
	int		i;

	p = ft_calloc(5, sizeof(int));
	CHECK(p != NULL);
	if (p)
	{
		for (i = 0; i < 5; i++)
			CHECKF(p[i] == 0, "p[%d] = %d (not zeroed)", i, p[i]);
		p[4] = 42;
	}
	free(p);
	q = ft_calloc(100, 10);
	CHECK(q != NULL);
	if (q)
		for (i = 0; i < 1000; i++)
			CHECKF(q[i] == 0, "byte %d not zeroed", i);
	free(q);
	z = ft_calloc(0, 0);
	CHECKF(z != NULL, "ft_calloc(0, 0) returned NULL (libc returns a valid pointer)");
	free(z);
	o = ft_calloc(SIZE_MAX, 2);
	CHECKF(o == NULL, "ft_calloc(SIZE_MAX, 2) should return NULL (overflow)");
	free(o);
}
#endif

#if HAVE_ft_strdup
static void t_ft_strdup(void)
{
	char		big[1001];
	const char	*t[] = {"", "a", "hello world", big};
	char		*r;
	size_t		i;

	memset(big, 'y', 1000);
	big[1000] = 0;
	for (i = 0; i < 4; i++)
	{
		r = ft_strdup(t[i]);
		CHECKF(r != NULL, "case %zu returned NULL", i);
		if (r)
		{
			CHECKF(r != t[i], "case %zu: returned the same pointer (no copy)", i);
			CHECKF(!strcmp(r, t[i]), "case %zu: content differs", i);
		}
		free(r);
	}
}
#endif

/* ================================ PART 2 ================================= */

#if HAVE_ft_substr
static void t_ft_substr(void)
{
	static const struct { const char *s; unsigned int st; size_t len; const char *e; } c[] = {
		{"hello world", 6, 5, "world"}, {"hello world", 0, 5, "hello"},
		{"hello world", 6, 100, "world"}, {"hello world", 11, 5, ""},
		{"hello world", 100, 5, ""}, {"hello world", 3, 0, ""}, {"", 0, 5, ""},
		{"hello", 0, 100, "hello"}, {"hello", 4, 1, "o"}, {"hello", UINT_MAX, 5, ""},
		{"hello", 1, 3, "ell"}};
	char	*r;
	size_t	i;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
	{
		r = ft_substr(c[i].s, c[i].st, c[i].len);
		CHECKF(r && !strcmp(r, c[i].e), "ft_substr(\"%s\", %u, %zu) = \"%s\", expected \"%s\"",
			c[i].s, c[i].st, c[i].len, S(r), c[i].e);
		free(r);
	}
}
#endif

#if HAVE_ft_strjoin
static void t_ft_strjoin(void)
{
	static const char	*c[][3] = {{"hello", " world", "hello world"}, {"", "", ""},
		{"a", "", "a"}, {"", "b", "b"}, {"abc", "def", "abcdef"}};
	char	*r;
	size_t	i;

	for (i = 0; i < 5; i++)
	{
		r = ft_strjoin(c[i][0], c[i][1]);
		CHECKF(r && !strcmp(r, c[i][2]), "ft_strjoin(\"%s\", \"%s\") = \"%s\", expected \"%s\"",
			c[i][0], c[i][1], S(r), c[i][2]);
		free(r);
	}
}
#endif

#if HAVE_ft_strtrim
static void t_ft_strtrim(void)
{
	static const char	*c[][3] = {{"  hello  ", " ", "hello"}, {"xxhixx", "x", "hi"},
		{"abc", "", "abc"}, {"xxxx", "x", ""}, {"", "x", ""}, {"   ", " ", ""},
		{"abcba", "ab", "c"}, {"\t\n hi \n\t", " \t\n", "hi"}, {"hello", " ", "hello"},
		{"  a b  ", " ", "a b"}, {"abc", "xyz", "abc"}};
	char	*r;
	size_t	i;

	for (i = 0; i < sizeof(c) / sizeof(*c); i++)
	{
		r = ft_strtrim(c[i][0], c[i][1]);
		CHECKF(r && !strcmp(r, c[i][2]), "ft_strtrim(\"%s\", \"%s\") = \"%s\", expected \"%s\"",
			c[i][0], c[i][1], S(r), c[i][2]);
		free(r);
	}
}
#endif

#if HAVE_ft_split
static void chk_split(int line, char **r, const char **e, int n)
{
	int	i;

	if (!r)
	{
		fail("  - line %d: returned NULL\n", line);
		return ;
	}
	for (i = 0; i < n; i++)
	{
		if (!r[i])
		{
			fail("  - line %d: only %d strings, expected %d\n", line, i, n);
			break ;
		}
		if (strcmp(r[i], e[i]))
			fail("  - line %d: [%d] = \"%s\", expected \"%s\"\n", line, i, r[i], e[i]);
	}
	if (i == n && r[n])
		fail("  - line %d: more than %d strings or missing NULL terminator\n", line, n);
	for (i = 0; i < n && r[i]; i++)
		free(r[i]);
	free(r);
}

static void t_ft_split(void)
{
	const char	*e1[] = {"hello", "world", "foo"};
	const char	*e2[] = {"a", "b"};
	const char	*e3[] = {"abc"};
	const char	*e4[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
	const char	*e5[] = {"he", "o"};

	chk_split(__LINE__, ft_split("hello world foo", ' '), e1, 3);
	chk_split(__LINE__, ft_split("  a  b  ", ' '), e2, 2);
	chk_split(__LINE__, ft_split("", ' '), NULL, 0);
	chk_split(__LINE__, ft_split("     ", ' '), NULL, 0);
	chk_split(__LINE__, ft_split("abc", ' '), e3, 1);
	chk_split(__LINE__, ft_split("abc", '\0'), e3, 1);
	chk_split(__LINE__, ft_split(",a,,b,", ','), e2, 2);
	chk_split(__LINE__, ft_split("a b c d e f g h", ' '), e4, 8);
	chk_split(__LINE__, ft_split("hello", 'l'), e5, 2);
}
#endif

#if HAVE_ft_itoa
static void t_ft_itoa(void)
{
	int		v[] = {0, 1, -1, 9, 10, -10, 42, -42, 100, -100, 1000000, 2147483647,
		-2147483647, INT_MIN};
	char	exp[20];
	char	*r;
	size_t	i;

	for (i = 0; i < sizeof(v) / sizeof(*v); i++)
	{
		snprintf(exp, sizeof(exp), "%d", v[i]);
		r = ft_itoa(v[i]);
		CHECKF(r && !strcmp(r, exp), "ft_itoa(%d) = \"%s\", expected \"%s\"", v[i], S(r), exp);
		free(r);
	}
}
#endif

#if HAVE_ft_strmapi
static char map_shift(unsigned int i, char c) { return (c + (char)i); }
static void t_ft_strmapi(void)
{
	const char	*t[] = {"abcde", "", "a", "hello world"};
	char		exp[32];
	char		*r;
	size_t		i;
	size_t		k;

	for (i = 0; i < 4; i++)
	{
		for (k = 0; k <= strlen(t[i]); k++)
			exp[k] = t[i][k] ? t[i][k] + (char)k : 0;
		r = ft_strmapi(t[i], map_shift);
		CHECKF(r && !strcmp(r, exp), "ft_strmapi(\"%s\") = \"%s\", expected \"%s\"",
			t[i], S(r), exp);
		free(r);
	}
}
#endif

#if HAVE_ft_striteri
static int g_iter_calls;
static void iter_shift(unsigned int i, char *c) { *c += (char)i; g_iter_calls++; }
static void t_ft_striteri(void)
{
	char	a[] = "abcde";
	char	e[] = "acegi";
	char	empty[] = "";

	g_iter_calls = 0;
	ft_striteri(a, iter_shift);
	CHECKF(!strcmp(a, e), "got \"%s\", expected \"%s\"", a, e);
	CHECKF(g_iter_calls == 5, "f called %d times, expected 5", g_iter_calls);
	g_iter_calls = 0;
	ft_striteri(empty, iter_shift);
	CHECKF(g_iter_calls == 0, "f called %d times on empty string", g_iter_calls);
}
#endif

/* ---- put*_fd: output is captured through a pipe ---- */
#if HAVE_ft_putchar_fd || HAVE_ft_putstr_fd || HAVE_ft_putendl_fd || HAVE_ft_putnbr_fd
static void cap_begin(int p[2])
{
	if (pipe(p) < 0)
	{
		fail("  - pipe() failed\n");
		exit(2);
	}
}

static char *cap_end(int p[2], char *buf, size_t sz)
{
	ssize_t	r;

	close(p[1]);
	r = read(p[0], buf, sz - 1);
	close(p[0]);
	buf[r < 0 ? 0 : r] = 0;
	return (buf);
}
#endif

#if HAVE_ft_putchar_fd
static void t_ft_putchar_fd(void)
{
	char	b[64];
	int		p[2];

	cap_begin(p);
	ft_putchar_fd('a', p[1]);
	ft_putchar_fd('\n', p[1]);
	ft_putchar_fd('Z', p[1]);
	CHECKF(!strcmp(cap_end(p, b, 64), "a\nZ"), "wrote \"%s\", expected \"a\\nZ\"", b);
	ft_putchar_fd('a', -1);
}
#endif

#if HAVE_ft_putstr_fd
static void t_ft_putstr_fd(void)
{
	const char	*t[] = {"hello", "", "with\nnewline", "a b c"};
	char		b[64];
	int			p[2];
	size_t		i;

	for (i = 0; i < 4; i++)
	{
		cap_begin(p);
		ft_putstr_fd((char *)t[i], p[1]);
		CHECKF(!strcmp(cap_end(p, b, 64), t[i]), "wrote \"%s\", expected \"%s\"", b, t[i]);
	}
	ft_putstr_fd("x", -1);
}
#endif

#if HAVE_ft_putendl_fd
static void t_ft_putendl_fd(void)
{
	const char	*t[] = {"hello", "", "a b"};
	const char	*e[] = {"hello\n", "\n", "a b\n"};
	char		b[64];
	int			p[2];
	size_t		i;

	for (i = 0; i < 3; i++)
	{
		cap_begin(p);
		ft_putendl_fd((char *)t[i], p[1]);
		CHECKF(!strcmp(cap_end(p, b, 64), e[i]), "wrote \"%s\", expected \"%s\"", b, e[i]);
	}
	ft_putendl_fd("x", -1);
}
#endif

#if HAVE_ft_putnbr_fd
static void t_ft_putnbr_fd(void)
{
	int		v[] = {0, 1, -1, 9, 10, -10, 42, -42, 100, 123456, INT_MAX, INT_MIN};
	char	b[64];
	char	e[20];
	int		p[2];
	size_t	i;

	for (i = 0; i < sizeof(v) / sizeof(*v); i++)
	{
		cap_begin(p);
		ft_putnbr_fd(v[i], p[1]);
		snprintf(e, sizeof(e), "%d", v[i]);
		CHECKF(!strcmp(cap_end(p, b, 64), e), "ft_putnbr_fd(%d) wrote \"%s\"", v[i], b);
	}
	ft_putnbr_fd(42, -1);
}
#endif

/* ================================ BONUS ================================== */

#define HAVE_ANY_LIST (HAVE_ft_lstnew || HAVE_ft_lstadd_front || HAVE_ft_lstsize \
	|| HAVE_ft_lstlast || HAVE_ft_lstadd_back || HAVE_ft_lstdelone \
	|| HAVE_ft_lstclear || HAVE_ft_lstiter || HAVE_ft_lstmap)

#if HAVE_ANY_LIST
static int	g_del;
static char	g_order[64];

static t_list *mk(void *c)
{
	t_list	*n;

	n = malloc(sizeof(t_list));
	n->content = c;
	n->next = NULL;
	return (n);
}

static t_list *chain_str(int n)
{
	t_list	*h = NULL;
	t_list	*t = NULL;
	t_list	*x;
	char	b[16];
	int		i;

	for (i = 0; i < n; i++)
	{
		snprintf(b, sizeof(b), "i%d", i);
		x = mk(strdup(b));
		if (!h)
			h = x;
		else
			t->next = x;
		t = x;
	}
	return (h);
}

static int lsize(t_list *l)
{
	int	n;

	for (n = 0; l; l = l->next)
		n++;
	return (n);
}

static void free_chain(t_list *l, int with_content)
{
	t_list	*n;

	while (l)
	{
		n = l->next;
		if (with_content)
			free(l->content);
		free(l);
		l = n;
	}
}

static void del_count(void *c) { g_del++; free(c); }
#endif

#if HAVE_ft_lstnew
static void t_ft_lstnew(void)
{
	int		x;
	t_list	*n;

	n = ft_lstnew(&x);
	CHECK(n != NULL);
	if (n)
	{
		CHECK(n->content == &x);
		CHECK(n->next == NULL);
	}
	free(n);
	n = ft_lstnew(NULL);
	CHECK(n != NULL);
	if (n)
	{
		CHECK(n->content == NULL);
		CHECK(n->next == NULL);
	}
	free(n);
}
#endif

#if HAVE_ft_lstadd_front
static void t_ft_lstadd_front(void)
{
	t_list	*l = NULL;
	t_list	*a = mk(NULL);
	t_list	*b = mk(NULL);

	ft_lstadd_front(&l, a);
	CHECK(l == a);
	CHECK(a->next == NULL);
	ft_lstadd_front(&l, b);
	CHECK(l == b);
	CHECK(b->next == a);
	free(a);
	free(b);
}
#endif

#if HAVE_ft_lstsize
static void t_ft_lstsize(void)
{
	t_list	*l;

	CHECKF(ft_lstsize(NULL) == 0, "ft_lstsize(NULL) = %d", ft_lstsize(NULL));
	l = chain_str(1);
	CHECKF(ft_lstsize(l) == 1, "size of 1 node = %d", ft_lstsize(l));
	free_chain(l, 1);
	l = chain_str(4);
	CHECKF(ft_lstsize(l) == 4, "size of 4 nodes = %d", ft_lstsize(l));
	free_chain(l, 1);
}
#endif

#if HAVE_ft_lstlast
static void t_ft_lstlast(void)
{
	t_list	*l;
	t_list	*last;

	CHECK(ft_lstlast(NULL) == NULL);
	l = chain_str(1);
	CHECK(ft_lstlast(l) == l);
	free_chain(l, 1);
	l = chain_str(4);
	for (last = l; last->next; last = last->next)
		;
	CHECK(ft_lstlast(l) == last);
	free_chain(l, 1);
}
#endif

#if HAVE_ft_lstadd_back
static void t_ft_lstadd_back(void)
{
	t_list	*l = NULL;
	t_list	*a = mk(NULL);
	t_list	*b = mk(NULL);
	t_list	*c = mk(NULL);

	ft_lstadd_back(&l, a);
	CHECK(l == a);
	ft_lstadd_back(&l, b);
	CHECK(l == a);
	CHECK(a->next == b);
	ft_lstadd_back(&l, c);
	CHECK(b->next == c);
	CHECK(c->next == NULL);
	free(a);
	free(b);
	free(c);
}
#endif

#if HAVE_ft_lstdelone
static void t_ft_lstdelone(void)
{
	t_list	*n;

	n = mk(strdup("x"));
	g_del = 0;
	ft_lstdelone(n, del_count);
	CHECKF(g_del == 1, "del called %d times, expected 1", g_del);
}
#endif

#if HAVE_ft_lstclear
static void t_ft_lstclear(void)
{
	t_list	*l;

	l = chain_str(5);
	g_del = 0;
	ft_lstclear(&l, del_count);
	CHECK(l == NULL);
	CHECKF(g_del == 5, "del called %d times, expected 5", g_del);
	l = NULL;
	g_del = 0;
	ft_lstclear(&l, del_count);
	CHECK(l == NULL);
	CHECK(g_del == 0);
}
#endif

#if HAVE_ft_lstiter
static void it_record(void *c) { strncat(g_order, (char *)c, 3); }
static void t_ft_lstiter(void)
{
	t_list	*l;

	l = chain_str(3);
	g_order[0] = 0;
	ft_lstiter(l, it_record);
	CHECKF(!strcmp(g_order, "i0i1i2"), "visit order \"%s\", expected \"i0i1i2\"", g_order);
	free_chain(l, 1);
	ft_lstiter(NULL, it_record);
}
#endif

#if HAVE_ft_lstmap
static void *map_up(void *c)
{
	char	*s;

	s = strdup((char *)c);
	s[0] = toupper((unsigned char)s[0]);
	return (s);
}

static void t_ft_lstmap(void)
{
	t_list	*src;
	t_list	*res;
	t_list	*a;
	t_list	*b;
	int		n;

	CHECK(ft_lstmap(NULL, map_up, del_count) == NULL);
	for (n = 1; n <= 5; n += 4)
	{
		src = chain_str(n);
		res = ft_lstmap(src, map_up, del_count);
		CHECKF(lsize(res) == n, "n=%d: result has %d nodes", n, lsize(res));
		for (a = src, b = res; a && b; a = a->next, b = b->next)
		{
			CHECKF(a != b && a->content != b->content, "n=%d: shares memory with source", n);
			CHECKF(((char *)b->content)[0] == 'I', "n=%d: f not applied", n);
			CHECKF(!strcmp((char *)a->content + 1, (char *)b->content + 1), "n=%d: bad content", n);
			CHECKF(((char *)a->content)[0] == 'i', "n=%d: source was modified", n);
		}
		g_del = 0;
		ft_lstclear(&res, del_count);
		free_chain(src, 1);
	}
}
#endif

/* ================================ runner ================================== */

typedef struct { const char *name; void (*fn)(void); int have; } t_test;

#define ENTRY(n) ENTRY2(n, HAVE_##n)
#define ENTRY2(n, h) ENTRY3(n, h)
#define ENTRY3(n, h) ENTRY_##h(n)
#define ENTRY_1(n) {#n, t_##n, 1},
#define ENTRY_0(n) {#n, NULL, 0},
#define HEAD(s) {s, NULL, -1},

static const t_test g_tests[] = {
	HEAD("PART 1 - libc")
	ENTRY(ft_isalpha) ENTRY(ft_isdigit) ENTRY(ft_isalnum) ENTRY(ft_isascii)
	ENTRY(ft_isprint) ENTRY(ft_strlen) ENTRY(ft_memset) ENTRY(ft_bzero)
	ENTRY(ft_memcpy) ENTRY(ft_memmove) ENTRY(ft_strlcpy) ENTRY(ft_strlcat)
	ENTRY(ft_toupper) ENTRY(ft_tolower) ENTRY(ft_strchr) ENTRY(ft_strrchr)
	ENTRY(ft_strncmp) ENTRY(ft_memchr) ENTRY(ft_memcmp) ENTRY(ft_strnstr)
	ENTRY(ft_atoi) ENTRY(ft_calloc) ENTRY(ft_strdup)
	HEAD("PART 2 - additional functions")
	ENTRY(ft_substr) ENTRY(ft_strjoin) ENTRY(ft_strtrim) ENTRY(ft_split)
	ENTRY(ft_itoa) ENTRY(ft_strmapi) ENTRY(ft_striteri) ENTRY(ft_putchar_fd)
	ENTRY(ft_putstr_fd) ENTRY(ft_putendl_fd) ENTRY(ft_putnbr_fd)
	HEAD("BONUS - linked lists")
	ENTRY(ft_lstnew) ENTRY(ft_lstadd_front) ENTRY(ft_lstsize) ENTRY(ft_lstlast)
	ENTRY(ft_lstadd_back) ENTRY(ft_lstdelone) ENTRY(ft_lstclear) ENTRY(ft_lstiter)
	ENTRY(ft_lstmap)
};

static const char *verdict(int st, int *bad)
{
	static char	buf[64];

	*bad = 1;
	if (WIFSIGNALED(st))
	{
		if (WTERMSIG(st) == SIGALRM)
			return ("TIMEOUT (infinite loop?)");
		if (WTERMSIG(st) == SIGSEGV)
			return ("SEGFAULT");
		if (WTERMSIG(st) == SIGABRT)
			return ("ABORT (sanitizer / crash)");
		snprintf(buf, sizeof(buf), "KILLED by signal %d", WTERMSIG(st));
		return (buf);
	}
	if (WEXITSTATUS(st) == 0)
		*bad = 0;
	else if (WEXITSTATUS(st) == 2)
		return ("wrong result");
	else if (WEXITSTATUS(st) == 23)
		return ("MEMORY LEAK");
	else if (WEXITSTATUS(st) == 3)
		return ("MEMORY ERROR (see sanitizer report above)");
	else
	{
		snprintf(buf, sizeof(buf), "sanitizer error / exit code %d", WEXITSTATUS(st));
		return (buf);
	}
	return ("");
}

int main(void)
{
	size_t		i;
	int			ok = 0, ko = 0, skip = 0, st, bad;
	pid_t		pid;
	const char	*v;

	g_sh = mmap(NULL, sizeof(*g_sh), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (i = 0; i < sizeof(g_tests) / sizeof(*g_tests); i++)
	{
		if (g_tests[i].have < 0)
		{
			printf("\n" BOLD "== %s ==" RST "\n", g_tests[i].name);
			continue ;
		}
		if (!g_tests[i].have)
		{
			printf("%-18s " YEL "SKIP" RST " (file missing or does not compile)\n", g_tests[i].name);
			skip++;
			continue ;
		}
		memset(g_sh, 0, sizeof(*g_sh));
		fflush(NULL);
		pid = fork();
		if (pid == 0)
		{
			alarm(5);
			g_tests[i].fn();
			exit(g_sh->fails ? 2 : 0);
		}
		waitpid(pid, &st, 0);
		v = verdict(st, &bad);
		fflush(NULL);
		if (!bad)
		{
			printf("%-18s " GREEN "OK" RST "\n", g_tests[i].name);
			ok++;
		}
		else
		{
			printf("%-18s " RED "KO" RST " - %s\n%s", g_tests[i].name, v, g_sh->msg);
			ko++;
		}
	}
	printf("\n" BOLD "Result: " RST GREEN "%d OK" RST ", " RED "%d KO" RST ", " YEL "%d skipped" RST "\n",
		ok, ko, skip);
	return (ko != 0);
}
