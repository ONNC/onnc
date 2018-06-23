#; RUN : onnx-as fuse_conv_bn_mul_add_relu.s | onnx2tg -march bm1880 -ignore-calibration-step -print-module-before-isel | FileCheck fuse_conv_bn_mul_add_relu.s
#; CHECK:  FLOAT tensor <4, 64, 112, 112> %scale_conv1_1 = Scale <axis:INT 1, broadcast:INT 1> (FLOAT tensor <4, 64, 112, 112> %data_0, FLOAT tensor <64, 112, 112, 64> %scale_conv1_w_0, FLOAT tensor <64> %scale_conv1_b_0)

ir_version: 3
producer_name: "onnx-caffe2"
graph {
  name: "fuse-mul-add"
  node { input: "data_0" input: "scale_conv1_w_0" output: "scale_conv1_internal_1" name: "" op_type: "Mul" attribute { name: "axis" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  node { input: "scale_conv1_internal_1" input: "scale_conv1_b_0" output: "scale_conv1_1" name: "" op_type: "Add" attribute { name: "axis" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } dim { dim_value: 64 } dim { dim_value: 112 } dim { dim_value: 112 } } } } }
  input { name: "scale_conv1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 112 }  dim { dim_value: 112 } dim { dim_value: 64 } } } } }
  input { name: "scale_conv1_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  output { name: "scale_conv1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4} dim { dim_value: 64 } dim { dim_value: 112} dim {dim_value: 112 } } } } }
}
opset_import { domain: "" version: 6 }
