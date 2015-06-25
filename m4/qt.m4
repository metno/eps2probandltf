AC_DEFUN([QT_CORE_CHECK],
[
required_qt_version=ifelse([$1], [], [1.0], [$1])

PKG_CHECK_MODULES(
	[QtCore],
	[QtCore >= $required_qt_version],
	[
		AC_SUBST(QtCore_CFLAGS)
		AC_SUBST(QtCore_LDFLAGS)
		AC_SUBST(QtCore_LIBS)
				
		CPPFLAGS="$CPPFLAGS $qt_CFLAGS"
		LDFLAGS="$LDFLAGS $qt_LDFLAGS"
		LIBS="$LIBS $QtCore_LIBS"
	],
	[
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not find qt version $required_version_number using pkg-config. Verify that qt 
    is correctly installed, and that pkg-config has access to the the 
    appropriate .pc file.
-------------------------------------------------------------------------
		])
	])
])

AC_DEFUN([QT_XML_CHECK],
[
required_qt_version=ifelse([$1], [], [1.0], [$1])

PKG_CHECK_MODULES(
	[QtXml],
	[QtXml >= $required_qt_version],
	[
		AC_SUBST(QtXml_CFLAGS)
		AC_SUBST(QtXml_LDFLAGS)
		AC_SUBST(QtXml_LIBS)
				
		CPPFLAGS="$CPPFLAGS $qt_CFLAGS"
		LDFLAGS="$LDFLAGS $qt_LDFLAGS"
		LIBS="$LIBS $QtXml_LIBS"
	],
	[
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not find qt version $required_version_number using pkg-config. Verify that qt 
    is correctly installed, and that pkg-config has access to the the 
    appropriate .pc file.
-------------------------------------------------------------------------
		])
	])
])
