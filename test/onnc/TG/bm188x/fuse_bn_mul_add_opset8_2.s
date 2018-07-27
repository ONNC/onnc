# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight -o=- |  FileCheck %s
#; CHECK: FLOAT tensor <1, 64, 112, 112> %scale_conv1_1 = Conv <pads:INTS [3,3,3,3], strides:INTS [2,2], kernel_shape:INTS [7,7], do_scale:INT 1, do_scale_bias:INT 1, conv_output_threshold:FLOAT 1> (FLOAT tensor <1, 3, 224, 224> %data_0, FLOAT tensor <64, 3, 7, 7> %conv1_w_0, FLOAT tensor <64> %21, FLOAT tensor <64> %23)

ir_version: 3
producer_name: "caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "onnxifi_conv1_1"
  doc_string: ""
  node { input: "data_0" input: "conv1_w_0" output: "conv1_1" name: "conv1_1" op_type: "Conv" attribute { name: "pads" ints: 3 ints: 3 ints: 3 ints: 3 type: INTS } attribute { name: "strides" ints: 2 ints: 2 type: INTS } attribute { name: "kernel_shape" ints: 7 ints: 7 type: INTS } }
  node { input: "conv1_1" input: "bn_conv1_scale_0" input: "bn_conv1_bias_0" input: "bn_conv1_mean_0" input: "bn_conv1_var_0" output: "bn_conv1_1" name: "bn_conv1_1" op_type: "BatchNormalization" attribute { name: "epsilon" f: 1e-05 type: FLOAT } }
  node { input: "bn_conv1_1" input: "OC2_DUMMY_0" output: "scale_conv1_internal_1" name: "scale_conv1_internal_1" op_type: "Mul" }
  node { input: "scale_conv1_internal_1" input: "OC2_DUMMY_1" output: "scale_conv1_1" name: "scale_conv1_1" op_type: "Add" }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 3 } dim { dim_value: 224 } dim { dim_value: 224 } } } } }
  input { name: "conv1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 3 } dim { dim_value: 7 } dim { dim_value: 7 } } } } }
  input { name: "bn_conv1_scale_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_bias_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_mean_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_var_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "OC2_DUMMY_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  input { name: "OC2_DUMMY_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  output { name: "scale_conv1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 64 } dim { dim_value: 112 } dim { dim_value: 112 } } } } }
}
opset_import { domain: "" version: 8 }
metadata_props { key: "initializers" value: "conv1_w_0,bn_conv1_scale_0,bn_conv1_bias_0,bn_conv1_mean_0,bn_conv1_var_0,OC2_DUMMY_0,OC2_DUMMY_1" }
