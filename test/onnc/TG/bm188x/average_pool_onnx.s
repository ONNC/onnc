# RUN: onnx-as %s | onnx2tg -march bm1880 -print-module-before-isel -add-dummy-ctable -o=- | FileCheck %s
#; CHECK: FLOAT tensor <1, 20, 12, 12> %pool1_1 = AveragePool <pads:INTS [0,0,1,1], kernel_shape:INTS [2,2], strides:INTS [2,2]> (FLOAT tensor <1, 20, 24, 24> %data_0)

# CHECK: inst {
# CHECK-NEXT:   name: "pool1_1"
# CHECK-NEXT:   type: "bmnet_pooling_fixed_forward_bmkernel"
# CHECK-NEXT:   pooling {
# CHECK-NEXT:     ifmap_gaddr: 0
# CHECK-NEXT:     ofmap_gaddr: 11520
# CHECK-NEXT:     index_gaddr: 1099511627775
# CHECK-NEXT:     o_findex_gaddr: 1099511627775
# CHECK-NEXT:     n: 1
# CHECK-NEXT:     c: 20
# CHECK-NEXT:     h: 24
# CHECK-NEXT:     w: 24
# CHECK-NEXT:     kh: 2
# CHECK-NEXT:     kw: 2
# CHECK-NEXT:     pad_top: 0
# CHECK-NEXT:     pad_bot: 1
# CHECK-NEXT:     pad_left: 0
# CHECK-NEXT:     pad_right: 1
# CHECK-NEXT:     stride_h: 2
# CHECK-NEXT:     stride_w: 2
# CHECK-NEXT:     is_avg_pooling: 1
# CHECK-NEXT:     avg_const: 0
# CHECK-NEXT:     do_relu: 0
# CHECK-NEXT:     right_shift_width: {{.*}}
# CHECK-NEXT:     threshold_x_quantized: {{.*}}
# CHECK-NEXT:   }
# CHECK-NEXT: }

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
