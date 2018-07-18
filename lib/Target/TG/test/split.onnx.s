#; RUN : onnx-as ocr.onnx.s | onnx2tg -march bm1880 -print-module-before-isel | FileCheck ocr.onnx.s

# CHECK: FLOAT tensor <1, 512, 1024, 2> %split:1, FLOAT tensor <1, 512, 1024, 2> %split:2 = Split <axis:INT 1> (FLOAT tensor <1, 1024, 1024, 2> %input_images)
# CHECK: FLOAT tensor <1, 1024, 1024, 2> %concat = Concat <axis:INT 1> (FLOAT tensor <1, 512, 1024, 2> %split:1, FLOAT tensor <1, 512, 1024, 2> %split:2)

ir_version: 3
producer_name: "onnx-tensorflow"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "graph"
  doc_string: ""
  node { input: "input_images" output: "split:1" output: "split:2" name: "split" op_type: "Split" attribute { name: "axis" i: 1 type: INT } }
  node { input: "split:1" input: "split:2" output: "concat" name: "concat" op_type: "Concat" attribute { name: "axis" i: 1 type: INT } }
  input { name: "input_images" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 1024 } dim { dim_value: 1024 } dim { dim_value: 2 } } } } }
  output { name: "concat" type { tensor_type { elem_type: FLOAT shape { dim { dim_value: 1 } dim { dim_value: 1024 } dim { dim_value: 1024 } dim { dim_value: 2 } } } } }
}
opset_import { domain: "" version: 7 }
