# X11_DEVEL()
# ---------------------------------------------------------------------
# Check for the presence of the X Window System headers and libraries.
# Sets CPPFLAGS and LIBS as appropriate, ensures -lX11 is available.
AC_DEFUN([X11_DEVEL],
[
  AC_PATH_XTRA
  if test "x$no_x" = xyes; then
    AC_MSG_ERROR([The X Window System could not be found.])
  fi

  # Save current flags
  OLDLIBS=$LIBS
  OLDCPPFLAGS=$CPPFLAGS

  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  X_LIBS="$X_PRE_LIBS $X_LIBS -lX11"
  LIBS="$LIBS $X_LIBS"

  # Check if -lX11 has XOpenDisplay
  AC_CHECK_LIB(
    [X11],
    [XOpenDisplay],
    [],
    [AC_MSG_ERROR([Could not find XOpenDisplay in -lX11.])]
  )

  # Restore old flags
  LIBS=$OLDLIBS
  CPPFLAGS=$OLDCPPFLAGS
])


# X11_EXT_XKB()
# ---------------------------------------------------------------------
# Checks for the XKB extension in X11 (X11/XKBlib.h, XkbBell, etc.).
# Defines "XKB" and sets XKB_CFLAGS, XKB_LIBS if found.
AC_DEFUN([X11_EXT_XKB],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([xkb],
    [AS_HELP_STRING([--disable-xkb],
      [build without support for XKB extension [default=enabled]])],
    [USE=$enableval], [USE=yes]
  )

  if test "x$USE" = xyes; then
    # Save current flags
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS"

    # First check the symbol in -lX11
    AC_CHECK_LIB(
      [X11],
      [XkbBell],
      [
        # Now we do a small compile/link check with X11/XKBlib.h
        AC_MSG_CHECKING([for X11/XKBlib.h and XkbBell in -lX11])
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM([[
            #include <X11/Xlib.h>
            #include <X11/Xutil.h>
            #include <X11/XKBlib.h>
          ]],
          [[
            Display *d = NULL;
            Window w = 0;
            XkbBell(d, w, 0, 0);
          ]])],
          [
            AC_MSG_RESULT([yes])
            XKB="yes"
            AC_DEFINE([XKB], [1], [Found the XKB extension])
            XKB_CFLAGS=""
            XKB_LIBS=""
            AC_SUBST([XKB_CFLAGS])
            AC_SUBST([XKB_LIBS])
          ],
          [
            AC_MSG_RESULT([no])
            XKB="no"
          ]
        )
      ],
      [
        # if AC_CHECK_LIB fails
        XKB="no"
      ]
    )

    # Restore old flags
    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for the XKB extension])
  if test "x$XKB" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_EXT_XRANDR()
# ---------------------------------------------------------------------
# Checks for the XRandR extension: X11/extensions/Xrandr.h, XRRSelectInput, etc.
# Defines "XRANDR" and sets XRANDR_CFLAGS, XRANDR_LIBS if found.
AC_DEFUN([X11_EXT_XRANDR],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([xrandr],
    [AS_HELP_STRING([--disable-xrandr],
      [build without support for XRandR extension [default=enabled]])],
    [USE=$enableval], [USE=yes]
  )

  if test "x$USE" = xyes; then
    # Save current flags
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS -lXext -lXrender -lXrandr"

    # Check function in -lXrandr
    AC_CHECK_LIB([Xrandr], [XRRSelectInput],
      [
        AC_MSG_CHECKING([for X11/extensions/Xrandr.h])
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM([[
            #include <X11/Xlib.h>
            #include <X11/extensions/Xrandr.h>
          ]],
          [[
            Display *d = NULL;
            Drawable r = 0;
            int i;
            XRRQueryExtension(d, &i, &i);
            XRRGetScreenInfo(d, r);
          ]])],
          [
            AC_MSG_RESULT([yes])
            XRANDR="yes"
            AC_DEFINE([XRANDR], [1], [Found the XRandR extension])
            XRANDR_CFLAGS=""
            XRANDR_LIBS="-lXext -lXrender -lXrandr"
            AC_SUBST([XRANDR_CFLAGS])
            AC_SUBST([XRANDR_LIBS])
          ],
          [
            AC_MSG_RESULT([no])
            XRANDR="no"
          ]
        )
      ],
      [XRANDR="no"]
    )

    # Restore old flags
    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for the XRandR extension])
  if test "x$XRANDR" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_EXT_SHAPE()
