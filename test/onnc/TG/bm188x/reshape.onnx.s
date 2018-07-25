# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight -batch-size=10 -o=- | FileCheck %s

#CHECK:  FLOAT tensor <10, 512, 1, 1> %resnetv22_pool1_fwd = GlobalAveragePool(FLOAT tensor <10, 512, 1, 1> %data)
#CHECK-NEXT:  FLOAT tensor <10, 512> %resnetv22_flatten0_reshape0 = Reshape(FLOAT tensor <10, 512, 1, 1> %resnetv22_pool1_fwd, INT64 tensor <2> %resnetv22_pool1_fwdreshape)
#CHECK-NEXT:  FLOAT tensor <10, 1000> %resnetv22_dense0_fwd = Gemm <alpha:FLOAT 1, beta:FLOAT 1, transA:INT 0, transB:INT 1> (FLOAT tensor <10, 512> %resnetv22_flatten0_reshape0, FLOAT tensor <1000, 512> %resnetv22_dense0_weight, FLOAT tensor <1000> %resnetv22_dense0_bias)


ir_version: 3
producer_name: ""
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "main"
  doc_string: ""
  node { input: "data" output: "resnetv22_pool1_fwd" name: "resnetv22_pool1_fwd" op_type: "GlobalAveragePool" }
  node { input: "resnetv22_pool1_fwd" input: "reshape_attr_tensor164" output: "resnetv22_flatten0_reshape0" name: "resnetv22_flatten0_reshape0" op_type: "Reshape" }
  node { input: "resnetv22_flatten0_reshape0" input: "resnetv22_dense0_weight" input: "resnetv22_dense0_bias" output: "resnetv22_dense0_fwd" name: "resnetv22_dense0_fwd" op_type: "Gemm" attribute { name: "alpha" f: 1 type: FLOAT } attribute { name: "beta" f: 1 type: FLOAT } attribute { name: "transA" i: 0 type: INT } attribute { name: "transB" i: 1 type: INT } }
  input { name: "data" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 512 } dim { dim_value: 1 } dim { dim_value: 1 } } } } }
  input { name: "reshape_attr_tensor164" type { tensor_type { elem_type: INT64 shape { dim { dim_value: 2 } } } } }
  input { name: "resnetv22_dense0_weight" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1000 } dim { dim_value: 512 } } } } }
  input { name: "resnetv22_dense0_bias" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1000 } } } } }
  output { name: "resnetv22_dense0_fwd" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 1000 } } } } }
  initializer { 
dims: 2
data_type: INT64
int64_data: 0
int64_data: -1
name: "reshape_attr_tensor164"
 }
}
opset_import { version: 7 }
metadata_props { key: "initializers" value: "resnetv22_dense0_weight,resnetv22_dense0_bias" }
