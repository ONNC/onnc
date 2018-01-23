dnl
dnl @synopsis CHECK_WERROR_FLAGS
dnl
dnl @summary check compiler's warning flags.
dnl
dnl Luba Tang <lubatang@gmail.com>
dnl From Kito Cheng <kito@0xlab.org>
AC_DEFUN([CHECK_WERROR_FLAGS],
[dnl
  AC_ARG_ENABLE(werror,
    AS_HELP_STRING([--enable-werror],
      [Compile with -Werror enabled (default is NO)]),
      [case "${enableval}" in
        yes) enable_werror=true  ;;
        no)  enable_werror=false ;;
        *) AC_MSG_ERROR([Invalid setting for --enable-werror. Use "yes" or "no"]) ;;
       esac],
      [enable_werror="no"])

AM_CONDITIONAL([ENABLE_WERROR], [test x${enable_werror} = xtrue])

])
