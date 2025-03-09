AC_PREREQ([2.69])
AC_INIT([openbox],
        [3.7],
        [http://bugzilla.icculus.org],
        [openbox])
AC_CONFIG_SRCDIR([openbox/openbox.c])

# If macros are in an m4/ directory, let autoconf know:
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])

OB_VERSION=$PACKAGE_VERSION
AC_SUBST(OB_VERSION)

dnl Making releases:
dnl RR_ is for obrender, OBT_ is for obt, keep major/minor in sync,
dnl treat the rest separately.
dnl
dnl if MAJOR or MINOR version changes, be sure to change AC_INIT above to match.
dnl the other variables aren't related to the user visible versions, but are used
dnl to version the .so files.
dnl if no backward compat is broken, it's just a preference if you want to bump
dnl the AC_INIT micro version or minor version. if the AC_INIT micro version is
dnl 0, don't write it out.
dnl
dnl How to process .so versioning,
dnl   RR_MICRO_VERSION += 1;
dnl   RR_INTERFACE_AGE += 1;
dnl   RR_BINARY_AGE += 1;
dnl if any functions have been added, set RR_INTERFACE_AGE to 0.
dnl if backwards compatibility has been broken,
dnl set RR_BINARY_AGE and RR_INTERFACE_AGE to 0.
dnl if you set anything to 0, you should also bump the MINOR version.
dnl
RR_MAJOR_VERSION=3
RR_MINOR_VERSION=6
RR_MICRO_VERSION=33
RR_INTERFACE_AGE=1
RR_BINARY_AGE=1
RR_VERSION=$RR_MAJOR_VERSION.$RR_MINOR_VERSION

OBT_MAJOR_VERSION=3
OBT_MINOR_VERSION=6
OBT_MICRO_VERSION=5
OBT_INTERFACE_AGE=3
OBT_BINARY_AGE=3
OBT_VERSION=$OBT_MAJOR_VERSION.$OBT_MINOR_VERSION

AC_SUBST(RR_MAJOR_VERSION)
AC_SUBST(RR_MINOR_VERSION)
AC_SUBST(RR_MICRO_VERSION)
AC_SUBST(RR_INTERFACE_AGE)
AC_SUBST(RR_BINARY_AGE)
AC_SUBST(RR_VERSION)
AC_SUBST(OBT_MAJOR_VERSION)
AC_SUBST(OBT_MINOR_VERSION)
AC_SUBST(OBT_MICRO_VERSION)
AC_SUBST(OBT_INTERFACE_AGE)
AC_SUBST(OBT_BINARY_AGE)
AC_SUBST(OBT_VERSION)

dnl Libtool versioning
RR_RELEASE=$RR_MAJOR_VERSION.$RR_MINOR_VERSION
RR_CURRENT=$((RR_MICRO_VERSION - RR_INTERFACE_AGE))
RR_REVISION=$RR_INTERFACE_AGE
RR_AGE=$((RR_BINARY_AGE - RR_INTERFACE_AGE))
RR_CURRENT_MINUS_AGE=$((RR_CURRENT - RR_AGE))

OBT_RELEASE=$OBT_MAJOR_VERSION.$OBT_MINOR_VERSION
OBT_CURRENT=$((OBT_MICRO_VERSION - OBT_INTERFACE_AGE))
OBT_REVISION=$OBT_INTERFACE_AGE
OBT_AGE=$((OBT_BINARY_AGE - OBT_INTERFACE_AGE))
OBT_CURRENT_MINUS_AGE=$((OBT_CURRENT - OBT_AGE))

AC_SUBST(RR_RELEASE)
AC_SUBST(RR_CURRENT)
AC_SUBST(RR_REVISION)
AC_SUBST(RR_AGE)
AC_SUBST(RR_CURRENT_MINUS_AGE)
AC_SUBST(OBT_RELEASE)
AC_SUBST(OBT_CURRENT)
AC_SUBST(OBT_REVISION)
AC_SUBST(OBT_AGE)
AC_SUBST(OBT_CURRENT_MINUS_AGE)

AC_PREFIX_DEFAULT([/usr/local])
test "$prefix" = "NONE" && prefix=$ac_default_prefix

dnl Determine build target
OB_DEBUG
dnl Pick compiler specific/build target flags, and set $CVS
AM_PROG_CC_C_O
OB_COMPILER_FLAGS
AC_C_CONST
AC_C_INLINE

AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
LIBTOOL="$LIBTOOL --silent"

AC_PROG_INSTALL

AM_GNU_GETTEXT_VERSION([0.15])
AM_GNU_GETTEXT([external])

dnl Check for system headers
AC_CHECK_HEADERS([ctype.h dirent.h errno.h fcntl.h grp.h locale.h pwd.h])
AC_CHECK_HEADERS([signal.h string.h stdio.h stdlib.h unistd.h sys/stat.h])
AC_CHECK_HEADERS([sys/select.h sys/socket.h sys/time.h sys/types.h sys/wait.h])

dnl Check for sed
AC_PROG_SED
AS_IF([test "x$SED" = x],
  [AC_MSG_ERROR([A working sed is required to build Openbox.])])

dnl Check for dirname
AC_CHECK_PROG([DIRNAME], [dirname], [dirname], [no])
AS_IF([test "x$DIRNAME" = xno],
  [AC_MSG_ERROR([The program "dirname" is not available. This program is required to build Openbox.])])

