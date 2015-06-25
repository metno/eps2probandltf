#-----------------------------------------------------------------------------
# Project Makefile for wdb's probabilityForecast
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------


# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I m4

pkgdatarootdir = $(datarootdir)/eps2probandltf
pkgsysconfdir = $(sysconfdir)/eps2probandltf

AM_CPPFLAGS = -DSYSCONFDIR='"$(pkgsysconfdir)"'
AM_CXXFLAGS = -I@top_srcdir@/src/  $(fimex_CFLAGS) $(weather_symbol_CFLAGS) $(log4cxx_CFLAGS)

# Setting Targets
bin_PROGRAMS = eps2probandltf 
eps2probandltf_SOURCES =

eps2probandltf_LDADD = $(fimex_LIBS) $(weather_symbol_LIBS) $(log4cxx_LIBS) \
	$(BOOST_REGEX_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) 
eps2probandltf_LDFLAGS = 


man1_MANS = eps2probandltf.1

eps2probandltf.1:	eps2probandltf
	help2man -n "Generate probabilities based on ensemble data" -N  ./$< > $@


WDB_VERSION = "$(VERSION)"

CLEANFILES = eps2probandltf.1


EXTRA_DIST =

include src/probabilityForecast.mk
include etc/probabilityForecast.mk
include test/probabilityForecast.mk

include mk/metno_debian.mk
