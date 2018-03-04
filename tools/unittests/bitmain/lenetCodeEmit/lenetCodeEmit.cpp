#include "TGBackend.h"
#include <fcntl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <onnx/onnx_pb.h>

int main() {

  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  onnx::ModelProto model;
  {
    // TOOD, find the lenet onnx file
    int fd = open("bvlc_alexnet/model.onnx", O_RDONLY);
    ::google::protobuf::io::CodedInputStream coded_stream(
        new ::google::protobuf::io::FileInputStream(fd));
    coded_stream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    if (!model.ParseFromCodedStream(&coded_stream)) {
      std::cerr << "Failed to parse onnx file." << std::endl;
      return -1;
    }
  }

  TGBackend tgBackend;
  tgBackend.lowering(model).codeEmit();

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;

  /* lenet TG info
  tg_convolution param:
    input_shape 4 1 1c 1c , output_shape 4 14 18 18
    input 0, output 3100, weight 0
    bias_term 1, kernel 5*5, stride 1*1, pad 0*0, dilation 1*1, do_bn 0,
  do_scale 0, do_scale_bias 0, do_pooling 0, do_activation 0
  __total_coeff_size, weight_blob_size:100, oc_blob_size:100,
  total_coeff_size:200

  tg_pooling param:
    pooling method MAX
    input_shape 4 14 18 18 , output_shape 4 14 c c
    input 3100, output 30100
    bm1680Backend.codeEmit();
    bm1680Backend.codeEmit();
    kernel 2*2, stride 2*2, pad 0*0, do_activation0
  tg_convolution param:
    input_shape 4 14 c c , output_shape 4 32 8 8
    input 30100, output 3b500, weight 820
    bias_term 1, kernel 5*5, stride 1*1, pad 0*0, dilation 1*1, do_bn 0,
  do_scale 0, do_scale_bias 0, do_pooling 0, do_activation 0
  __total_coeff_size, weight_blob_size:800, oc_blob_size:100,
  total_coeff_size:900
  tg_pooling param:
    pooling method MAX
    input_shape 4 32 8 8 , output_shape 4 32 4 4
    input 3b500, output 47d00
    kernel 2*2, stride 2*2, pad 0*0, do_activation0
  tg_innerproduct param:
    input_shape 4 32 , output_shape 4 1f4
    input 47d00, output 4af00, weight 18f88, bias 19f988
    bias_term 1, do_activation 0
  tg_activation param:
    input_shape 4 1f4
    input 4af00, output 4ce40
    activation RELU
    RELU, negative_slope 0
  tg_innerproduct param:
    input_shape 4 1f4 , output_shape 4 a
    input 4ce40, output 4ed80, weight 1a0158, bias 1a4f78
    bias_term 1, do_activation 0
  */
}
