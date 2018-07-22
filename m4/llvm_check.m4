dnl
dnl @synopsis CHECK_LLVM
dnl
dnl @summary check --with-llvm=<installdir>, and setup variables:
dnl   LLVM_INCLUDES="-I${llvm_dir}/include"
dnl   LLVM_LDFLAGS="-L${llvm_dir}/lib"
dnl
dnl Luba Tang <luba@skymizer.com>
dnl
AC_DEFUN([CHECK_LLVM],
[dnl

AC_ARG_WITH([llvm],
  [AC_HELP_STRING([--with-llvm=PREFIX],
    [use LLVM on the given path PREFIX])],
  [llvm_dir="${withval}"],
  [llvm_dir="${prefix}"])

AC_MSG_CHECKING(LLVM)

HAVE_LLVM=0

AC_LANG_PUSH([C++])
orig_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-std=c++11 -I${llvm_dir}/include"

AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
    #include <llvm/Pass.h>
  ]], [])],
  [dnl action if true
    HAVE_LLVM=1
    AC_MSG_RESULT([ok])
  ],
  [dnl action if else
    AC_MSG_FAILURE([failed to find LLVM headers])
  ]
)

CXXFLAGS="${orig_CXXFLAGS}"
AC_LANG_POP([C++])

LLVM_INCLUDES="-I${llvm_dir}/include"
LLVM_LDFLAGS="-L${llvm_dir}/lib"
LLVM_LIBS="$(${llvm_dir}/bin/llvm-config --libs) \
$(${llvm_dir}/bin/llvm-config --system-libs)"
CLANG_LIBS="-lclangTooling \
-lclangToolingCore \
-lclangFrontendTool \
-lclangCodeGen \
-lclangRewriteFrontend \
-lclangARCMigrate \
-lclangStaticAnalyzerFrontend \
-lclangFrontend \
-lclangDriver \
-lclangParse \
-lclangSerialization \
-lclangSema \
-lclangEdit \
-lclangStaticAnalyzerCheckers \
-lclangASTMatchers \
-lclangStaticAnalyzerCore \
-lclangAnalysis \
-lclangAST \
-lclangRewrite \
-lclangLex \
-lclangBasic"

AC_SUBST(LLVM_INCLUDES)
AC_SUBST(LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS)
AC_SUBST(CLANG_LIBS)

AM_CONDITIONAL([HAVE_LLVM], [test "${HAVE_LLVM}" = 1])
])
