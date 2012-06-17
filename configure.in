# Process this file with autoconf to produce a configure script.

AC_INIT(hsc, 1.0, matthias.bethke@gmx.net)
AC_PREREQ(2.57)
AC_CONFIG_SRCDIR([src/ubiqx/sys_include.h])
AC_CONFIG_HEADER([src/config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([locale.h memory.h netdb.h netinet/in.h stddef.h stdlib.h string.h strings.h sys/param.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
#AC_CHECK_FUNCS(isascii)
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([atexit bzero getcwd gethostbyname getpagesize memset socket strchr strerror strpbrk strstr strtol strtoul])

AC_CANONICAL_TARGET
case $host in
	*-*-mingw32* | *-*-cygwin* | *-*-windows*)
	SYSTEM="WINNT"
	LINKLIBS=-lws2_32
	;;
	*)
	SYSTEM="UNIX"
	;;
esac
echo "Configuring for $SYSTEM"
CFLAGS="-Wall -pedantic"

AC_ARG_ENABLE(debug,
	      AS_HELP_STRING([--enable-debug],[Compile programs with GDB support and debugging output]),
	      [CFLAGS="$CFLAGS -ggdb -g3 -DDEBUG"; REGEXDEBUG="regex/printchar.o"],
	      CFLAGS="$CFLAGS -fomit-frame-pointer -Os")
AC_ARG_ENABLE(efence,
	      AS_HELP_STRING([--enable-efence],[Link with efence library for memory debugging]),
	      CFLAGS="$CFLAGS -lefence")
AC_ARG_ENABLE(gnuargs,
	      AS_HELP_STRING([--enable-gnu-args],[Change Amiga-style command line args to GNU style (not implemented yet)]),
	      CFLAGS="$CFLAGS -DGNU_STYLE_ARGS")
AC_ARG_ENABLE(diet,
	      AS_HELP_STRING([--enable-diet],[Link against dietlibc instead of glibc]),
	      CC="diet gcc -nostdinc")

# Substitute variables
AC_SUBST(CC)
AC_SUBST(OBJEXT)
AC_SUBST(SYSTEM)
AC_SUBST(REGEXDEBUG)
AC_SUBST(LINKLIBS)

# Configure the regex package
AC_CONFIG_SUBDIRS(src/regex)
AC_CONFIG_FILES([Makefile docs-source/Makefile src/Makefile])
AC_OUTPUT