# ---------------------------------------------------------------------
# Checks for XShape (X11/extensions/shape.h, XShapeCombineShape).
# Defines "SHAPE" and sets XSHAPE_CFLAGS, XSHAPE_LIBS if found.
AC_DEFUN([X11_EXT_SHAPE],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([xshape],
    [AS_HELP_STRING([--disable-xshape],
      [build without support for XShape extension [default=enabled]])],
    [USE=$enableval], [USE=yes]
  )

  if test "x$USE" = xyes; then
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS"

    AC_CHECK_LIB([Xext], [XShapeCombineShape],
      [
        AC_MSG_CHECKING([for X11/extensions/shape.h])
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM([[
            #include <X11/Xlib.h>
            #include <X11/Xutil.h>
            #include <X11/extensions/shape.h>
          ]],
          [[
            long foo = ShapeSet;
          ]])],
          [
            AC_MSG_RESULT([yes])
            SHAPE="yes"
            AC_DEFINE([SHAPE], [1], [Found the XShape extension])
            XSHAPE_CFLAGS=""
            XSHAPE_LIBS="-lXext"
            AC_SUBST([XSHAPE_CFLAGS])
            AC_SUBST([XSHAPE_LIBS])
          ],
          [
            AC_MSG_RESULT([no])
            SHAPE="no"
          ]
        )
      ],
      [SHAPE="no"]
    )

    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for the XShape extension])
  if test "x$SHAPE" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_EXT_XINERAMA()
# ---------------------------------------------------------------------
# Checks for Xinerama (X11/extensions/Xinerama.h, XineramaQueryExtension).
# Defines "XINERAMA" and sets XINERAMA_LIBS if found.
AC_DEFUN([X11_EXT_XINERAMA],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([xinerama],
    [AS_HELP_STRING([--disable-xinerama],
      [build without support for Xinerama [default=enabled]])],
    [USE=$enableval], [USE=yes]
  )

  if test "x$USE" = xyes; then
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS -lXext"

    AC_CHECK_LIB([Xinerama], [XineramaQueryExtension],
      [
        AC_MSG_CHECKING([for X11/extensions/Xinerama.h])
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM([[
            #include <X11/Xlib.h>
            #include <X11/extensions/Xinerama.h>
          ]],
          [[
            XineramaScreenInfo foo;
          ]])],
          [
            AC_MSG_RESULT([yes])
            XINERAMA="yes"
            AC_DEFINE([XINERAMA], [1], [Enable support of the Xinerama extension])
            XINERAMA_LIBS="-lXext -lXinerama"
            AC_SUBST([XINERAMA_LIBS])
          ],
          [
            AC_MSG_RESULT([no])
            XINERAMA="no"
          ]
        )
      ],
      [XINERAMA="no"]
    )

    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for the Xinerama extension])
  if test "x$XINERAMA" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_EXT_SYNC()
