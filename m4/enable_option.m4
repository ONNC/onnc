dnl
dnl @synposis CHECK_ENABLE_OPTION[option, value, if yes]
dnl
dnl $1: option name
dnl $2: default value ("yes" or "no")
dnl $3: define $3 if yes
dnl
dnl @summary check --enable-`option`=[yes or no]
dnl
AC_DEFUN([CHECK_ENABLE_OPTION],
[
  AC_ARG_ENABLE($1,
    [AS_HELP_STRING([--enable-$1],
      [turn on $1 (default is $2)])],
    [],
    [enable_$1=$2])
  AM_CONDITIONAL([$3],[test "x${enable_$1}" == "xyes" ])
  AS_IF([test "x${enable_$1}" = "xyes"],
    [AC_DEFINE($3, 1, [Enable $1])]
  )
])
