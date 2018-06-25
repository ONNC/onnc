#; RUN : onnx-as fuse_conv_bn_mul_add_relu.s | onnx2tg -march bm1880 -ignore-calibration-step -print-module-before-isel | FileCheck fuse_conv_bn_mul_add_relu.s

# CHECK: inst {
# CHECK-NEXT:   name: "scale_conv1_1"
# CHECK-NEXT:   type: "bmnet_conv_fixed_forward_bmkernel"
# CHECK-NEXT:   conv {
# CHECK-NEXT:     ga_ifmap: 0
# CHECK-NEXT:     ga_ofmap: 0
# CHECK-NEXT:     ga_weight: 0
# CHECK-NEXT:     ga_bias: 1099511627775
# CHECK-NEXT:     input_n: 4
# CHECK-NEXT:     input_c: 3
# CHECK-NEXT:     input_h: 224
# CHECK-NEXT:     input_w: 224
# CHECK-NEXT:     groups: 1
# CHECK-NEXT:     output_c: 64
# CHECK-NEXT:     kh: 7
# CHECK-NEXT:     kw: 7
# CHECK-NEXT:     dilation_h: 1
# CHECK-NEXT:     dilation_w: 1
# CHECK-NEXT:     pad_h: 3
# CHECK-NEXT:     pad_w: 3
# CHECK-NEXT:     stride_h: 2
# CHECK-NEXT:     stride_w: 2
# CHECK-NEXT:     result_add: 0
# CHECK-NEXT:     do_bn: 0
# CHECK-NEXT:     do_scale: 1
# CHECK-NEXT:     do_scale_bias: 1
# CHECK-NEXT:     bn_scale: 0
# CHECK-NEXT:     bn_eps: 0
# CHECK-NEXT:     activation_ga_slope: 0
# CHECK-NEXT:     activation_channel_shared: false
# CHECK-NEXT:     activation_gt_scale: 0
# CHECK-NEXT:     activation_gt_rshift: 0
# CHECK-NEXT:     activation_le_scale: 0
# CHECK-NEXT:     activation_le_rshift: 0
# CHECK-NEXT:     right_shift_width: 0
# CHECK-NEXT:   }
# CHECK-NEXT: }

#; CHECK:  FLOAT tensor <4, 64, 112, 112> %scale_conv1_1 = Conv <pads:INTS [3,3,3,3], strides:INTS [2,2], kernel_shape:INTS [7,7], do_scale:INT 1, do_scale_bias:INT 1> (FLOAT tensor <4, 3, 224, 224> %data_0, FLOAT tensor <64, 3, 7, 7> %conv1_w_0, FLOAT tensor <64> %bn_conv1_scale_0, FLOAT tensor <64> %bn_conv1_bias_0)

ir_version: 3
producer_name: "onnx-caffe2"
graph {
  name: "fuse-conv-bn-scale-relu"
  node { input: "data_0" input: "conv1_w_0" output: "conv1_1" name: "" op_type: "Conv" attribute { name: "pads" ints: 3 ints: 3 ints: 3 ints: 3 type: INTS } attribute { name: "strides" ints: 2 ints: 2 type: INTS } attribute { name: "kernel_shape" ints: 7 ints: 7 type: INTS } }
  node { input: "conv1_1" input: "bn_conv1_scale_0" input: "bn_conv1_bias_0" input: "bn_conv1_mean_0" input: "bn_conv1_var_0" output: "bn_conv1_1" name: "" op_type: "BatchNormalization" attribute { name: "is_test" i: 1 type: INT } attribute { name: "epsilon" f: 1e-05 type: FLOAT } }
  node { input: "bn_conv1_1" input: "scale_conv1_w_0" output: "scale_conv1_internal_1" name: "" op_type: "Mul" attribute { name: "axis" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  node { input: "scale_conv1_internal_1" input: "scale_conv1_b_0" output: "scale_conv1_1" name: "" op_type: "Add" attribute { name: "axis" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } dim { dim_value: 3 } dim { dim_value: 224 } dim { dim_value: 224 } } } } }
  input { name: "conv1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } dim { dim_value: 3 } dim { dim_value: 7 } dim { dim_value: 7 } } } } }
  input { name: "bn_conv1_scale_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_bias_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_mean_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "bn_conv1_var_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "scale_conv1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  input { name: "scale_conv1_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 64 } } } } }
  output { name: "scale_conv1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4} dim { dim_value: 64 } dim { dim_value: 112} dim {dim_value: 112 } } } } }
}
opset_import { domain: "" version: 6 }
