#include "TGConv.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"
#include <onnc/Support/Debug.h>

using namespace onnc;
using namespace std;

namespace {

auto KERNEL_SHAPE = ::onnx::Symbol("kernel_shape");
auto STRIDES = ::onnx::Symbol("strides");
auto PADS = ::onnx::Symbol("pads");

void dump_onnx_Conv(const ::onnx::Node &pNode)
{
  auto vec = pNode.attributeNames();
  string tab = "\t";
  DEBUG(dbgs() << "dump TGConv:" << endl;);

  DEBUG(dbgs() << tab << "Attr: ";);
  for (auto i : vec) {
    DEBUG(dbgs() << i.toString() << ",";);
  }
  DEBUG(dbgs() << endl;);

  if (pNode.hasAttribute(KERNEL_SHAPE)) {
    DEBUG(dbgs() << tab << "kernel_shape: ";);
    DEBUG(dbgs() << "<";);
    for (auto &i : pNode.is(KERNEL_SHAPE)) {
      DEBUG(dbgs() << i << ",";);
    }
    DEBUG(dbgs() << ">" << endl;);
  } else {
  }

  if (pNode.hasAttribute(STRIDES)) {
    DEBUG(dbgs() << tab << "strides: ";);
    DEBUG(dbgs() << "<";);
    for (auto &i : pNode.is(STRIDES)) {
      DEBUG(dbgs() << i << ",";);
    }
    DEBUG(dbgs() << ">" << endl;);
  }

  if (pNode.hasAttribute(PADS)) {
    DEBUG(dbgs() << tab << "pads: ";);
    DEBUG(dbgs() << "<";);
    for (auto &i : pNode.is(PADS)) {
      DEBUG(dbgs() << i << ",";);
    }
    DEBUG(dbgs() << ">" << endl;);
  }

  auto inputs = pNode.inputs();
  DEBUG(dbgs() << tab << "inputs: ";);
  for (auto i : inputs) {
    DEBUG(dbgs() << i->uniqueName() << "<";);
    for (auto &dim : i->sizes()) {
      DEBUG(dbgs() << dim.dim << ",";);
    }
    DEBUG(dbgs() << ">,";);
  }
  DEBUG(dbgs() << endl;);
  auto outputs = pNode.outputs();
  DEBUG(dbgs() << tab << "outputs: ";);
  for (auto i : outputs) {
    DEBUG(dbgs() << i->uniqueName() << "<";);
    for (auto &dim : i->sizes()) {
      DEBUG(dbgs() << dim.dim << ",";);
    }
    DEBUG(dbgs() << ">,";);
  }
  DEBUG(dbgs() << endl;);
}

} // anonymous namespace

// TGConv
TGConv::TGConv(const ::onnx::Node &pNode, MemTable &pMemTable)
    : Operator(pNode, "Conv"), m_groups(1), m_dilationH(1), m_dilationW(1),
      m_padH(0), m_padW(0), m_strideH(1), m_strideW(1), m_doBias(0)
{
  dump_onnx_Conv(pNode);
  auto inputs = pNode.inputs();
  auto outputs = pNode.outputs();
  m_ifmapAddr = pMemTable[inputs[0]->uniqueName()];
  m_weightAddr = pMemTable[inputs[1]->uniqueName()];
  m_ofmapAddr = pMemTable[outputs[0]->uniqueName()];

  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_inN = inDim[0].dim;
  m_inC = inDim[1].dim;
  m_inH = inDim[2].dim;
  m_inW = inDim[3].dim;
  const std::vector< ::onnx::Dimension> weightDim = pNode.inputs()[1]->sizes();
  m_outC = weightDim[0].dim;
  if (pNode.hasAttribute(::onnx::Symbol("group"))) {
    m_groups = pNode.i(::onnx::Symbol("group"));
  }
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    m_kH = i[0];
    m_kW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("dilations"))) {
    auto &i = pNode.is(::onnx::Symbol("dilations"));
    m_dilationH = i[0];
    m_dilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_padH = i[0];
    m_padW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
  if (3 == pNode.inputs().size()) {
    m_doBias = 1;
    m_biasAddr = pMemTable[inputs[2]->uniqueName()];
  } else {
    m_biasAddr = 0;
  }
}

void TGConv::emit() const
{
  DEBUG(dbgs() << "TGConv::emit\tm_ifmapAddr:" << m_ifmapAddr << " m_ofmapAddr:"
               << m_ofmapAddr << " m_weightAddr:" << m_weightAddr
               << " m_biasAddr:" << m_biasAddr << " m_inN:" << m_inN
               << " m_inC:" << m_inC << " m_inH:" << m_inH << " m_inW:" << m_inW
               << " m_outC:" << m_outC << " m_groups:" << m_groups << " m_kH:"
               << m_kH << " m_kW:" << m_kW << " m_dilationH:" << m_dilationH
               << " m_dilationW:" << m_dilationW << " m_padH:" << (int)m_padH
               << " m_padW:" << (int)m_padW << " m_strideH:" << (int)m_strideH
               << " m_strideW:" << (int)m_strideW << " m_doBias:" << m_doBias
               << std::endl;);
#if 0
  bmnet::bmnet_conv_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx, m_ifmapAddr, m_ofmapAddr,
      m_weightAddr, m_biasAddr, GADDR_INVALID, // ga_bn_mean,
      GADDR_INVALID,                           // ga_bn_variance,
      GADDR_INVALID,                           // ga_scale,
      GADDR_INVALID,                           // ga_scale_bias,
      m_inN, m_inC, m_inH, m_inW, m_groups, m_outC, m_kH, m_kW, m_dilationH,
      m_dilationW, m_padH, m_padW, m_strideH, m_strideW, false, // result_add
      m_doBias,                                                 // do_bias,
      0,                                                        // do_bn,
      0,                                                        // do_scale,
      0, // do_scale_bias,
      0, // do_activation,
      0, // bn_scale,
      0, // bn_eps,
      0, // activation_method,
      0, // activation_arg[],
      0, // activation_ga_slope,
      0  // activation_channel_shared
  );
#endif
}
