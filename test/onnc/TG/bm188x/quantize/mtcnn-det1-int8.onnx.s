# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-weight -o=- | FileCheck %s

# CHECK:      {{inst {[[:space:]]+ name: "conv1_2"}}
# CHECK-NEXT:     type: "bmnet_prelu_fixed_forward_bmkernel"
# CHECK-NEXT:     prelu {
# CHECK-NEXT: 	input_gaddr: 432
# CHECK-NEXT: 	slope_gaddr: 290
# CHECK-NEXT: 	output_gaddr: 1432
# CHECK-NEXT: 	channel_shared: 0
# CHECK-NEXT: 	slope: -39
# CHECK-NEXT: 	input_n: 1
# CHECK-NEXT: 	input_c: 10
# CHECK-NEXT: 	input_h: 10
# CHECK-NEXT: 	input_w: 10
# CHECK-NEXT: 	GT_scale: 124
# CHECK-NEXT: 	GT_right_shift_width: 7
# CHECK-NEXT: 	LE_right_shift_width: 6
# CHECK-NEXT:     }
# CHECK-NEXT: }

# CHECK:      {{inst {[[:space:]]+ name: "conv2_2"}}
# CHECK-NEXT:     type: "bmnet_prelu_fixed_forward_bmkernel"
# CHECK-NEXT:     prelu {
# CHECK-NEXT: 	input_gaddr: 2682
# CHECK-NEXT: 	slope_gaddr: 1772
# CHECK-NEXT: 	output_gaddr: 2826
# CHECK-NEXT: 	channel_shared: 0
# CHECK-NEXT: 	slope: 15
# CHECK-NEXT: 	input_n: 1
# CHECK-NEXT: 	input_c: 16
# CHECK-NEXT: 	input_h: 3
# CHECK-NEXT: 	input_w: 3
# CHECK-NEXT: 	GT_scale: 112
# CHECK-NEXT: 	GT_right_shift_width: 5
# CHECK-NEXT: 	LE_right_shift_width: 6
# CHECK-NEXT:     }
# CHECK-NEXT: }

