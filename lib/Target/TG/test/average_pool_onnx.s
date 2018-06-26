#; RUN : onnx-as average_pool_onnx.s | onnx2tg -march bm1880 -print-machineinstrs | FileCheck average_pool_onnx.s
#; CHECK: FLOAT tensor <1, 20, 12, 12> %pool1_1 = AveragePool <pads:INTS [0,0,1,1], kernel_shape:INTS [2,2], strides:INTS [2,2]> (FLOAT tensor <1, 20, 24, 24> %data_0)

ir_version: 3
producer_name: "onnx-caffe2"
producer_version: ""
domain: ""
model_version: 0
doc_string: ""
graph {
  node {
    input: "data_0"
    output: "pool1_1"
    name: "pool1_1"
    op_type: "AveragePool"
    attribute {
      name: "pads"
      ints: 0
      ints: 0
      ints: 1
      ints: 1
      type: INTS
    }
    attribute {
      name: "kernel_shape"
      ints: 2
      ints: 2
      type: INTS
    }
    attribute {
      name: "strides"
      ints: 2
      ints: 2
      type: INTS
    }
  }
  name: "Test_AveragePool"
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
            dim_value: 20
          }
          dim {
            dim_value: 24
          }
          dim {
            dim_value: 24
          }
        }
      }
    }
  }
  output {
    name: "pool1_1"
    type {
      tensor_type {
        elem_type: FLOAT
        shape {
          dim {
            dim_value: 1
          }
          dim {
            dim_value: 20
          }
          dim {
            dim_value: 12
          }
          dim {
            dim_value: 12
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
metadata_props {
  key: "bm1880_ctable"
  value: "layer {\n  name: \"data_0\"\n  blob_param {\n    name: \"data_0\"\n    threshold_y: 0.99658036\n  }\n}\nlayer {\n  name: \"conv1_1\"\n  blob_param {\n    name: \"conv1_1\"\n    threshold_y: 1.835129\n  }\n}\nlayer {\n  name: \"pool1_1\"\n  blob_param {\n    name: \"pool1_1\"\n    threshold_y: 1.835129\n  }\n}\nlayer {\n  name: \"conv2_1\"\n  blob_param {\n    name: \"conv2_1\"\n    threshold_y: 5.0528378\n  }\n}\nlayer {\n  name: \"pool2_1\"\n  blob_param {\n    name: \"pool2_1\"\n    threshold_y: 4.4557886\n  }\n}\nlayer {\n  name: \"OC2_DUMMY_0\"\n  blob_param {\n    name: \"OC2_DUMMY_0\"\n    threshold_y: 4.4557886\n  }\n  blob_param {\n    name: \"OC2_DUMMY_2\"\n    threshold_y: 0\n  }\n}\nlayer {\n  name: \"ip1_1\"\n  blob_param {\n    name: \"ip1_1\"\n    threshold_y: 4.9610548\n  }\n}\nlayer {\n  name: \"relu1_1\"\n  blob_param {\n    name: \"relu1_1\"\n    threshold_y: 4.9610548\n  }\n}\nlayer {\n  name: \"ip2_1\"\n  blob_param {\n    name: \"ip2_1\"\n    threshold_y: 13.257649\n  }\n}\nlayer {\n  name: \"prob_1\"\n  blob_param {\n    name: \"prob_1\"\n    threshold_y: 1.0004244\n  }\n}\n"
}
