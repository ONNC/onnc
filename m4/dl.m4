dnl
dnl @synopsis CHECK_DL
dnl
dnl @summary check libdl library and setup variables:
dnl   HAVE_DL
dnl
dnl Luba Tang <luba@skymizer.com>
dnl

AC_DEFUN([CHECK_DL],
[dnl
  AC_SEARCH_LIBS([dlopen], [dl dld],
    [dnl found
      HAVE_DL=1
    ],
    [AC_MSG_RESULT([unable to find the dlopen() function])]
  )
])
