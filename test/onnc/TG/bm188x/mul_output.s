# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -add-dummy-weight -o=- | FileCheck %s

#CHECK:    "output layer": {
#CHECK-NEXT:        "0": {
#CHECK-NEXT:            "onnc output": "conv6-3_1",
#CHECK-NEXT:            "onnx output": "prob1_1",
#CHECK-NEXT:            "threshold": {{.*}}
#CHECK-NEXT:        },
#CHECK-NEXT:        "1": {
#CHECK-NEXT:            "onnc output": "conv6-2_1",
#CHECK-NEXT:            "onnx output": "conv6-2_1",
#CHECK-NEXT:            "threshold": {{.*}}
#CHECK-NEXT:        },
#CHECK-NEXT:        "2": {
#CHECK-NEXT:            "onnc output": "conv6-3_1",
#CHECK-NEXT:            "onnx output": "conv6-3_1",
#CHECK-NEXT:            "threshold": {{.*}}
#CHECK-NEXT:        }

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "ONet"
  doc_string: ""
  node { input: "data_0" input: "conv5_w_0" input: "conv5_b_0" output: "conv5_3" name: "conv5_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } }
  node { input: "conv5_3" input: "conv6-1_w_0" input: "conv6-1_b_0" output: "conv6-1_1" name: "conv6-1_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } }
  node { input: "conv5_3" input: "conv6-2_w_0" input: "conv6-2_b_0" output: "conv6-2_1" name: "conv6-2_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } }
  node { input: "conv5_3" input: "conv6-3_w_0" input: "conv6-3_b_0" output: "conv6-3_1" name: "conv6-3_1" op_type: "Gemm" attribute { name: "transB" i: 1 type: INT } }
  node { input: "conv6-1_1" output: "prob1_1" name: "prob1_1" op_type: "Softmax" }
  input { name: "data_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 1152 } } } } }
  input { name: "conv5_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 256 } dim { dim_value: 1152 } } } } }
  input { name: "conv5_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 256 } } } } }
  input { name: "conv6-1_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 2 } dim { dim_value: 256 } } } } }
  input { name: "conv6-1_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 2 } } } } }
  input { name: "conv6-2_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } dim { dim_value: 256 } } } } }
  input { name: "conv6-2_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 4 } } } } }
  input { name: "conv6-3_w_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } dim { dim_value: 256 } } } } }
  input { name: "conv6-3_b_0" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 10 } } } } }
  output { name: "prob1_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 2 } } } } }
  output { name: "conv6-2_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 4 } } } } }
  output { name: "conv6-3_1" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 10 } } } } }
}
opset_import { domain: "" version: 8 }
metadata_props { key: "initializers" value: "conv5_w_0,conv5_b_0,conv6-1_w_0,conv6-1_b_0,conv6-2_w_0,conv6-2_b_0,conv6-3_w_0,conv6-3_b_0" }