# ---------------------------------------------------------------------
# Checks for XSync (X11/extensions/sync.h, XSyncInitialize).
# Defines "SYNC" and sets XSYNC_CFLAGS, XSYNC_LIBS if found.
AC_DEFUN([X11_EXT_SYNC],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([xsync],
    [AS_HELP_STRING([--disable-xsync],
      [build without support for XSync extension [default=enabled]])],
    [USE=$enableval], [USE=yes]
  )

  if test "x$USE" = xyes; then
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS"

    AC_CHECK_LIB([Xext], [XSyncInitialize],
      [
        AC_MSG_CHECKING([for X11/extensions/sync.h])
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM([[
            #include <X11/Xlib.h>
            #include <X11/Xutil.h>
            #include <X11/extensions/sync.h>
          ]],
          [[
            XSyncValueType foo;
          ]])],
          [
            AC_MSG_RESULT([yes])
            SYNC="yes"
            AC_DEFINE([SYNC], [1], [Found the XSync extension])
            XSYNC_CFLAGS=""
            XSYNC_LIBS="-lXext"
            AC_SUBST([XSYNC_CFLAGS])
            AC_SUBST([XSYNC_LIBS])
          ],
          [
            AC_MSG_RESULT([no])
            SYNC="no"
          ]
        )
      ],
      [SYNC="no"]
    )

    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for the XSync extension])
  if test "x$SYNC" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_EXT_AUTH()
# ---------------------------------------------------------------------
# Checks for Xau (X11/Xauth.h, XauReadAuth).
# Defines "AUTH" and sets XAUTH_LIBS if found.
AC_DEFUN([X11_EXT_AUTH],
[
  AC_REQUIRE([X11_DEVEL])

  # Save current flags
  OLDLIBS=$LIBS
  OLDCPPFLAGS=$CPPFLAGS

  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  LIBS="$LIBS $X_LIBS"

  AC_CHECK_LIB([Xau], [XauReadAuth],
    [
      AC_MSG_CHECKING([for X11/Xauth.h])
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM([[
          #include <X11/Xlib.h>
          #include <X11/Xutil.h>
          #include <X11/Xauth.h>
        ]], [[]])],
        [
          AC_MSG_RESULT([yes])
          AUTH="yes"
          AC_DEFINE([AUTH], [1], [Found the Xauth extension])
          XAUTH_CFLAGS=""
          XAUTH_LIBS="-lXau"
          AC_SUBST([XAUTH_CFLAGS])
          AC_SUBST([XAUTH_LIBS])
        ],
        [
          AC_MSG_RESULT([no])
          AUTH="no"
        ]
      )
    ],
    [AUTH="no"]
  )

  # Restore old flags
  LIBS=$OLDLIBS
  CPPFLAGS=$OLDCPPFLAGS

  AC_MSG_CHECKING([for the Xauth extension])
  if test "x$AUTH" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])


# X11_SM()
# ---------------------------------------------------------------------
# Checks for SMlib (session management), i.e. -lSM -lICE and <X11/SM/SMlib.h>.
# Defines USE_SM if found.
AC_DEFUN([X11_SM],
[
  AC_REQUIRE([X11_DEVEL])

  AC_ARG_ENABLE([session-management],
    [AS_HELP_STRING([--disable-session-management],
      [build without session manager support [default=enabled]])],
    [SM=$enableval], [SM=yes]
  )

  if test "x$SM" = xyes; then
    OLDLIBS=$LIBS
    OLDCPPFLAGS=$CPPFLAGS

    CPPFLAGS="$CPPFLAGS $X_CFLAGS"
    LIBS="$LIBS $X_LIBS"

    SM="no"

    AC_CHECK_LIB([SM], [SmcSaveYourselfDone],
      [
        AC_CHECK_HEADERS([X11/SM/SMlib.h], [
          SM_CFLAGS="$X_CFLAGS"
          SM_LIBS="-lSM -lICE"
          AC_DEFINE([USE_SM], [1], [Use session management])
          AC_SUBST([SM_CFLAGS])
          AC_SUBST([SM_LIBS])
          SM="yes"
        ])
      ]
    )

    LIBS=$OLDLIBS
    CPPFLAGS=$OLDCPPFLAGS
  fi

  AC_MSG_CHECKING([for session management support])
  if test "x$SM" = xyes; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])
