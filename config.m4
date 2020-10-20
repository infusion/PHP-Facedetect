PHP_ARG_WITH(facedetect, for facedetect support, [  --with-facedetect     Enable facedetect support])

if test "$PHP_FACEDETECT" != "no"; then
  PHP_REQUIRE_CXX()
  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  AC_MSG_CHECKING(for opencv)
  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists opencv; then
    CV_INCLUDE=`$PKG_CONFIG opencv --variable=includedir_new`
    CV_LIBRARY=`$PKG_CONFIG opencv --libs`
    CV_VERSION=`$PKG_CONFIG opencv --modversion`
    if $PKG_CONFIG opencv --atleast-version=3.0.0 ; then
      AC_MSG_RESULT($CV_VERSION)
    else
      AC_MSG_ERROR(opencv version is too old.)
    fi
    PHP_EVAL_LIBLINE($CV_LIBRARY, FACEDETECT_SHARED_LIBADD)
    PHP_ADD_INCLUDE($CV_INCLUDE)
  else
    if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists opencv4; then
      CV_INCLUDE=`$PKG_CONFIG opencv4 --variable=includedir`
      CV_LIBRARY=`$PKG_CONFIG opencv4 --libs`
      CV_VERSION=`$PKG_CONFIG opencv4 --modversion`
      if $PKG_CONFIG opencv4 --atleast-version=3.0.0 ; then
        AC_MSG_RESULT($CV_VERSION)
      else
        AC_MSG_ERROR(opencv version is too old)
      fi
      PHP_EVAL_LIBLINE($CV_LIBRARY, FACEDETECT_SHARED_LIBADD)
      PHP_ADD_INCLUDE($CV_INCLUDE)
    else
      AC_MSG_ERROR(Please reinstall opencv)
    fi
  fi

  PHP_SUBST(FACEDETECT_SHARED_LIBADD)
  AC_DEFINE(HAVE_FACEDETECT, 1, [ ])
  PHP_NEW_EXTENSION(facedetect, facedetect.cc, $ext_shared)
fi
