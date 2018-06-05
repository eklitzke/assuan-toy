AC_PREREQ([2.68])
AC_INIT([assuan-toy], [0.1], [evan@eklitzke.org])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/main.cc])

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

AM_INIT_AUTOMAKE([dist-bzip2 foreign subdir-objects -Wall -Werror])
AM_PATH_LIBASSUAN([2])

# Checks for libraries.

# Checks for header files.
# FIXME: need to make this work with AM_PATH_LIBASSUAN
#AC_CHECK_HEADERS([assuan.h], [], [AC_MSG_ERROR([Could not find assuan.h])])
AC_CHECK_HEADERS([gpg-error.h], [], [AC_MSG_ERROR([Could not find gpg-error.h])])
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
