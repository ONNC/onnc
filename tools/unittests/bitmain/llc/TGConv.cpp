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
TGConv::TGConv(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Conv"), m_dilationH(1), m_dilationW(1), m_groups(1),
      m_padH(0), m_padW(0), m_strideH(1), m_strideW(1), m_doBias(0) {
  dump_onnx_Conv(node);
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);
  // TODO
  m_ifmapAddr = m_ofmapAddr = m_weightAddr = m_biasAddr = 0;
  const std::vector<onnx::Dimension> inDim = node.inputs()[0]->sizes();
  m_inN = inDim[0].dim;
  m_inC = inDim[1].dim;
  m_inH = inDim[2].dim;
  m_inW = inDim[3].dim;
  const std::vector<onnx::Dimension> weightDim = node.inputs()[1]->sizes();
  m_outC = weightDim[0].dim;
  if (node.hasAttribute(onnx::Symbol("group"))) {
     m_groups = node.i(onnx::Symbol("group"));
  }
  if (node.hasAttribute(onnx::Symbol("kernel_shape"))) {
    auto &i = node.is(onnx::Symbol("kernel_shape"));
    m_kH = i[0];
    m_kW = i[1];
  }
  if (node.hasAttribute(onnx::Symbol("dilations"))) {
    auto &i = node.is(onnx::Symbol("dilations"));
    m_dilationH = i[0];
    m_dilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (node.hasAttribute(onnx::Symbol("pads"))) {
    auto &i = node.is(onnx::Symbol("pads"));
    m_padH = i[0] + i[2];
    m_padW = i[1] + i[3];
  }
  if (node.hasAttribute(onnx::Symbol("strides"))) {
    auto &i = node.is(onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
  if(3 == node.inputs().size())
    m_doBias = 1;
}

void TGConv::emit(void) const {
  std::cout << "TGConv::emit\tm_ifmapAddr:" << m_ifmapAddr
            << " m_ofmapAddr:" << m_ofmapAddr
            << " m_weightAddr:" << m_weightAddr << " m_biasAddr:" << m_biasAddr
            << " m_inN:" << m_inN << " m_inC:" << m_inC << " m_inH:" << m_inH
            << " m_inW:" << m_inW << " m_outC:" << m_outC
            << " m_groups:" << m_groups << " m_kH:" << m_kH << " m_kW:" << m_kW
            << " m_dilationH:" << m_dilationH << " m_dilationW:" << m_dilationW
            << " m_padH:" << (int)m_padH << " m_padW:" << (int)m_padW
            << " m_strideH:" << (int)m_strideH
            << " m_strideW:" << (int)m_strideW << " m_doBias:" << m_doBias
            << std::endl;
  bmnet_conv_forward_bmkernel(m_ifmapAddr, m_ofmapAddr, m_weightAddr,
                              m_biasAddr, GADDR_INVALID, // ga_bn_mean,
                              GADDR_INVALID,             // ga_bn_variance,
                              GADDR_INVALID,             // ga_scale,
                              GADDR_INVALID,             // ga_scale_bias,
                              m_inN, m_inC, m_inH, m_inW, m_groups, m_outC,
                              m_kH, m_kW, m_dilationH, m_dilationW, m_padH,
                              m_padW, m_strideH, m_strideW, false, // result_add
                              m_doBias,                            // do_bias,
                              0,                                   // do_bn,
                              0,                                   // do_scale,
                              0, // do_scale_bias,
                              0, // do_activation,
                              0, // bn_scale,
                              0, // bn_eps,
                              0, // activation_method,
                              0, // activation_arg[],
                              0, // activation_ga_slope,
                              0  // activation_channel_shared
                              );
}
