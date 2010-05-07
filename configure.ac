#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([apmech], [0.2.0], [apmech@lehtiranta.net])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

dnl Set CEGUI flags
PKG_CHECK_MODULES([CEGUI], [CEGUI >= 0.6])

dnl Set Ogre flags
PKG_CHECK_MODULES([OGRE], [OGRE >= 1.6])

dnl set CeguiOGRE flags
PKG_CHECK_MODULES([CEGUI_OGRE], [CEGUI-OGRE >= 0.1])

dnl Set SDL flags
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

dnl Set SDL_image flags
PKG_CHECK_MODULES([SDLIMAGE], [SDL_image >= 0.1])

dnl Get enet library and include locations

dnl enet headers have a file "time.h" which conflicts
dnl with system headers that ogre requires.
dnl enet is expected to be used from default include path
dnl with #include <enet/time.h>.
AC_ARG_WITH([enet-include-path], [AS_HELP_STRING([--with-enet-include-path],
  [path containing the enet include directory, defaults to /usr/include])],
  [],
  [with_enet_include_path=yes])

AS_IF([test "x$with_enet_include_path" = xno],
  [with_enet_include_path=yes])
AS_IF([test "x$with_enet_include_path" != xyes],
  [AC_SUBST([ENET_CFLAGS], ["-I$with_enet_include_path"])],
  [AC_SUBST([ENET_CFLAGS], [-I/usr/include])])

AC_ARG_WITH([enet-lib-path], [AS_HELP_STRING([--with-enet-lib-path],
  [location of the enet libraries, defaults to /usr/lib])],
  [],
  [with_enet_lib_path=yes])

AS_IF([test "x$with_enet_lib_path" = xno],
  [with_enet_lib_path=yes])
AS_IF([test "x$with_enet_lib_path" != xyes],
  [AC_SUBST([ENET_LIBS], ["-L$with_enet_lib_path -lenet"])],
  [AC_SUBST([ENET_LIBS], ["-L/usr/lib -lenet"])])

# Checks for header files.
AC_PATH_X
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h inttypes.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/param.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit floor gettimeofday memmove memset pow select sqrt strtol])

AC_CONFIG_FILES([Makefile
		src/Makefile
		src/math/Makefile
		src/ooinput/Makefile
		src/net/Makefile
		src/server/Makefile
		])
AC_OUTPUT
