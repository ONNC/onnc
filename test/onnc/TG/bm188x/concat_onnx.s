# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -o=- | FileCheck %s
#; CHECK: INT8 tensor <1, 256, 28, 28> %inception_3a/output_1 = Concat <axis:INT 1> (INT8 tensor <1, 64, 28, 28> %inception_3a/relu_1x1_1, INT8 tensor <1, 128, 28, 28> %inception_3a/relu_3x3_1, INT8 tensor <1, 32, 28, 28> %inception_3a/relu_5x5_1, INT8 tensor <1, 32, 28, 28> %inception_3a/relu_pool_proj_1)

# CHECK: inst {
# CHECK-NEXT:   name: "inception_3a/output_1"
# CHECK-NEXT:   type: "bmnet_concat_fixed_forward_bmkernel"
# CHECK-NEXT:   concat {
# CHECK-NEXT:     input_gaddrs: 0
# CHECK-NEXT:     input_gaddrs: 50176
# CHECK-NEXT:     input_gaddrs: 150528
# CHECK-NEXT:     input_gaddrs: 175616
# CHECK-NEXT:     output_gaddr: 200704
# CHECK-NEXT:     input_dims: 64
# CHECK-NEXT:     input_dims: 128
# CHECK-NEXT:     input_dims: 32
# CHECK-NEXT:     input_dims: 32
# CHECK-NEXT:     input_num: 4
# CHECK-NEXT:     concat_axis: 1
# CHECK-NEXT:     output_dim_len: 4
# CHECK-NEXT:     output_dim: 1
# CHECK-NEXT:     output_dim: 256
# CHECK-NEXT:     output_dim: 28
# CHECK-NEXT:     output_dim: 28
# CHECK-NEXT:     }
# CHECK-NEXT: }

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "Concat"
  doc_string: ""

  node { input: "inception_3a/relu_1x1_1" input: "inception_3a/relu_3x3_1" input: "inception_3a/relu_5x5_1" input: "inception_3a/relu_pool_proj_1" output: "inception_3a/output_1" name: "" op_type: "Concat" attribute { name: "axis" i: 1 type: INT } }

  input { name: "inception_3a/relu_1x1_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 64 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }
  input { name: "inception_3a/relu_3x3_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 128 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }
  input { name: "inception_3a/relu_5x5_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 32 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }
  input { name: "inception_3a/relu_pool_proj_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 32 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }

  output { name: "inception_3a/output_1" type { tensor_type { elem_type: INT8 shape { dim { dim_value: 1 } dim { dim_value: 256 } dim { dim_value: 28 } dim { dim_value: 28 } } } } }
}
opset_import { domain: "" version: 6 }
