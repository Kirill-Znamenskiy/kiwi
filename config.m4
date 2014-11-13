dnl $Id$
dnl config.m4 for extension Kiwi

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(kiwi, for kiwi support,
dnl Make sure that the comment is aligned:
dnl [  --with-kiwi             Include kiwi support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(
  Kiwi,
  whether to enable kiwi support,
  Make sure that the comment is aligned: [ --enable-kiwi Enable Kiwi support]
)

if test "$PHP_KIWI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-kiwi -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/kiwi.h"  # you most likely want to change this
  dnl if test -r $PHP_KIWI/$SEARCH_FOR; then # path given as parameter
  dnl   KIWI_DIR=$PHP_KIWI
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for kiwi files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       KIWI_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$KIWI_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the kiwi distribution])
  dnl fi

  dnl # --with-kiwi -> add include path
  dnl PHP_ADD_INCLUDE($KIWI_DIR/include)

  dnl # --with-kiwi -> check for lib and symbol presence
  dnl LIBNAME=kiwi # you may want to change this
  dnl LIBSYMBOL=kiwi # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $KIWI_DIR/lib, KIWI_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_KIWILIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong kiwi lib version or lib not found])
  dnl ],[
  dnl   -L$KIWI_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(KIWI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(Kiwi, Kiwi.c, $ext_shared)
fi
