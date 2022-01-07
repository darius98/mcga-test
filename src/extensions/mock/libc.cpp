#include "mcga/test_ext/mock/libc.hpp"

#include "pp_codegen.hpp"

namespace mcga::test::mock {

MCGA_TEST_EXPORT LibCMocks libc asm("_mcga_test_ext_libc_mock");

}  // namespace mcga::test::mock

using mcga::test::mock::libc;

DECLARE_NORETURN_FUNCTION(libc, abort, 0)
DECLARE_FUNCTION(libc, abs, 1)
DECLARE_FUNCTION(libc, acos, 1)
DECLARE_FUNCTION(libc, aligned_alloc, 2)
DECLARE_FUNCTION(libc, asctime, 1)
DECLARE_FUNCTION(libc, asctime_r, 2)
DECLARE_FUNCTION(libc, asin, 1)
DECLARE_FUNCTION(libc, atan, 1)
DECLARE_FUNCTION(libc, atan2, 2)
DECLARE_FUNCTION(libc, atexit, 1)
DECLARE_FUNCTION(libc, atof, 1)
DECLARE_FUNCTION(libc, atoi, 1)
DECLARE_FUNCTION(libc, atol, 1)
DECLARE_FUNCTION(libc, bsearch, 5)
DECLARE_FUNCTION(libc, btowc, 1)
DECLARE_FUNCTION(libc, calloc, 2)
DECLARE_FUNCTION(libc, ceil, 1)
DECLARE_FUNCTION(libc, clearerr, 1)
DECLARE_FUNCTION(libc, clock, 0)
DECLARE_FUNCTION(libc, cos, 1)
DECLARE_FUNCTION(libc, cosh, 1)
DECLARE_FUNCTION(libc, ctime, 1)
DECLARE_FUNCTION(libc, ctime_r, 2)
DECLARE_FUNCTION(libc, difftime, 2)
DECLARE_FUNCTION(libc, div, 2)
DECLARE_FUNCTION(libc, erf, 1)
DECLARE_FUNCTION(libc, erfc, 1)
DECLARE_NORETURN_FUNCTION(libc, exit, 1)
DECLARE_FUNCTION(libc, exp, 1)
DECLARE_FUNCTION(libc, fabs, 1)
DECLARE_FUNCTION(libc, fclose, 1)
DECLARE_FUNCTION(libc, fdopen, 2)
DECLARE_FUNCTION(libc, feof, 1)
DECLARE_FUNCTION(libc, ferror, 1)
DECLARE_FUNCTION(libc, fflush, 1)
DECLARE_FUNCTION(libc, fgetc, 1)
DECLARE_FUNCTION(libc, fgetpos, 2)
DECLARE_FUNCTION(libc, fgets, 3)
DECLARE_FUNCTION(libc, fgetwc, 1)
DECLARE_FUNCTION(libc, fgetws, 3)
DECLARE_FUNCTION(libc, fileno, 1)
DECLARE_FUNCTION(libc, floor, 1)
DECLARE_FUNCTION(libc, fmod, 2)
DECLARE_FUNCTION(libc, fopen, 2)
DECLARE_FUNCTION(libc, fputc, 2)
DECLARE_FUNCTION(libc, fputs, 2)
DECLARE_FUNCTION(libc, fputwc, 2)
DECLARE_FUNCTION(libc, fputws, 2)
DECLARE_FUNCTION(libc, fread, 4)
DECLARE_FUNCTION(libc, free, 1)
DECLARE_FUNCTION(libc, freopen, 3)
DECLARE_FUNCTION(libc, frexp, 2)
DECLARE_FUNCTION(libc, fseek, 3)
DECLARE_FUNCTION(libc, fsetpos, 2)
DECLARE_FUNCTION(libc, ftell, 1)
DECLARE_FUNCTION(libc, fwide, 2)
DECLARE_FUNCTION(libc, fwrite, 4)
DECLARE_FUNCTION(libc, gamma, 1)
DECLARE_FUNCTION(libc, getc, 1)
DECLARE_FUNCTION(libc, getchar, 0)
DECLARE_FUNCTION(libc, getenv, 1)
DECLARE_FUNCTION(libc, gets, 1)
DECLARE_FUNCTION(libc, getwc, 1)
DECLARE_FUNCTION(libc, getwchar, 0)
DECLARE_FUNCTION(libc, gmtime, 1)
DECLARE_FUNCTION(libc, gmtime_r, 2)
DECLARE_FUNCTION(libc, hypot, 2)
DECLARE_FUNCTION(libc, iswalnum, 1)
DECLARE_FUNCTION(libc, iswalpha, 1)
DECLARE_FUNCTION(libc, iswblank, 1)
DECLARE_FUNCTION(libc, iswcntrl, 1)
DECLARE_FUNCTION(libc, iswctype, 2)
DECLARE_FUNCTION(libc, iswdigit, 1)
DECLARE_FUNCTION(libc, iswgraph, 1)
DECLARE_FUNCTION(libc, iswlower, 1)
DECLARE_FUNCTION(libc, iswprint, 1)
DECLARE_FUNCTION(libc, iswpunct, 1)
DECLARE_FUNCTION(libc, iswspace, 1)
DECLARE_FUNCTION(libc, iswupper, 1)
DECLARE_FUNCTION(libc, iswxdigit, 1)
DECLARE_FUNCTION(libc, j0, 1)
DECLARE_FUNCTION(libc, j1, 1)
DECLARE_FUNCTION(libc, jn, 2)
DECLARE_FUNCTION(libc, labs, 1)
DECLARE_FUNCTION(libc, ldexp, 2)
DECLARE_FUNCTION(libc, ldiv, 2)
DECLARE_FUNCTION(libc, localeconv, 0)
DECLARE_FUNCTION(libc, localtime, 1)
DECLARE_FUNCTION(libc, localtime_r, 2)
DECLARE_FUNCTION(libc, log, 1)
DECLARE_FUNCTION(libc, log10, 1)
DECLARE_FUNCTION(libc, malloc, 1)
DECLARE_FUNCTION(libc, mblen, 2)
DECLARE_FUNCTION(libc, mbrlen, 3)
DECLARE_FUNCTION(libc, mbrtowc, 4)
DECLARE_FUNCTION(libc, mbsinit, 1)
DECLARE_FUNCTION(libc, mbsrtowcs, 4)
DECLARE_FUNCTION(libc, mbstowcs, 3)
DECLARE_FUNCTION(libc, mbtowc, 3)
DECLARE_FUNCTION(libc, memchr, 3)
DECLARE_FUNCTION(libc, memcmp, 3)
DECLARE_FUNCTION(libc, memcpy, 3)
DECLARE_FUNCTION(libc, memmove, 3)
DECLARE_FUNCTION(libc, memset, 3)
DECLARE_FUNCTION(libc, mktime, 1)
DECLARE_FUNCTION(libc, modf, 2)
DECLARE_FUNCTION(libc, nextafter, 2)
DECLARE_FUNCTION(libc, nextafterl, 2)
DECLARE_FUNCTION(libc, nexttoward, 2)
DECLARE_FUNCTION(libc, nexttowardl, 2)
DECLARE_FUNCTION(libc, perror, 1)
DECLARE_FUNCTION(libc, pow, 2)
DECLARE_FUNCTION(libc, putc, 2)
DECLARE_FUNCTION(libc, putchar, 1)
DECLARE_FUNCTION(libc, putenv, 1)
DECLARE_FUNCTION(libc, puts, 1)
DECLARE_FUNCTION(libc, putwc, 2)
DECLARE_FUNCTION(libc, putwchar, 1)
DECLARE_FUNCTION(libc, qsort, 4)
DECLARE_FUNCTION(libc, raise, 1)
DECLARE_FUNCTION(libc, rand, 0)
DECLARE_FUNCTION(libc, rand_r, 1)
DECLARE_FUNCTION(libc, realloc, 2)
DECLARE_FUNCTION(libc, remove, 1)
DECLARE_FUNCTION(libc, rename, 2)
DECLARE_FUNCTION(libc, rewind, 1)
DECLARE_FUNCTION(libc, setbuf, 2)
DECLARE_FUNCTION(libc, setlocale, 2)
DECLARE_FUNCTION(libc, setvbuf, 4)
DECLARE_FUNCTION(libc, sin, 1)
DECLARE_FUNCTION(libc, sinh, 1)
DECLARE_FUNCTION(libc, sqrt, 1)
DECLARE_FUNCTION(libc, srand, 1)
DECLARE_FUNCTION(libc, strcasecmp, 2)
DECLARE_FUNCTION(libc, strcat, 2)
DECLARE_FUNCTION(libc, strchr, 2)
DECLARE_FUNCTION(libc, strcmp, 2)
DECLARE_FUNCTION(libc, strcoll, 2)
DECLARE_FUNCTION(libc, strcpy, 2)
DECLARE_FUNCTION(libc, strcspn, 2)
DECLARE_FUNCTION(libc, strerror, 1)
DECLARE_FUNCTION(libc, strftime, 4)
DECLARE_FUNCTION(libc, strlen, 1)
DECLARE_FUNCTION(libc, strncasecmp, 3)
DECLARE_FUNCTION(libc, strncat, 3)
DECLARE_FUNCTION(libc, strncmp, 3)
DECLARE_FUNCTION(libc, strncpy, 3)
DECLARE_FUNCTION(libc, strpbrk, 2)
DECLARE_FUNCTION(libc, strptime, 3)
DECLARE_FUNCTION(libc, strrchr, 2)
DECLARE_FUNCTION(libc, strspn, 2)
DECLARE_FUNCTION(libc, strstr, 2)
DECLARE_FUNCTION(libc, strtod, 2)
DECLARE_FUNCTION(libc, strtof, 2)
DECLARE_FUNCTION(libc, strtok, 2)
DECLARE_FUNCTION(libc, strtok_r, 3)
DECLARE_FUNCTION(libc, strtol, 3)
DECLARE_FUNCTION(libc, strtold, 2)
DECLARE_FUNCTION(libc, strtoul, 3)
DECLARE_FUNCTION(libc, strxfrm, 3)
DECLARE_FUNCTION(libc, system, 1)
DECLARE_FUNCTION(libc, tan, 1)
DECLARE_FUNCTION(libc, tanh, 1)
DECLARE_FUNCTION(libc, time, 1)
DECLARE_FUNCTION(libc, tmpfile, 0)
DECLARE_FUNCTION(libc, tmpnam, 1)
DECLARE_FUNCTION(libc, towctrans, 2)
DECLARE_FUNCTION(libc, towlower, 1)
DECLARE_FUNCTION(libc, towupper, 1)
DECLARE_FUNCTION(libc, ungetc, 2)
DECLARE_FUNCTION(libc, ungetwc, 2)
DECLARE_FUNCTION(libc, vfprintf, 3)
DECLARE_FUNCTION(libc, vfscanf, 3)
DECLARE_FUNCTION(libc, vfwprintf, 3)
DECLARE_FUNCTION(libc, vfwscanf, 3)
DECLARE_FUNCTION(libc, vprintf, 2)
DECLARE_FUNCTION(libc, vscanf, 2)
DECLARE_FUNCTION(libc, vsprintf, 3)
DECLARE_FUNCTION(libc, vsnprintf, 4)
DECLARE_FUNCTION(libc, vsscanf, 3)
DECLARE_FUNCTION(libc, vswprintf, 4)
DECLARE_FUNCTION(libc, vswscanf, 3)
DECLARE_FUNCTION(libc, vwprintf, 2)
DECLARE_FUNCTION(libc, vwscanf, 2)
DECLARE_FUNCTION(libc, wcrtomb, 3)
DECLARE_FUNCTION(libc, wcscat, 2)
DECLARE_FUNCTION(libc, wcschr, 2)
DECLARE_FUNCTION(libc, wcscmp, 2)
DECLARE_FUNCTION(libc, wcscoll, 2)
DECLARE_FUNCTION(libc, wcscpy, 2)
DECLARE_FUNCTION(libc, wcscspn, 2)
DECLARE_FUNCTION(libc, wcsftime, 4)
DECLARE_FUNCTION(libc, wcslen, 1)
DECLARE_FUNCTION(libc, wcsncat, 3)
DECLARE_FUNCTION(libc, wcsncmp, 3)
DECLARE_FUNCTION(libc, wcsncpy, 3)
DECLARE_FUNCTION(libc, wcspbrk, 2)
DECLARE_FUNCTION(libc, wcsrchr, 2)
DECLARE_FUNCTION(libc, wcsrtombs, 4)
DECLARE_FUNCTION(libc, wcsspn, 2)
DECLARE_FUNCTION(libc, wcsstr, 2)
DECLARE_FUNCTION(libc, wcstod, 2)
DECLARE_FUNCTION(libc, wcstof, 2)
DECLARE_FUNCTION(libc, wcstok, 3)
DECLARE_FUNCTION(libc, wcstol, 3)
DECLARE_FUNCTION(libc, wcstold, 2)
DECLARE_FUNCTION(libc, wcstombs, 3)
DECLARE_FUNCTION(libc, wcstoul, 3)
DECLARE_FUNCTION(libc, wcsxfrm, 3)
DECLARE_FUNCTION(libc, wctob, 1)
DECLARE_FUNCTION(libc, wctomb, 2)
DECLARE_FUNCTION(libc, wctrans, 1)
DECLARE_FUNCTION(libc, wctype, 1)
DECLARE_FUNCTION(libc, wcwidth, 1)
DECLARE_FUNCTION(libc, wmemchr, 3)
DECLARE_FUNCTION(libc, wmemcmp, 3)
DECLARE_FUNCTION(libc, wmemcpy, 3)
DECLARE_FUNCTION(libc, wmemmove, 3)
DECLARE_FUNCTION(libc, wmemset, 3)
DECLARE_FUNCTION(libc, y0, 1)
DECLARE_FUNCTION(libc, y1, 1)
DECLARE_FUNCTION(libc, yn, 2)

DECLARE_VA_FUNCTION(libc, fprintf, 2)
DECLARE_VA_FUNCTION(libc, fscanf, 2)
DECLARE_VA_FUNCTION(libc, fwprintf, 2)
DECLARE_VA_FUNCTION(libc, fwscanf, 2)
DECLARE_VA_FUNCTION(libc, printf, 1)
DECLARE_VA_FUNCTION(libc, scanf, 1)
DECLARE_VA_FUNCTION(libc, snprintf, 3)
DECLARE_VA_FUNCTION(libc, sprintf, 2)
DECLARE_VA_FUNCTION(libc, sscanf, 2)
DECLARE_VA_FUNCTION(libc, swprintf, 3)
DECLARE_VA_FUNCTION(libc, swscanf, 2)
DECLARE_VA_FUNCTION(libc, wprintf, 1)
DECLARE_VA_FUNCTION(libc, wscanf, 1)