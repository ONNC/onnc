# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-weight -debug-only=quantize_lrn -o=- | FileCheck %s

#; CHECK:[conv2/norm2_1_sqrlut] sqLutContent dump for ctest:
#; CHECK-NEXT:00  00  00  01  01  02  02  03  04  05  07  08  0A  0B  0D  0F
#; CHECK-NEXT:11  13  16  18  1B  1E  21  24  27  2A  2D  31  35  38  3C  41
#; CHECK-NEXT:45  49  4E  52  57  5C  61  66  6B  71  76  7C  82  88  8E  94
#; CHECK-NEXT:9B  A1  A8  AF  B6  BD  C4  CB  D3  DA  E2  EA  F2  FA  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF
#; CHECK-NEXT:FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF  FF

#; CHECK:[conv2/norm2_1_powerlut] powerLutContent dump for ctest:
#; CHECK-NEXT:FF  FF  FE  FD  FC  FC  FB  FA  F9  F8  F7  F6  F6  F5  F4  F3
#; CHECK-NEXT:F2  F2  F1  F0  EF  EF  EE  ED  EC  EC  EB  EA  E9  E9  E8  E7
#; CHECK-NEXT:E7  E6  E5  E5  E4  E3  E2  E2  E1  E0  E0  DF  DF  DE  DD  DD
#; CHECK-NEXT:DC  DB  DB  DA  DA  D9  D8  D8  D7  D7  D6  D5  D5  D4  D4  D3
#; CHECK-NEXT:D2  D2  D1  D1  D0  D0  CF  CF  CE  CE  CD  CC  CC  CB  CB  CA
#; CHECK-NEXT:CA  C9  C9  C8  C8  C7  C7  C6  C6  C5  C5  C4  C4  C3  C3  C2
#; CHECK-NEXT:C2  C2  C1  C1  C0  C0  BF  BF  BE  BE  BD  BD  BD  BC  BC  BB
#; CHECK-NEXT:BB  BA  BA  BA  B9  B9  B8  B8  B7  B7  B7  B6  B6  B5  B5  B5
#; CHECK-NEXT:B4  B4  B3  B3  B3  B2  B2  B2  B1  B1  B0  B0  B0  AF  AF  AF
#; CHECK-NEXT:AE  AE  AD  AD  AD  AC  AC  AC  AB  AB  AB  AA  AA  AA  A9  A9
#; CHECK-NEXT:A9  A8  A8  A8  A7  A7  A7  A6  A6  A6  A5  A5  A5  A4  A4  A4
#; CHECK-NEXT:A3  A3  A3  A2  A2  A2  A2  A1  A1  A1  A0  A0  A0  9F  9F  9F
#; CHECK-NEXT:9F  9E  9E  9E  9D  9D  9D  9D  9C  9C  9C  9B  9B  9B  9B  9A
#; CHECK-NEXT:9A  9A  9A  99  99  99  98  98  98  98  97  97  97  97  96  96
#; CHECK-NEXT:96  96  95  95  95  95  94  94  94  94  93  93  93  93  92  92
#; CHECK-NEXT:92  92  91  91  91  91  90  90  90  90  90  8F  8F  8F  8F  8E

#; CHECK: inst {
#; CHECK-NEXT:   name: "conv2/norm2_1"
#; CHECK-NEXT:   type: "bmnet_lrn_fixed_forward_bmkernel"
#; CHECK-NEXT:   lrn {
#; CHECK-NEXT:     bottom_gaddr: 0
#; CHECK-NEXT:     top_gaddr: {{.*}}
#; CHECK-NEXT:     sqr_lut_gaddr: 0
#; CHECK-NEXT:     power_lut_gaddr: 8192
#; CHECK-NEXT:     input_n: 10
#; CHECK-NEXT:     input_c: 192
#; CHECK-NEXT:     input_h: 56
#; CHECK-NEXT:     input_w: 56
#; CHECK-NEXT:     size: 5
#; CHECK-NEXT:     sum_right_shift_width: 7
#; CHECK-NEXT:     lrn_right_shift_width: 13
#; CHECK-NEXT:     threshold_x_quantized: 202
#; CHECK-NEXT:     threshold_x_quantized: 147
#; CHECK-NEXT:   }
#; CHECK-NEXT: }

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "GoogleNet"
  doc_string: ""
  node { input: "conv2/relu_3x3_1" output: "conv2/norm2_1" name: "conv2/norm2_1" op_type: "LRN" attribute { name: "size" i: 5 type: INT } attribute { name: "alpha" f: 0.0001 type: FLOAT } attribute { name: "beta" f: 0.75 type: FLOAT } attribute { name: "bias" f: 1 type: FLOAT } }
  input { name: "conv2/relu_3x3_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } dim { dim_value: 192 } dim { dim_value: 56 } dim { dim_value: 56 } } } } }
  output { name: "conv2/norm2_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } dim { dim_value: 192 } dim { dim_value: 56 } dim { dim_value: 56 } } } } }
  
}
opset_import { domain: "" version: 8 }
metadata_props { key: "bm1880_ctable" value: "layer {\n  name: \"conv2/relu_3x3_1\"\n  blob_param {\n    name: \"conv2/relu_3x3_1\"\n    threshold_y: 636.08325\n  }\n}\nlayer {\n  name: \"conv2/norm2_1\"\n  threshold_y: 138.79416\n  blob_param {\n    name: \"conv2/norm2_1\"\n    threshold_y: 138.79416\n  }\n  blob_param {\n    name: \"sq\"\n    threshold_y: 1.8825117\n  }\n  blob_param {\n    name: \"scale\"\n    threshold_y: 1.0004892\n  }\n  blob_param {\n    name: \"sum_sq\"\n    threshold_y: 1.1900998\n  }\n}\n" }
