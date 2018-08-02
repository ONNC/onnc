dnl
dnl @synopsis CHECK_SKYPAT
dnl
dnl @summary check --with-skypat=<installdir>, and setup variables:
dnl   SKYPAT_INCLUDES="-I${skypat_dir}/include"
dnl   SKYPAT_LIBS="${skypat_dir}/lib/libskypat.a"
dnl
dnl Yi Huang <yi@skymizer.com>
dnl
AC_DEFUN([CHECK_SKYPAT],
[dnl

AC_ARG_WITH([skypat],
  [AS_HELP_STRING([--with-skypat@<:@=DIR@:>@],
    [use SkyPat on DIR, or PREFIX if not given])],
  [skypat_dir="$(realpath ${withval})"],
  [skypat_dir="${prefix}"])

AC_MSG_CHECKING(SkyPat)

HAVE_SKYPAT=0

AC_LANG_PUSH([C++])
orig_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-I${skypat_dir}/include"

AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
    #include <skypat/skypat.h>
    #if !defined(SKYPAT_VERNUM)
    #error SkyPat version is too old
    #endif
  ]], [])],
  [dnl action if true
    HAVE_SKYPAT=1
    AC_MSG_RESULT([ok])
  ],
  [dnl action if else
    AC_MSG_FAILURE([failed to find SkyPat headers])
  ]
)

CXXFLAGS="${orig_CXXFLAGS}"
AC_LANG_POP([C++])

SKYPAT_INCLUDES="-I${skypat_dir}/include"
SKYPAT_LIBS="${skypat_dir}/lib/libskypat.a"

AC_SUBST(SKYPAT_INCLUDES)
AC_SUBST(SKYPAT_LIBS)

AM_CONDITIONAL([HAVE_SKYPAT], [test ${HAVE_SKYPAT} = 1])
])
