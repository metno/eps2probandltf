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
AC_DEFUN([WDB_USER_CREATE],
[
	AC_ARG_ENABLE(create_user,
     	AC_HELP_STRING( [--disable-create-user], [Do not automatically create a wdb user at install time.] ),
     			[case "${enableval}" in
       				yes) create_user=true ;;
       				no)  create_user=false ;;
       				*) AC_MSG_ERROR(bad value ${enableval} for --enable-create-user) ;;
     			 esac],
     			 [create_user=true])
	AC_SUBST( [DO_CREATE_USER], [$create_user] )
])
