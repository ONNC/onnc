#; RUN : onnx-as fuse_bn_mul_add_opset8.s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight | FileCheck fuse_bn_mul_add_opset8.s
#; CHECK: FLOAT tensor <1, 64, 1, 1> %conv1_7x7_s2_bn_sc_1 = Scale <axis:INT 1, broadcast:INT 1> (FLOAT tensor <1, 64, 1, 1> %data_0, FLOAT tensor <64> %19, FLOAT tensor <64> %21)


ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "inception_v2"
  doc_string: ""
  node { input: "data_0" input: "conv1_7x7_s2_bn_scale_0" input: "conv1_7x7_s2_bn_bias_0" input: "conv1_7x7_s2_bn_mean_0" input: "conv1_7x7_s2_bn_var_0" output: "conv1_7x7_s2_bn_1" name: "conv1_7x7_s2_bn_1" op_type: "BatchNormalization" attribute { name: "epsilon" f: 1e-05 type: FLOAT } }
  node { input: "conv1_7x7_s2_bn_sc_w_0" output: "OC2_DUMMY_0" name: "OC2_DUMMY_0" op_type: "Unsqueeze" attribute { name: "axes" ints: 1 ints: 2 type: INTS } }
  node { input: "conv1_7x7_s2_bn_1" input: "OC2_DUMMY_0" output: "conv1_7x7_s2_bn_sc_internal_1" name: "conv1_7x7_s2_bn_sc_internal_1" op_type: "Mul" }
  node { input: "conv1_7x7_s2_bn_sc_b_0" output: "OC2_DUMMY_1" name: "OC2_DUMMY_1" op_type: "Unsqueeze" attribute { name: "axes" ints: 1 ints: 2 type: INTS } }
  node { input: "conv1_7x7_s2_bn_sc_internal_1" input: "OC2_DUMMY_1" output: "conv1_7x7_s2_bn_sc_1" name: "conv1_7x7_s2_bn_sc_1" op_type: "Add" }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 64 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  input { name: "conv1_7x7_s2_bn_scale_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_bias_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_mean_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_var_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_sc_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "conv1_7x7_s2_bn_sc_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  output { name: "conv1_7x7_s2_bn_sc_1"  type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 64 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
}
opset_import { domain: "" version: 8 }
