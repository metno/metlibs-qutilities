# Checks for QtCore, QtGui, QtNetwork and Qt3Support
# source: GNU Classpath project (HEAD/configure.ac)
# MLS 20080211

AC_DEFUN([HAVE_QT4_CHECK], 
[dnl Check for AWT related Qt4
PKG_CHECK_MODULES(QT, QtCore QtGui QtNetwork Qt3Support >= 4.1.0, HAVE_QT4="yes", HAVE_QT4="no")
if test "x$HAVE_QT4" = "xyes"; then
  dnl Check needed because in some cases the QtGui includedir
  dnl doesn't contain the subsystem dir.
  QT_INCLUDE_DIR=`$PKG_CONFIG --variable=includedir QtGui`
  EXTRA_QT_INCLUDE_DIR="$QT_INCLUDE_DIR/Qt"
  AC_CHECK_FILE([$QT_INCLUDE_DIR/QWidget],
  AC_MSG_NOTICE([No extra QT_INCLUDE_DIR needed]),
  AC_CHECK_FILE([$EXTRA_QT_INCLUDE_DIR/QWidget],
QT_CFLAGS="$QT_CFLAGS -I$EXTRA_QT_INCLUDE_DIR",
AC_MSG_WARN([QWidget not found])))
AC_CHECK_PROG(MOC, [moc-qt4], [moc-qt4])
AC_CHECK_PROG(MOC, [moc], [moc])
fi
if test "x$HAVE_QT4" = "xno"; then
  AC_MSG_NOTICE([Looking for QT_CFLAGS and QT_LIBS without pkg-config])
  case "$host_os" in
darwin*)
  AC_ARG_WITH([qt4dir],
    [AS_HELP_STRING([--with-qt4dir=DIR],
    [Qt4 installation directory used for OS-X.
    For other systems use pkg-config.])],
    [QT4DIR=$withval]
    )
  if test x"$QT4DIR" = x ; then
    AC_MSG_ERROR([*** No path for Qt4 --with-qt4dir option given])
  fi
  AC_MSG_RESULT([QT4DIR... $QT4DIR])
  AC_CHECK_PROG(MOC, [moc], [$QT4DIR/bin/moc], [], $QT4DIR/bin)
  if test x"$MOC" = x; then
    AC_MSG_ERROR([*** This is not the right Qt installation])
  fi
  QT_CFLAGS="-F$QT4DIR/lib -I$QT4DIR/lib/QtCore.framework/Headers"
  QT_CFLAGS="$QT_CFLAGS -I$QT4DIR/lib/QtGui.framework/Headers"
  QT_CFLAGS="$QT_CFLAGS -I$QT4DIR/lib/QtNetwork.framework/Headers"
  QT_CFLAGS="$QT_CFLAGS -I$QT4DIR/lib/Qt3Support.framework/Headers"
  QT_LIBS="-Xlinker -F$QT4DIR/lib -Xlinker -framework -Xlinker QtCore"
  QT_LIBS="$QT_LIBS -Xlinker -framework -Xlinker QtGui"
  QT_LIBS="$QT_LIBS -Xlinker -framework -Xlinker QtNetwork"
  QT_LIBS="$QT_LIBS -Xlinker -framework -Xlinker Qt3Support"
  ;;
*)
  AC_MSG_ERROR([*** Please check PKG_CONFIG_PATH or the version
  of your installed Qt4 installation.])
  ;;
  esac
fi
AC_MSG_NOTICE([Set QT_CFLAGS... $QT_CFLAGS])
AC_SUBST(QT_CFLAGS)
AC_SUBST(QT_LIBS)
])

