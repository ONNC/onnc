dnl
dnl @synopsis CHECK_BMKERNEL
dnl
dnl @summary check --with-bmkernel=<installdir>, and setup variables:
dnl   BMKERNEL_INCLUDES="-I${bmkernel_dir}/include"
dnl   BMKERNEL_LIBS="${bmkernel_dir}/lib/libbmkernel.a"
dnl
dnl arcbbb <shihpo.hung@bitmain.com>
dnl
AC_DEFUN([CHECK_BMKERNEL],
[dnl

AC_ARG_WITH([bmkernel],
  [AS_HELP_STRING([--with-bmkernel@<:@=DIR@:>@],
    [use bmkernel from DIR, or PREFIX if not given])],
  [bmkernel_dir="${withval}"],
  [bmkernel_dir="${prefix}"])

AC_MSG_CHECKING(bmkernel)

HAVE_BMKERNEL=0

AC_LANG_PUSH([C++])
orig_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-I${bmkernel_dir}/include"
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
    #include <bm_kernel.h>
  ]], [])],
  [dnl action if true
    HAVE_BMKERNEL=1
    AC_MSG_RESULT([ok])
  ],
  [dnl action if else
    AC_MSG_FAILURE([failed to find bmkernel headers])
  ]
)

CXXFLAGS="${orig_CXXFLAGS}"
AC_LANG_POP([C++])

BMKERNEL_INCLUDES="-I${bmkernel_dir}/include"
BMKERNEL_LIBS="-L${bmkernel_dir}/lib -lbmtap -lpthread"

dnl TODO: workaround for include & lib path, should export bmapi in install folder
BMAPI_INCLUDES="-I${bmkernel_dir}/../bmnet/src/targets/plat-bm168x/bmkernel_api_origin"
BMAPI_LIBS="-L${bmkernel_dir}/../out_bmnet/src/targets/.libs -ltargets"

AC_SUBST(BMKERNEL_INCLUDES)
AC_SUBST(BMKERNEL_LIBS)
AC_SUBST(BMAPI_INCLUDES)
AC_SUBST(BMAPI_LIBS)

AM_CONDITIONAL([HAVE_BMKERNEL], [test ${HAVE_BMKERNEL} = 1])
])
