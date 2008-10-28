#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([qUtilities], [3.1.0], [diana@met.no])
AM_INIT_AUTOMAKE
METNO_SET_VERSION_INFO

AC_CONFIG_SRCDIR([src/miMessage.cc])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_LANG_PUSH(C++)

# Checks for libraries.
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

METNO_HAVE_LOG4CXX

HAVE_QT4_CHECK			# Check for QT.

# Make sure pkg-config also considers $prefix/lib/pkgconfig.
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$prefix/lib/pkgconfig

# Check for the puTools library.
PKG_CHECK_MODULES([puTools], [putools])
METNO_CHECK_LIB([puTools], [main], , , , [$puTools_LIBS], 
                [the puTools package])
METNO_CHECK_HEADERS([puTools/miString.h puTools/miTime puTools/miDate], , , ,
		    [$puTools_CFLAGS], [the puTools package])

# Check for the puSQL library.
PKG_CHECK_MODULES([puSQL], [pusql])
METNO_CHECK_LIB([puSQL], [main], , , , [$puSQL_LIBS], [the puSQL package])
METNO_CHECK_HEADERS([puSQL/puSQLgate.h], , , , [$puSQL_CFLAGS], 
                    [the puSQL package])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER([stdlib.h], , AC_MSG_ERROR("missing header"))

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([socket])

AC_LANG_POP(C++)


#making our own .pc file, checking that it is ok and forfilled
AC_CONFIG_FILES(qutilities.pc)
AC_OUTPUT

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:.
PKG_CHECK_MODULES([own_dependencies], [${PACKAGE_TARNAME}])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
		 lang/Makefile])
AC_OUTPUT
