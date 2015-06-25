#
#   probabilityForecast
#
#   Copyright (C) 2008 met.no
#   
#   Contact information:
#   Norwegian Meteorological Institute
#   Box 43 Blindern
#   0313 OSLO
#   NORWAY
#   E-mail: wdb@met.no
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
#   MA  02110-1301, USA
#

#
#   Enable/Disable wdb user creation at install time
#
AC_DEFUN([WDB_EMBED_GTEST],
[
	AC_MSG_CHECKING([if test suite should be embedded into program])
	AC_ARG_ENABLE(embedded-test,
     	AC_HELP_STRING( [--enable-embedded-test], [Embed test suite into program.] ),
		[],
		[enable_embedded_test=no])
	AM_CONDITIONAL( [EMBED_TEST], [test x$enable_embedded_test != xno] )
	AC_MSG_RESULT([$enable_embedded_test])
])
