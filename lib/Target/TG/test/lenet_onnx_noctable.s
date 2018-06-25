#; RUN : onnx-as lenet.onnx.s | onnx2tg -march bm1880 -print-machineinstrs | FileCheck lenet.onnx.noctable.s

# CHECK: inst {
# CHECK-NEXT:   name: "conv1_1"
# CHECK-NEXT:   type: "bmnet_conv_fixed_forward_bmkernel"
# CHECK-NEXT:   conv {
# CHECK-NEXT:     ga_ifmap: 0
# CHECK-NEXT:     ga_ofmap: 0
# CHECK-NEXT:     ga_weight: 784
# CHECK-NEXT:     ga_bias: 500
# CHECK-NEXT:     input_n: 1
# CHECK-NEXT:     input_c: 1
# CHECK-NEXT:     input_h: 28
# CHECK-NEXT:     input_w: 28
# CHECK-NEXT:     groups: 1
# CHECK-NEXT:     output_c: 20
# CHECK-NEXT:     kh: 5
# CHECK-NEXT:     kw: 5
# CHECK-NEXT:     dilation_h: 1
# CHECK-NEXT:     dilation_w: 1
# CHECK-NEXT:     pad_h: 0
# CHECK-NEXT:     pad_w: 0
# CHECK-NEXT:     stride_h: 1
# CHECK-NEXT:     stride_w: 1
# CHECK-NEXT:     result_add: 0
# CHECK-NEXT:     do_bn: 1
# CHECK-NEXT:     do_scale: 0
# CHECK-NEXT:     do_scale_bias: 0
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
# CHECK-NEXT: inst {
# CHECK-NEXT:   name: "pool1_1"
# CHECK-NEXT:   type: "bmnet_pooling_fixed_forward_bmkernel"
# CHECK-NEXT:   pooling {
# CHECK-NEXT:     ifmap_gaddr: 784
# CHECK-NEXT:     ofmap_gaddr: 12304
# CHECK-NEXT:     n: 1
# CHECK-NEXT:     c: 20
# CHECK-NEXT:     h: 24
# CHECK-NEXT:     w: 24
# CHECK-NEXT:     kh: 2
# CHECK-NEXT:     kw: 2
# CHECK-NEXT:     pad_h: 0
# CHECK-NEXT:     pad_w: 0
# CHECK-NEXT:     stride_h: 2
# CHECK-NEXT:     stride_w: 2
# CHECK-NEXT:     is_avg_pooling: false
# CHECK-NEXT:     avg_const: 0
# CHECK-NEXT:     do_relu: false
# CHECK-NEXT:     right_shift_width: 0
# CHECK-NEXT:     threshold_x_quantized: 0
# CHECK-NEXT:   }
# CHECK-NEXT: }
# CHECK-NEXT: inst {
# CHECK-NEXT:   name: "conv2_1"
# CHECK-NEXT:   type: "bmnet_conv_fixed_forward_bmkernel"
# CHECK-NEXT:   conv {
# CHECK-NEXT:     ga_ifmap: 12304
# CHECK-NEXT:     ga_ofmap: 540
# CHECK-NEXT:     ga_weight: 15184
# CHECK-NEXT:     ga_bias: 25540
# CHECK-NEXT:     input_n: 1
# CHECK-NEXT:     input_c: 20
# CHECK-NEXT:     input_h: 12
# CHECK-NEXT:     input_w: 12
# CHECK-NEXT:     groups: 1
# CHECK-NEXT:     output_c: 50
# CHECK-NEXT:     kh: 5
# CHECK-NEXT:     kw: 5
# CHECK-NEXT:     dilation_h: 1
# CHECK-NEXT:     dilation_w: 1
# CHECK-NEXT:     pad_h: 0
# CHECK-NEXT:     pad_w: 0
# CHECK-NEXT:     stride_h: 1
# CHECK-NEXT:     stride_w: 1
# CHECK-NEXT:     result_add: 0
# CHECK-NEXT:     do_bn: 1
# CHECK-NEXT:     do_scale: 0
# CHECK-NEXT:     do_scale_bias: 0
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
# CHECK-NEXT: inst {
# CHECK-NEXT:   name: "pool2_1"
# CHECK-NEXT:   type: "bmnet_pooling_fixed_forward_bmkernel"
# CHECK-NEXT:   pooling {
# CHECK-NEXT:     ifmap_gaddr: 15184
# CHECK-NEXT:     ofmap_gaddr: 18384
# CHECK-NEXT:     n: 1
# CHECK-NEXT:     c: 50
# CHECK-NEXT:     h: 8
# CHECK-NEXT:     w: 8
# CHECK-NEXT:     kh: 2
# CHECK-NEXT:     kw: 2
# CHECK-NEXT:     pad_h: 0
# CHECK-NEXT:     pad_w: 0
# CHECK-NEXT:     stride_h: 2
# CHECK-NEXT:     stride_w: 2
# CHECK-NEXT:     is_avg_pooling: false
# CHECK-NEXT:     avg_const: 0
# CHECK-NEXT:     do_relu: false
# CHECK-NEXT:     right_shift_width: 0
# CHECK-NEXT:     threshold_x_quantized: 0
# CHECK-NEXT:   }
# CHECK-NEXT: }
# CHECK-NEXT: inst {
# CHECK-NEXT:   name: "ip1_1"
# CHECK-NEXT:   type: "bmnet_fc_fixed_forward_bmkernel"
# CHECK-NEXT:   fc {
# CHECK-NEXT:     bottom_data_gaddr: 18384
# CHECK-NEXT:     weight_data_gaddr: 25640
# CHECK-NEXT:     bias_data_gaddr: 425640
# CHECK-NEXT:     top_data_gaddr: 19184
# CHECK-NEXT:     input_row_num: 1
# CHECK-NEXT:     input_col_num: 800
# CHECK-NEXT:     weight_col_num: 500
# CHECK-NEXT:     have_bias: true
# CHECK-NEXT:     do_activation: 1
# CHECK-NEXT:     activation_method: RELU
# CHECK-NEXT:     weight_transpose: true
# CHECK-NEXT:     right_shift_width: 0
# CHECK-NEXT:   }
# CHECK-NEXT: }
# CHECK-NEXT: inst {
# CHECK-NEXT:   name: "ip2_1"
# CHECK-NEXT:   type: "bmnet_fc_fixed_forward_bmkernel"
# CHECK-NEXT:   fc {
# CHECK-NEXT:     bottom_data_gaddr: 19184
# CHECK-NEXT:     weight_data_gaddr: 426640
# CHECK-NEXT:     bias_data_gaddr: 431640
# CHECK-NEXT:     top_data_gaddr: 19684
# CHECK-NEXT:     input_row_num: 1
# CHECK-NEXT:     input_col_num: 500
# CHECK-NEXT:     weight_col_num: 10
# CHECK-NEXT:     have_bias: true
# CHECK-NEXT:     do_activation: 0
# CHECK-NEXT:     activation_method: RELU
# CHECK-NEXT:     weight_transpose: true
# CHECK-NEXT:     right_shift_width: 0
# CHECK-NEXT:   }
# CHECK-NEXT: }

