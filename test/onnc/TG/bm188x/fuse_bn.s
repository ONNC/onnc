# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight -o=- | FileCheck %s
#; CHECK: FLOAT tensor <4, 64, 112, 112> %bn_conv1_1 = Scale(FLOAT tensor <4, 64, 112, 112> %data_0, FLOAT tensor <64> %7, FLOAT tensor <64> %9)

ir_version: 3
producer_name: "onnx-caffe2"
graph {
  name: "fuse-bn-mul-add"
  node { input: "data_0" input: "bn_conv1_scale_0" input: "bn_conv1_bias_0" input: "bn_conv1_mean_0" input: "bn_conv1_var_0" output: "bn_conv1_1" name: "" op_type: "BatchNormalization" attribute { name: "is_test" i: 1 type: INT } attribute { name: "epsilon" f: 1e-05 type: FLOAT } }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } dim { dim_value: 64 } dim { dim_value: 112 } dim { dim_value: 112 } } } } }
  input { name: "bn_conv1_scale_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_bias_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_mean_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_var_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  output { name: "bn_conv1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4} dim { dim_value: 64 } dim { dim_value: 112} dim {dim_value: 112 } } } } }
}
opset_import { domain: "" version: 6 }
