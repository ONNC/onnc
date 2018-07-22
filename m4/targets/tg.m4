AC_CONFIG_FILES([include/onnc/Target/TG/BM188x/common_calibration2.pb.h:$srcdir/lib/Target/TG/BM188x/proto/common_calibration2.pb.h],
  [dnl commands
  ],
  [dnl init commands
  PROTO_CONFIG_FILES([lib/Target/TG/BM188x/proto/common_calibration2.proto],
      [include/onnc/Target/TG/BM188x/common_calibration2.pb.h],
      [lib/Target/TG/BM188x/common_calibration2.pb.cc])
  ])
