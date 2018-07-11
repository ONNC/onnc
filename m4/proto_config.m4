dnl
dnl @synopsis PROTO_CONFIG_FILES([source],[target])
dnl
AC_DEFUN([PROTO_CONFIG_FILES],
[dnl
  AS_IF([test -f "$srcdir/$1"], [
      SEARCHPATH=$(dirname "$srcdir/$1")
      protoc --proto_path=${SEARCHPATH} --cpp_out=$srcdir/$2 $srcdir/$1
      AC_MSG_RESULT([creating $1])
    ],
    [AC_MSG_FAILURE([proto text file $srcdir/$1 not found])])
])