dnl Pkg-config checks
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.14.0])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES([PANGO], [pango >= 1.8.0 pangoxft >= 1.8.0])
AC_SUBST(PANGO_CFLAGS)
AC_SUBST(PANGO_LIBS)

PKG_CHECK_MODULES([XML], [libxml-2.0 >= 2.6.0])
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)

dnl -------------------------------------------------------------------
dnl Optional feature: Startup Notification
dnl -------------------------------------------------------------------
AC_ARG_ENABLE([startup-notification],
  AS_HELP_STRING([--disable-startup-notification],
                 [disable the startup notification library [default=enabled]]),
  [enable_sn=$enableval],
  [enable_sn=yes]
)

sn_found=no
if test "x$enable_sn" = xyes; then
  PKG_CHECK_MODULES([LIBSN], [libstartup-notification-1.0 >= 0.8],
    [
      AC_DEFINE([USE_LIBSN], [1], [Use startup-notification])
      AC_SUBST([LIBSN_CFLAGS])
      AC_SUBST([LIBSN_LIBS])
      sn_found=yes
    ],
    [
      sn_found=no
    ]
  )
fi

dnl -------------------------------------------------------------------
dnl Optional feature: Xcursor
dnl -------------------------------------------------------------------
AC_ARG_ENABLE([xcursor],
  AS_HELP_STRING([--disable-xcursor],
                 [disable use of the X Cursor library [default=enabled]]),
  [enable_xcursor=$enableval],
  [enable_xcursor=yes]
)

xcursor_found=no
if test "x$enable_xcursor" = xyes; then
  PKG_CHECK_MODULES([XCURSOR], [xcursor],
    [
      AC_DEFINE([USE_XCURSOR], [1], [Use X Cursor library])
      AC_SUBST([XCURSOR_CFLAGS])
      AC_SUBST([XCURSOR_LIBS])
      xcursor_found=yes
    ],
    [
      xcursor_found=no
    ]
  )
fi

dnl -------------------------------------------------------------------
dnl Optional feature: Imlib2
dnl -------------------------------------------------------------------
AC_ARG_ENABLE([imlib2],
  AS_HELP_STRING([--disable-imlib2],
                 [disable use of Imlib2 for loading icons [default=enabled]]),
  [enable_imlib2=$enableval],
  [enable_imlib2=yes]
)

imlib2_found=no
if test "x$enable_imlib2" = xyes; then
  PKG_CHECK_MODULES([IMLIB2], [imlib2],
    [
      AC_DEFINE([USE_IMLIB2], [1], [Use Imlib2 image library])
      AC_SUBST([IMLIB2_CFLAGS])
      AC_SUBST([IMLIB2_LIBS])
      # export it for the pkg-config file
      PKG_CONFIG_IMLIB=imlib2
      AC_SUBST([PKG_CONFIG_IMLIB])
      imlib2_found=yes
    ],
    [
      imlib2_found=no
    ]
  )
fi

AM_CONDITIONAL([USE_IMLIB2], [test "x$imlib2_found" = xyes])

dnl -------------------------------------------------------------------
dnl Optional feature: librsvg
dnl -------------------------------------------------------------------
AC_ARG_ENABLE([librsvg],
  AS_HELP_STRING([--disable-librsvg],
                 [disable use of SVG image files [default=enabled]]),
  [enable_librsvg=$enableval],
  [enable_librsvg=yes]
)

librsvg_found=no
if test "x$enable_librsvg" = xyes; then
  PKG_CHECK_MODULES([LIBRSVG], [librsvg-2.0],
    [
      AC_DEFINE([USE_LIBRSVG], [1], [Use SVG image files])
      AC_SUBST([LIBRSVG_CFLAGS])
      AC_SUBST([LIBRSVG_LIBS])
      # export it for the pkg-config file
      PKG_CONFIG_LIBRSVG=librsvg-2.0
      AC_SUBST([PKG_CONFIG_LIBRSVG])
      librsvg_found=yes
    ],
    [
      librsvg_found=no
    ]
  )
fi

AM_CONDITIONAL([USE_LIBRSVG], [test "x$librsvg_found" = xyes])

dnl -------------------------------------------------------------------
dnl Check for session management and X11 extensions
dnl -------------------------------------------------------------------
X11_SM

EFENCE_LIBS=""
AC_SUBST(EFENCE_LIBS)

X11_EXT_XKB
X11_EXT_XRANDR
X11_EXT_SHAPE
X11_EXT_XINERAMA
X11_EXT_SYNC
X11_EXT_AUTH

dnl -------------------------------------------------------------------
dnl Generate output
dnl -------------------------------------------------------------------
AC_CONFIG_FILES([
  Makefile
  m4/Makefile
  po/Makefile.in
  obrender/obrender-3.5.pc
  obt/obt-3.5.pc
  obrender/version.h
  obt/version.h
  version.h
])

AC_CONFIG_COMMANDS([doc],        [test -d doc          || mkdir doc])
AC_CONFIG_COMMANDS([data],       [test -d data         || mkdir data])
AC_CONFIG_COMMANDS([data/xsession],
                   [test -d data/xsession || mkdir data/xsession])

AC_OUTPUT

dnl -------------------------------------------------------------------
dnl Final summary
dnl -------------------------------------------------------------------
AC_MSG_NOTICE([Compiling with these options:
  Startup Notification... $sn_found
  X Cursor Library...... $xcursor_found
  Session Management.... $SM
  Imlib2 Library........ $imlib2_found
  SVG Support (librsvg)..$librsvg_found
])

AC_MSG_NOTICE([configure complete, now type "make"])
