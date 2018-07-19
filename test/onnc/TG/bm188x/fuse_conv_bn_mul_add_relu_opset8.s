# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight -o=- | FileCheck %s
#; CHECK: FLOAT tensor <1, 64, 112, 112> %conv1_7x7_s2_bn_sc_2 = Conv <strides:INTS [2,2], pads:INTS [3,3,3,3], kernel_shape:INTS [7,7], do_scale:INT 1, do_scale_bias:INT 1, conv_output_threshold:FLOAT 1, do_relu:INT 1> (FLOAT tensor <1, 3, 224, 224> %data_0, FLOAT tensor <64, 3, 7, 7> %conv1_7x7_s2_w_0, FLOAT tensor <64> %22, FLOAT tensor <64> %24)

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "inception_v2"
  doc_string: ""
  node { input: "data_0" input: "conv1_7x7_s2_w_0" output: "conv1_7x7_s2_1" name: "conv1_7x7_s2_1" op_type: "Conv" attribute { name: "strides" ints: 2 ints: 2 type: INTS } attribute { name: "pads" ints: 3 ints: 3 ints: 3 ints: 3 type: INTS } attribute { name: "kernel_shape" ints: 7 ints: 7 type: INTS } }
  node { input: "conv1_7x7_s2_1" input: "conv1_7x7_s2_bn_scale_0" input: "conv1_7x7_s2_bn_bias_0" input: "conv1_7x7_s2_bn_mean_0" input: "conv1_7x7_s2_bn_var_0" output: "conv1_7x7_s2_bn_1" name: "conv1_7x7_s2_bn_1" op_type: "BatchNormalization" attribute { name: "epsilon" f: 1e-05 type: FLOAT } }
  node { input: "conv1_7x7_s2_bn_sc_w_0" output: "OC2_DUMMY_0" name: "OC2_DUMMY_0" op_type: "Unsqueeze" attribute { name: "axes" ints: 1 ints: 2 type: INTS } }
  node { input: "conv1_7x7_s2_bn_1" input: "OC2_DUMMY_0" output: "conv1_7x7_s2_bn_sc_internal_1" name: "conv1_7x7_s2_bn_sc_internal_1" op_type: "Mul" }
  node { input: "conv1_7x7_s2_bn_sc_b_0" output: "OC2_DUMMY_1" name: "OC2_DUMMY_1" op_type: "Unsqueeze" attribute { name: "axes" ints: 1 ints: 2 type: INTS } }
  node { input: "conv1_7x7_s2_bn_sc_internal_1" input: "OC2_DUMMY_1" output: "conv1_7x7_s2_bn_sc_1" name: "conv1_7x7_s2_bn_sc_1" op_type: "Add" }
  node { input: "conv1_7x7_s2_bn_sc_1" output: "conv1_7x7_s2_bn_sc_2" name: "conv1_7x7_s2_bn_sc_2" op_type: "Relu" }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 3 } dim { dim_value: 224 } dim { dim_value: 224 } } } } }
  input { name: "conv1_7x7_s2_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 3 } dim { dim_value: 7 } dim { dim_value: 7 } } } } }
  input { name: "conv1_7x7_s2_bn_scale_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_bias_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_mean_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_var_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_sc_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_sc_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  output { name: "conv1_7x7_s2_bn_sc_2"  type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 64 } dim { dim_value: 112 } dim { dim_value: 112 } } } } }
}
opset_import { domain: "" version: 8 }
metadata_props { key: "initializers" value: "conv1_7x7_s2_w_0,conv1_7x7_s2_bn_scale_0,conv1_7x7_s2_bn_bias_0,conv1_7x7_s2_bn_mean_0,conv1_7x7_s2_bn_var_0,conv1_7x7_s2_bn_sc_w_0,conv1_7x7_s2_bn_sc_b_0" }
