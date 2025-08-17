

AC_INIT

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CXX

STATIC=
PLATFORM=
CFLAGS=
LFLAGS=
PCFLAGS=
INCLUDEPATH=
LIBFILES=
ENABLEDEMO=

AC_ARG_ENABLE(static, AS_HELP_STRING([--enable-static], [Enable static linkage]))
AC_ARG_ENABLE(demo, AS_HELP_STRING([--enable-demo], [Enable demo building]))

if test x$enable_static = xyes; then
	STATIC=-static
fi

if test x$enable_demo = xyes; then
	ENABLEDEMO=true
fi

case $host_os in
	*bsd*)
		PLATFORM=FreeBSD
		CFLAGS="-pipe -O2 -std=gnu++11 -Wno-unused-result -Wno-int-to-pointer-cast -fPIC"
		LFLAGS="-Wl,-O1 $STATIC"
		INCLUDEPATH="-I/usr/local/include"
		LIBFILES="-L/usr/local/lib -lfltk -lfltk_images -lfltk_png -lfltk_z -ldl -lrt -lXcursor -lX11 -lXft -lfontconfig -lXrender -lXfixes -lXinerama -lXext -lxcb -lXau -lXdmcp -lpthread -lfreetype -lexpat -luuid -lutil -lbz2 -lintl"
		;;
	linux*)
		PLATFORM=Linux
		CFLAGS="-pipe -O2 -std=gnu++11 -Wno-unused-result -Wno-int-to-pointer-cast -fPIC"
		LFLAGS="-Wl,-O1 $STATIC"
		INCLUDEPATH="-I/usr/local/include"
		LIBFILES="-L/usr/local/lib -lfltk -lfltk_images -lfltk_png -lfltk_z -ldl -lrt -lXcursor -lX11 -lXft -lfontconfig -lXrender -lXfixes -lXinerama -lXext -lxcb -lXau -lXdmcp -lpthread -lfreetype -lexpat -luuid"
		;;
	darwin*)
		PLATFORM=MacOS
		CFLAGS="-pipe -std=gnu++11 -Wno-unused-result -Wno-int-to-pointer-cast -fPIC -mmacosx-version-min=10.10"
		LFLAGS="-Wl -mmacosx-version-min=10.10"
		PCFLAGS="-x objective-c++"
		INCLUDEPATH="-I/usr/local/include"
		LIBFILES="-L/usr/local/lib -lfltk -lfltk_images -lfltk_png -lfltk_z -lpthread -framework Cocoa -framework Security"
		;;
esac

AC_SUBST(STATIC)
AC_SUBST(PLATFORM)
AC_SUBST(CFLAGS)
AC_SUBST(LFLAGS)
AC_SUBST(PCFLAGS)
AC_SUBST(INCLUDEPATH)
AC_SUBST(LIBFILES)
AC_SUBST(ENABLEDEMO)

AC_CONFIG_FILES([Makefile Demo/UPIClient/Makefile Demo/UPIService/Makefile])

AC_OUTPUT

