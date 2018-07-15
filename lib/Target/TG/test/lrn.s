#; RUN : onnx-as lrn.s | onnx2tg -march bm1880 -S -print-module-before-isel  -add-dummy-weight | FileCheck lrn.s

#; CHECK: inst {
#; CHECK-NEXT:   name: "conv2/norm2_1"
#; CHECK-NEXT:   type: "bmnet_lrn_fixed_forward_bmkernel"
#; CHECK-NEXT:   lrn {
#; CHECK-NEXT:     bottom_gaddr: 0
#; CHECK-NEXT:     top_gaddr: {{.*}}
#; CHECK-NEXT:     sqr_lut_gaddr: 0
#; CHECK-NEXT:     power_lut_gaddr: 256
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
