#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Pre-Conf
#-----------------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT(eps2probandltf, 1.1.2, [wdb@met.no])
# Config
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

AC_CONFIG_MACRO_DIR([m4])


#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S


#-----------------------------------------------------------------------------
# Checks
#-----------------------------------------------------------------------------

# BOOST
AX_BOOST_BASE(1.33.1)
AX_BOOST_REGEX(1.33.1)
AX_BOOST_DATE_TIME(1.33.1)
AX_BOOST_FILESYSTEM(1.33.1)
AX_BOOST_PROGRAM_OPTIONS(1.33.1)
AX_BOOST_SYSTEM(1.33.1)

PKG_CHECK_MODULES([log4cxx], [liblog4cxx],
   	[
		AC_SUBST(log4cxx_CFLAGS)
		AC_SUBST(log4cxx_LIBS)
	],
	[AC_MSG_ERROR([Unable to find liblog4cxx.])
	])

AC_PROG_RANLIB
GMOCK_DIST_CHECK(.)

PKG_CHECK_MODULES([fimex], [fimex],
   	[
		AC_SUBST(fimex_CFLAGS)
		AC_SUBST(fimex_LIBS)
	],
	[AC_MSG_ERROR([Unable to find fimex.])
	])

PKG_CHECK_MODULES([weather_symbol], [libweather-symbol],
   	[
		AC_SUBST(weather_symbol_CFLAGS)
		AC_SUBST(weather_symbol_LIBS)
	],
	[AC_MSG_ERROR([Unable to find weather_symbol library.])
	])


#-----------------------------------------------------------------------------
# Automake
#-----------------------------------------------------------------------------

AM_INIT_AUTOMAKE(-Wall -Werror subdir-objects)
AM_PROG_CC_C_O

METNO_PVERSION


#-----------------------------------------------------------------------------
# Configure and Exit
#-----------------------------------------------------------------------------

# Configuration Files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
