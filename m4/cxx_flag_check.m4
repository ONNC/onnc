dnl
dnl @synopsis CXX_FLAGS_CHECK [compiler flags]
dnl
dnl @summary check whether compiler supports given C++ flags or not
dnl
dnl Luba Tang <lubatang@gmail.com>
AC_DEFUN([CXX_FLAG_CHECK],
[dnl
  AC_MSG_CHECKING([if $CXX supports $1])
  AC_LANG_PUSH([C++])
  ac_saved_cxxflags="$CXXFLAGS"
  CXXFLAGS="-Werror $1"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_ERROR([no])]
  )
  CXXFLAGS="$ac_saved_cxxflags"
  AC_LANG_POP([C++])
])
