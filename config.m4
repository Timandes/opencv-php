dnl $Id$
dnl config.m4 for extension opencv

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(opencv, for opencv support,
Make sure that the comment is aligned:
[  --with-opencv             Include opencv support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(opencv, whether to enable opencv support,
dnl Make sure that the comment is aligned:
dnl [  --enable-opencv           Enable opencv support])

if test "$PHP_OPENCV" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-opencv -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/opencv2/imgproc/imgproc.hpp"
  if test -r $PHP_OPENCV/$SEARCH_FOR; then # path given as parameter
    OPENCV_DIR=$PHP_OPENCV
  else # search default path list
    AC_MSG_CHECKING([for opencv files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        OPENCV_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$OPENCV_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the opencv distribution])
  fi

  dnl # --with-opencv -> add include path
  PHP_ADD_INCLUDE($OPENCV_DIR/include)

  dnl # --with-opencv -> check for lib and symbol presence
  LIBNAME=opencv_highgui
  LIBSYMBOL=cvConvertImage

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OPENCV_DIR/$PHP_LIBDIR, OPENCV_SHARED_LIBADD)
    AC_DEFINE(HAVE_OPENCVLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong opencv lib version or lib not found])
  ],[
    -L$OPENCV_DIR/$PHP_LIBDIR -lm
  ])

  dnl # --with-opencv -> check for lib and symbol presence
  LIBNAME=opencv_photo
  LIBSYMBOL=cvInpaint

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OPENCV_DIR/$PHP_LIBDIR, OPENCV_SHARED_LIBADD)
    AC_DEFINE(HAVE_OPENCVLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong opencv lib version or lib not found])
  ],[
    -L$OPENCV_DIR/$PHP_LIBDIR -lm
  ])

  PHP_SUBST(OPENCV_SHARED_LIBADD)

  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, OPENCV_SHARED_LIBADD)
  PHP_NEW_EXTENSION(opencv, opencv.cc mat.cc, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
