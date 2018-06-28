#; RUN : onnx-as transpose_onnx.s | onnx2tg -march bm1880 -ignore-calibration-step -print-module-before-isel 2>&1 | FileCheck transpose_onnx.s
#; CHECK: FLOAT tensor <1, 38, 38, 16> %transpose1 = Transpose <perm:INTS [0,2,3,1]> (FLOAT tensor <1, 16, 38, 38> %data_0)

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  name: "Test_Transpose"
  node {
    input: "data_0"
    output: "transpose1"
    name: "transpose1"
    op_type: "Transpose"
    attribute {
      name: "perm"
      ints: 0
      ints: 2
      ints: 3
      ints: 1
      type: INTS
    }
  }
  input {
    name: "data_0"
    type {
      tensor_type {
        elem_type: FLOAT
        shape {
          dim {
            dim_value: 1
          }
          dim {
            dim_value: 16
          }
          dim {
            dim_value: 38
          }
          dim {
            dim_value: 38
          }
        }
      }
    }
  }
  output {
    name: "transpose1"
    type {
      tensor_type {
        elem_type: FLOAT
        shape {
          dim {
            dim_value: 1
          }
          dim {
            dim_value: 38
          }
          dim {
            dim_value: 38
          }
          dim {
            dim_value: 16
          }
        }
      }
    }
  }
}
opset_import {
  domain: ""
  version: 6
}