# CHECK:      {{inst {[[:space:]]+ name: "conv3_2"}}
# CHECK-NEXT:     type: "bmnet_prelu_fixed_forward_bmkernel"
# CHECK-NEXT:     prelu {
# CHECK-NEXT: 	input_gaddr: 2970
# CHECK-NEXT: 	slope_gaddr: 6460
# CHECK-NEXT: 	output_gaddr: 3002
# CHECK-NEXT: 	channel_shared: 0
# CHECK-NEXT: 	slope: 5
# CHECK-NEXT: 	input_n: 1
# CHECK-NEXT: 	input_c: 32
# CHECK-NEXT: 	input_h: 1
# CHECK-NEXT: 	input_w: 1
# CHECK-NEXT: 	GT_scale: 93
# CHECK-NEXT: 	GT_right_shift_width: 6
# CHECK-NEXT: 	LE_right_shift_width: 6
# CHECK-NEXT:     }
# CHECK-NEXT: }

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "PNet"
  doc_string: ""
  node { input: "data_0" input: "conv1_w_0" input: "conv1_b_0" output: "conv1_1" name: "conv1_1" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 3 ints: 3 type: INTS } }
  node { input: "conv1_1" input: "conv1_Slope_0" output: "conv1_2" name: "conv1_2" op_type: "PRelu" }
  node { input: "conv1_2" output: "pool1_1" name: "pool1_1" op_type: "MaxPool" attribute { name: "pads" ints: 0 ints: 0 ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 2 ints: 2 type: INTS } attribute { name: "strides" ints: 2 ints: 2 type: INTS } }
  node { input: "pool1_1" input: "conv2_w_0" input: "conv2_b_0" output: "conv2_1" name: "conv2_1" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 3 ints: 3 type: INTS } }
  node { input: "conv2_1" input: "conv2_Slope_0" output: "conv2_2" name: "conv2_2" op_type: "PRelu" }
  node { input: "conv2_2" input: "conv3_w_0" input: "conv3_b_0" output: "conv3_1" name: "conv3_1" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 3 ints: 3 type: INTS } }
  node { input: "conv3_1" input: "conv3_Slope_0" output: "conv3_2" name: "conv3_2" op_type: "PRelu" }
  node { input: "conv3_2" input: "conv4-1_w_0" input: "conv4-1_b_0" output: "conv4-1_1" name: "conv4-1_1" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 1 ints: 1 type: INTS } }
  node { input: "conv3_2" input: "conv4-2_w_0" input: "conv4-2_b_0" output: "conv4-2_1" name: "conv4-2_1" op_type: "Conv" attribute { name: "pads" ints: 0 ints: 0 ints: 0 ints: 0 type: INTS } attribute { name: "strides" ints: 1 ints: 1 type: INTS } attribute { name: "kernel_shape" ints: 1 ints: 1 type: INTS } }
  node { input: "conv4-1_1" output: "prob1_1" name: "prob1_1" op_type: "Softmax" }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 3 } dim { dim_value: 12 } dim { dim_value: 12 } } } } }
  input { name: "conv1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } dim { dim_value: 3 } dim { dim_value: 3 } dim { dim_value: 3 } } } } }
  input { name: "conv1_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } } } } }
  input { name: "conv1_Slope_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } } } } }
  input { name: "conv2_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 16 } dim { dim_value: 10 } dim { dim_value: 3 } dim { dim_value: 3 } } } } }
  input { name: "conv2_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 16 } } } } }
  input { name: "conv2_Slope_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 16 } } } } }
  input { name: "conv3_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 32 } dim { dim_value: 16 } dim { dim_value: 3 } dim { dim_value: 3 } } } } }
  input { name: "conv3_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 32 } } } } }
  input { name: "conv3_Slope_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 32 } } } } }
  input { name: "conv4-1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 2 } dim { dim_value: 32 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  input { name: "conv4-1_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 2 } } } } }
  input { name: "conv4-2_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } dim { dim_value: 32 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  input { name: "conv4-2_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } } } } }
  output { name: "prob1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 2 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  value_info { name: "conv1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 10 } dim { dim_value: 10 } dim { dim_value: 10 } } } } }
  value_info { name: "conv1_2" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 10 } dim { dim_value: 10 } dim { dim_value: 10 } } } } }
  value_info { name: "pool1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 10 } dim { dim_value: 5 } dim { dim_value: 5 } } } } }
  value_info { name: "conv2_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 16 } dim { dim_value: 3 } dim { dim_value: 3 } } } } }
  value_info { name: "conv2_2" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 16 } dim { dim_value: 3 } dim { dim_value: 3 } } } } }
  value_info { name: "conv3_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 32 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  value_info { name: "conv3_2" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 32 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  value_info { name: "conv4-1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 2 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  value_info { name: "conv4-2_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 4 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  initializer { 
dims: 10
data_type: FLOAT
name: "conv1_Slope_0"
raw_data: "\241\032 \277\341\302X\276\330~\201?\217\332\341>\255r\204\277}\235\244>%\205\230\276O\367\200>\000\373a\277\340\237\243\277"
 }
  initializer { 
dims: 16
data_type: FLOAT
name: "conv2_Slope_0"
raw_data: "\2158\211=\200\234Q>\342\030\201>l\221)=\'.G>\235\001\232>\373\225\331>j\026\336>\267\2333> \246\006\277\356\361\004>{wB>\275\371\207\272\035\274!\276 \010Y\276\260\002o\275"
 }
  initializer { 
dims: 32
data_type: FLOAT
name: "conv3_Slope_0"
raw_data: "\0362X= FQ\275\t\275\274=\222\347\266\276\201\020\251\276n\322\010\27647\246\273\213\256_=^\213\246\274\320\033\307\274>y\264=\036\354\230\276\205\355u\276\264z\036>\374\016@>\256L\223\275\361\320n\276\226(x>]\214\221>\212\245\356\274bF\323\276\332\262\250\274\302g\265\275\211\224N\276\004\220~;\302\0250?\224\320\240>\357c}>w\260\200=\275\231<\276r\023\007\276\034\344e>"
 }
 
}
opset_import { domain: "" version: 6 }
metadata_props { key: "bm1880_ctable" value: "layer {\n  name: \"data_0\"\n  blob_param {\n    name: \"data_0\"\n    threshold_y: 195.09526\n  }\n}\nlayer {\n  name: \"conv1_1\"\n  blob_param {\n    name: \"conv1_1\"\n    threshold_y: 619.92389\n  }\n}\nlayer {\n  name: \"conv1_2\"\n  blob_param {\n    name: \"conv1_2\"\n    threshold_y: 641.46606\n  }\n}\nlayer {\n  name: \"pool1_1\"\n  blob_param {\n    name: \"pool1_1\"\n    threshold_y: 641.46606\n  }\n}\nlayer {\n  name: \"conv2_1\"\n  blob_param {\n    name: \"conv2_1\"\n    threshold_y: 1810.4604\n  }\n}\nlayer {\n  name: \"conv2_2\"\n  blob_param {\n    name: \"conv2_2\"\n    threshold_y: 516.01672\n  }\n}\nlayer {\n  name: \"conv3_1\"\n  blob_param {\n    name: \"conv3_1\"\n    threshold_y: 748.11731\n  }\n}\nlayer {\n  name: \"conv3_2\"\n  blob_param {\n    name: \"conv3_2\"\n    threshold_y: 513.95654\n  }\n}\nlayer {\n  name: \"conv4-1_1\"\n  blob_param {\n    name: \"conv4-1_1\"\n    threshold_y: 85.805367\n  }\n}\nlayer {\n  name: \"conv4-2_1\"\n  blob_param {\n    name: \"conv4-2_1\"\n    threshold_y: 6.6959958\n  }\n}\nlayer {\n  name: \"prob1_1\"\n  blob_param {\n    name: \"prob1_1\"\n    threshold_y: 1.0004885\n  }\n}\n" }
metadata_props { key: "initializers" value: "conv1_w_0,conv1_b_0,conv2_w_0,conv2_b_0,conv3_w_0,conv3_b_0,conv4-1_w_0,conv4-1_b_0,conv4-2_w_0,conv4-2_b_0" }
