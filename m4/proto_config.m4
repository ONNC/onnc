dnl
dnl @synopsis PROTO_CONFIG_FILES([source],[target header], [target implement])
dnl
AC_DEFUN([PROTO_CONFIG_FILES],
[dnl
  SEARCHPATH=$(dirname "$srcdir/$1")
  FILENAME=$(basename "$srcdir/$1" .proto)
  protoc --proto_path=$SEARCHPATH \
      --cpp_out=$SEARCHPATH \
      $srcdir/$1
  if test -f "$SEARCHPATH/$FILENAME.pb.h" && test "$SEARCHPATH/$FILENAME.pb.h" != "$2"; then
    mv "$SEARCHPATH/$FILENAME.pb.h" $2
  fi
  if test -f "$SEARCHPATH/$FILENAME.pb.cc" && test "$SEARCHPATH/$FILENAME.pb.cc" != "$3"; then
    mv "$SEARCHPATH/$FILENAME.pb.cc" $3
  fi
])
