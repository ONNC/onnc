dnl
dnl @synopsis CHECK_SKYPAT
dnl
dnl @summary check --with-skypat=<installdir>, and setup variables:
dnl   PAT_INCLUDES="-I${pat_dir}/include"
dnl   PAT_LIBS="${pat_dir}/lib/libpat.a"
dnl
dnl Yi Huang <yi@skymizer.com>
dnl
AC_DEFUN([CHECK_SKYPAT],
[dnl

AC_ARG_WITH([skypat],
  [AS_HELP_STRING([--with-skypat@<:@=DIR@:>@],
    [use SkyPat on DIR, or PREFIX if not given])],
  [pat_dir="${withval}"],
  [pat_dir="${prefix}"])

AC_MSG_CHECKING(SkyPat)

HAVE_SKYPAT=0

AC_LANG_PUSH([C++])
orig_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-I${pat_dir}/include"

AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
    #include <pat/pat.h>
    #if !defined(PAT_VERNUM)
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

PAT_INCLUDES="-I${pat_dir}/include"
PAT_LIBS="${pat_dir}/lib/libpat.a"

AC_SUBST(PAT_INCLUDES)
AC_SUBST(PAT_LIBS)

AM_CONDITIONAL([HAVE_SKYPAT], [test ${HAVE_SKYPAT} = 1])
])
