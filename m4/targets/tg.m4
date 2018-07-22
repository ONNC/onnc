PROTO_CONFIG_FILES([lib/Target/TG/BM188x/proto/common_calibration2.proto],
    [lib/Target/TG/BM188x/common_calibration2.pb.h],
    [lib/Target/TG/BM188x/common_calibration2.pb.cc])

PROTO_CONFIG_FILES([lib/Target/TG/BM188x/asm/bm188x_asm.proto],
    [lib/Target/TG/BM188x/bm188x_asm.pb.h],
    [lib/Target/TG/BM188x/asm.pb.cc])

AC_CONFIG_FILES([include/onnc/Target/TG/io.hpp:$srcdir/lib/Target/TG/io.hpp])
AC_CONFIG_FILES([include/onnc/Target/TG/bmkernel_api_base.h:$srcdir/lib/Target/TG/bmkernel_api_base.h])
AC_CONFIG_FILES([include/onnc/Target/TG/BM188x/bmkernel_api.h:$srcdir/lib/Target/TG/BM188x/bmkernel_api.h])
AC_CONFIG_FILES([include/onnc/Target/TG/BM188x/asm.h:$srcdir/lib/Target/TG/BM188x/asm.h])
AC_CONFIG_FILES([include/onnc/Target/TG/BM168x/bmkernel_api.h:$srcdir/lib/Target/TG/BM168x/bmkernel_api.h])
