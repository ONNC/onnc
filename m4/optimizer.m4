dnl
dnl @synopsis ENABLE_TUNER
dnl
dnl @summary check --enable-tuner=<tuner_mode>, and setup variables:
dnl   USE_<tuner_mode>_TUNER="<tuner_mode>"
dnl
dnl Hung Ying Tai <hydai@skymizer.com>
dnl

AC_DEFUN([CHECK_ENABLE_TUNER],
[dnl

AC_ARG_ENABLE([tuner],
  AS_HELP_STRING([--enable-tuner=<tuner_mode>], [set default tuner]))

AC_MSG_CHECKING([tuner to enable])

AS_IF([test "x$enable_tuner" != "xno"],
  [
    case "$enable_tuner" in
    diid)
      AC_MSG_RESULT([DIID enabled])
      AC_DEFINE([USE_DIID_TUNER],[1],[Define if we're using DIID tuner])
    ;;
    random)
      AC_MSG_RESULT([Random enabled])
      AC_DEFINE([USE_RANDOM_TUNER],[1],[Define if we're using Random tuner])
    ;;
    esac
  ], [
    AC_MSG_ERROR([invalid option, use --enable-tuner=<diid/random>.])
  ])
])
