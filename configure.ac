#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([bacteria], [0.1], [bug@sportloto.i2p])
AC_CONFIG_SRCDIR([libbacteria/macros.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_PROG_RANLIB([RANLIB])
# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
#-ljansson
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([iniparser], [main])
AC_CHECK_LIB([jansson], [main])
AC_CHECK_LIB([curl], [main])
AC_CHECK_LIB([ssl], [main])
#AC_CHECK_LIB([gpgme], [main])
AC_CHECK_LIB([crypto], [main])
AC_CHECK_LIB([gd], [main])
AC_CHECK_LIB([png], [main])
AC_CHECK_LIB([gmp], [main])
AC_CHECK_LIB([z], [main])
AC_CHECK_LIB([luajit], [main])
AC_CHECK_LIB([freetype], [main])
CFLAGS="$CFLAGS -g"

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T

# Checks for library functions.
AC_CHECK_FUNCS([bzero inet_ntoa socket])

AC_CONFIG_FILES([
		 Makefile
                 src/Makefile
		 libbacteria/Makefile
		 src/bootstrap/Makefile
		 src/client/Makefile
		 ])
AC_OUTPUT