# CHECK: INT8 tensor <1, 20, 24, 24> %conv1_1 = Conv <pads:INTS [0,0,0,0], strides:INTS [1,1], kernel_shape:INTS [5,5]> (INT8 tensor <1, 1, 28, 28> %data_0, INT8 tensor <20, 1, 5, 5> %conv1_w_0, INT16 tensor <20> %conv1_b_0)
# CHECK: INT8 tensor <1, 20, 12, 12> %pool1_1 = MaxPool <pads:INTS [0,0,1,1], kernel_shape:INTS [2,2], strides:INTS [2,2]> (INT8 tensor <1, 20, 24, 24> %conv1_1)
# CHECK: INT8 tensor <1, 50, 8, 8> %conv2_1 = Conv <pads:INTS [0,0,0,0], strides:INTS [1,1], kernel_shape:INTS [5,5]> (INT8 tensor <1, 20, 12, 12> %pool1_1, INT8 tensor <50, 20, 5, 5> %conv2_w_0, INT16 tensor <50> %conv2_b_0)
# CHECK: INT8 tensor <1, 50, 4, 4> %pool2_1 = MaxPool <pads:INTS [0,0,1,1], kernel_shape:INTS [2,2], strides:INTS [2,2]> (INT8 tensor <1, 50, 8, 8> %conv2_1)
# CHECK: INT8 tensor <1, 800> %OC2_DUMMY_0 = Reshape(INT8 tensor <1, 50, 4, 4> %pool2_1, INT64 tensor <2> %OC2_DUMMY_1)
# CHECK: INT8 tensor <1, 500> %ip1_1 = Gemm <transB:INT 1, broadcast:INT 1, enableReLu:INT 1> (INT8 tensor <1, 800> %OC2_DUMMY_0, INT8 tensor <500, 800> %ip1_w_0, INT16 tensor <500> %ip1_b_0)
# CHECK: INT8 tensor <1, 10> %ip2_1 = Gemm <transB:INT 1, broadcast:INT 1> (INT8 tensor <1, 500> %ip1_1, INT8 tensor <10, 500> %ip2_w_0, INT16 tensor <10> %ip2_b_0)
# CHECK: INT8 tensor <1, 10> %prob_1 = Softmax(INT8 tensor <1, 10> %ip2_1)

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "LeNet"
  node { input: "data_0" input: "conv1_w_0" input: "conv1_b_0" output: "conv1_1" name: "" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 5 ints: 5 type: INTS } }
  node { input: "conv1_1" output: "pool1_1" name: "" op_type: "MaxPool" attribute { name: "pads" ints: 0 ints: 0 ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 2 ints: 2 type: INTS } attribute { name: "strides" ints: 2 ints: 2 type: INTS } }
  node { input: "pool1_1" input: "conv2_w_0" input: "conv2_b_0" output: "conv2_1" name: "" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 5 ints: 5 type: INTS } }
  node { input: "conv2_1" output: "pool2_1" name: "" op_type: "MaxPool" attribute { name: "pads" ints: 0 ints: 0 ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 2 ints: 2 type: INTS } attribute { name: "strides" ints: 2 ints: 2 type: INTS } }
  node { input: "pool2_1" input: "OC2_DUMMY_1" output: "OC2_DUMMY_0" op_type: "Reshape" }
  node { input: "OC2_DUMMY_0" input: "ip1_w_0" input: "ip1_b_0" output: "ip1_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  node { input: "ip1_1" output: "relu1_1" name: "" op_type: "Relu" }
  node { input: "relu1_1" input: "ip2_w_0" input: "ip2_b_0" output: "ip2_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } attribute { name: "broadcast" i: 1 type: INT } }
  node { input: "ip2_1" output: "prob_1" name: "" op_type: "Softmax" }
  initializer { dims: 2 data_type: INT64 name: "OC2_DUMMY_1" raw_data: "\001\000\000\000\000\000\000\000 \003\000\000\000\000\000\000" }
  input { name: "data_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 1 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }
  input { name: "conv1_w_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 20 } dim { dim_value: 1 } dim { dim_value: 5 } dim { dim_value: 5 } } } } }
  input { name: "conv1_b_0" type { tensor_type { elem_type: INT16 shape { dim { dim_value: 20 } } } } }
  input { name: "conv2_w_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 50 } dim { dim_value: 20 } dim { dim_value: 5 } dim { dim_value: 5 } } } } }
  input { name: "conv2_b_0" type { tensor_type { elem_type: INT16 shape { dim { dim_value: 50 } } } } }
  input { name: "ip1_w_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 500 } dim { dim_value: 800 } } } } }
  input { name: "ip1_b_0" type { tensor_type { elem_type: INT16 shape { dim { dim_value: 500 } } } } }
  input { name: "ip2_w_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 10 } dim { dim_value: 500 } } } } }
  input { name: "ip2_b_0" type { tensor_type { elem_type: INT16 shape { dim { dim_value: 10 } } } } }
  input { name: "OC2_DUMMY_1" type { tensor_type { elem_type: INT64 shape { dim { dim_value: 2 } } } } }
  output { name: "prob_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 10 } } } } }
  value_info { name: "conv1_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 20 } dim { dim_value: 24 } dim { dim_value: 24 } } } } }
  value_info { name: "pool1_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 20 } dim { dim_value: 12 } dim { dim_value: 12 } } } } }
  value_info { name: "conv2_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 50 } dim { dim_value: 8 } dim { dim_value: 8 } } } } }
  value_info { name: "pool2_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 50 } dim { dim_value: 4 } dim { dim_value: 4 } } } } }
  value_info { name: "OC2_DUMMY_0" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 800 } } } } }
  value_info { name: "ip1_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 500 } } } } }
  value_info { name: "relu1_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 500 } } } } }
  value_info { name: "ip2_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 10 } } } } }
}
opset_import { domain: "" version: 6 }
