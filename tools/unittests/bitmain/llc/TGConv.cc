#include "TGOperator.h"
#include <bmkernel_api.h>

using namespace std;

static auto KERNEL_SHAPE = onnx::Symbol("kernel_shape");
static auto STRIDES = onnx::Symbol("strides");
static auto PADS = onnx::Symbol("pads");

static void dump_onnx_Conv(const onnx::Node &node)
{
  auto vec = node.attributeNames();
  string tab = "\t";
  cout << "Conv Support attributes:" << endl;

  for(auto i : vec) {
    cout << tab << "Attr " << i.toString() << endl;
  }

  if (node.hasAttribute(KERNEL_SHAPE)) {
    cout << tab << "kernel_shape: ";
    for (auto &i : node.is(KERNEL_SHAPE)) {
      cout << i << " ";
    }
    cout << endl;
  } else {
  }

  if (node.hasAttribute(STRIDES)) {
    cout << tab << "strides: ";
    for (auto &i : node.is(STRIDES)) {
      cout << i << " ";
    }
    cout << endl;
  }

  if (node.hasAttribute(PADS)) {
    cout << tab << "pads: ";
    for (auto &i : node.is(PADS)) {
      cout << i << " ";
    }
    cout << endl;
  }

  auto inputs = node.inputs();
  for(auto i : inputs) {
    cout << tab << "inputs: ";
    cout << i->uniqueName() << " ";
    for (auto &dim : i->sizes()) {
      cout << dim.dim << " ";
    }
    cout << endl;
  }
  auto outputs = node.outputs();
  for(auto i : outputs) {
    cout << tab << "outputs: ";
    cout << i->uniqueName() << " ";
    for (auto &dim : i->sizes()) {
      cout << dim.dim << " ";
    }
    cout << endl;
  }
}
// TGConv
TGConv::TGConv(const onnx::Node &node, uint64_t offset) : TGOperator(node, "Conv") {
  dump_onnx_Conv(node);
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);
}

void TGConv::emit(void) const {
  std::cout << "TGConv::emit" << std::endl;
  bmnet_conv_forward_bmkernel(
      0,     // gaddr_t             ga_ifmap,
      0,     // gaddr_t             ga_ofmap,
      0,     // gaddr_t             ga_weight,
      0,     // gaddr_t             ga_bias,
      0,     // gaddr_t             ga_bn_mean,
      0,     // gaddr_t             ga_bn_variance,
      0,     // gaddr_t             ga_scale,
      0,     // gaddr_t             ga_scale_bias,
      4,     // int                 input_n,
      3,     // int                 input_c,
      50,    // int                 input_h,
      50,    // int                 input_w,
      1,     // int                 groups,
      5,     // int                 output_c,
      5,     // u16                 kh,
      5,     // u16                 kw,
      1,     // u16                 dilation_h,
      1,     // u16                 dilation_w,
      0,     // u8                  pad_h,
      0,     // u8                  pad_w,
      1,     // u8                  stride_h,
      1,     // u8                  stride_w,
      false, // int                 result_add,
      0,     // int                 do_bias,
      0,     // int                 do_bn,
      0,     // int                 do_scale,
      0,     // int                 do_scale_bias,
      0,     // int                 do_activation,
      0,     // float               bn_scale,
      0,     // float               bn_eps,
      0,     // int                 activation_method,
      0,     // float               activation_arg[],
      0,     // gaddr_t             activation_ga_slope,
      0      //  bool activation_channel_shared
        );
}
