dnl
dnl @synopsis PROTO_CONFIG_FILES([source],[target header], [target implement])
dnl
AC_DEFUN([PROTO_CONFIG_FILES],
[dnl
  SEARCHPATH=$(dirname "$srcdir/$1")
  PLACEHOLDER=$(dirname "$srcdir/$3")
  FILENAME=$(basename "$srcdir/$1" .proto)
  protoc --proto_path=\$SEARCHPATH \
      --cpp_out=\$PLACEHOLDER \
      $srcdir/$1
  if test -f "\$PLACEHOLDER/\$FILENAME.pb.h"; then
    mv "\$PLACEHOLDER/\$FILENAME.pb.h" $2
  fi
])
