# OB_DEBUG()
# ---------------------------------------------------------------------
# Check if the user has requested a debug build, strict ANSI, super warnings,
# gprof profiling, or gprof profiling with a special libc.
# Sets the DEBUG (or NDEBUG) variable as appropriate, plus a summary message.
AC_DEFUN([OB_DEBUG],
[
  AC_MSG_CHECKING([build type])

  # --enable-strict-ansi
  AC_ARG_ENABLE([strict-ansi],
    [AS_HELP_STRING([--enable-strict-ansi],
      [Enable strict ANSI compliance build [[default=no]]])],
    [STRICT=$enableval],
    [STRICT=no]
  )

  # --enable-super-warnings
  AC_ARG_ENABLE([super-warnings],
    [AS_HELP_STRING([--enable-super-warnings],
      [Enable extra compiler warnings [[default=no]]])],
    [SUPERWARN=$enableval],
    [SUPERWARN=no]
  )

  # --enable-debug
  AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
      [build a debug version [[default=no]]])],
    [DEBUG=$enableval],
    [DEBUG=no]
  )

  # --enable-gprof
  AC_ARG_ENABLE([gprof],
    [AS_HELP_STRING([--enable-gprof],
      [Enable gprof profiling output [[default=no]]])],
    [PROF=$enableval],
    [PROF=no]
  )

  # --enable-gprof-libc
  AC_ARG_ENABLE([gprof-libc],
    [AS_HELP_STRING([--enable-gprof-libc],
      [Link against libc with profiling support [[default=no]]])],
    [PROFLC=$enableval],
    [PROFLC=no]
  )

  # If we enable gprof-libc, also enable gprof in general
  if test "x$PROFLC" = xyes; then
    PROF=yes
  fi

  # If the PACKAGE_VERSION contains alpha/beta, treat it as a test release
  TEST_RELEASE=no
  if test "${PACKAGE_VERSION%*alpha*}" != "$PACKAGE_VERSION"; then
    TEST_RELEASE=yes
  fi
  if test "${PACKAGE_VERSION%*beta*}" != "$PACKAGE_VERSION"; then
    TEST_RELEASE=yes
  fi

  # If test release, default DEBUG to yes
  if test "x$TEST_RELEASE" = xyes; then
    DEBUG=yes
  fi

  # Build up a summary message
  if test "x$DEBUG" = xyes; then
    MSG="DEBUG"
  else
    MSG="RELEASE"
  fi

  if test "x$TEST_RELEASE" = xyes; then
    MSG="$MSG (test release)"
  fi
  if test "x$STRICT" = xyes; then
    MSG="$MSG with strict ANSI compliance"
  fi
  if test "x$SUPERWARN" = xyes; then
    MSG="$MSG with super warnings"
  fi

  AC_MSG_RESULT([$MSG])

  # If debugging is enabled, define DEBUG=1
  if test "x$DEBUG" = xyes; then
    AC_DEFINE([DEBUG], [1], [Creating a debug build])
  fi
])


# OB_COMPILER_FLAGS()
# ---------------------------------------------------------------------
# Check what compiler is being used, adjust CFLAGS and LIBS for debug or release.
# Also handle profiling, strict-ansi, and super warnings for GNU CC.
AC_DEFUN([OB_COMPILER_FLAGS],
[
  # Ensure we have a compiler
  AC_REQUIRE([AC_PROG_CC])

  FLAGS=""
  L=""

  # If DEBUG is set by OB_DEBUG macro, define flags accordingly
  if test "x$DEBUG" = xyes; then
    FLAGS="-DDEBUG"
  else
    FLAGS="-DNDEBUG -DG_DISABLE_ASSERT"
  fi

  AC_MSG_CHECKING([for GNU CC])
  if test "x$GCC" = xyes; then
    AC_MSG_RESULT([yes])

    # If debugging, set typical debug flags
    if test "x$DEBUG" = xyes; then
      FLAGS="$FLAGS -O0 -ggdb -fno-inline -Wwrite-strings"
      FLAGS="$FLAGS -Wall -Wsign-compare -Waggregate-return"
      FLAGS="$FLAGS -Wbad-function-cast -Wpointer-arith"
      FLAGS="$FLAGS -Wno-write-strings"
    fi

    # If super warnings requested, add more
    if test "x$SUPERWARN" = xyes; then
      # glib can’t handle -Wcast-qual, but if you can, keep it:
      FLAGS="$FLAGS -Wcast-qual -Wextra"
    fi

    # If strict ANSI requested, add -ansi etc.
    if test "x$STRICT" = xyes; then
      FLAGS="$FLAGS -ansi -pedantic -D_XOPEN_SOURCE"
    fi

    # Profiling
    if test "x$PROF" = xyes; then
      FLAGS="$FLAGS -pg -fno-inline"
    fi
    if test "x$PROFLC" = xyes; then
      L="$L -lc_p -lm_p"
    fi

    # Additional recommended flag
    FLAGS="$FLAGS -fno-strict-aliasing"
  else
    AC_MSG_RESULT([no])
  fi

  AC_MSG_CHECKING([for compiler-specific flags])
  AC_MSG_RESULT([$FLAGS])

  # Append to CFLAGS and LIBS
  CFLAGS="$CFLAGS $FLAGS"
  LIBS="$LIBS $L"
])


# OB_NLS()
# ---------------------------------------------------------------------
# Enable or disable NLS translations (using gettext).
AC_DEFUN([OB_NLS],
[
  AC_ARG_ENABLE([nls],
    [AS_HELP_STRING([--enable-nls],
      [Enable NLS translations [[default=yes]]])],
    [NLS=$enableval],
    [NLS=yes]
  )

  if test "x$NLS" = xyes; then
    # Enable i18n
    DEFS="$DEFS -DENABLE_NLS"
  fi
])
