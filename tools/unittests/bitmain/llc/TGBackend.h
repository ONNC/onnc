#pragma once

#include <memory>
#include <onnx/common/ir.h>
#include <onnx/onnx_pb.h>
#include <vector>
#include "Operator.h"
#include "TGISelLowering.h"
#include "TargetLowering.h"
#include "TGCodeEmitter.h"

class TGCodeEmitter;
namespace {
// FIXME remove this when pass finish
namespace updateOutputInfoPass {

void _updateOutputInfo(onnx::ArrayRef<onnx::Value *> &&outputs,
                      const std::vector<onnx::Dimension> &dims,
                      onnx::TensorProto_DataType type) {
  for (auto outVal : outputs) {
    outVal->setElemType(type);
    if (0 == outVal->sizes().size())
      outVal->setSizes(dims);
  }
}

void updateOutputInfoByInput(onnx::Node *const node) {
  auto input = node->inputs()[0];
  const std::vector<onnx::Dimension> inputDim = input->sizes();
  const onnx::TensorProto_DataType inputType = input->elemType();
  assert(inputType != onnx::TensorProto_DataType_UNDEFINED);
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  _updateOutputInfo(node->outputs(), inputDim, inputType);
}

void updateConvOutputInfo(onnx::Node *const node) {
  const std::vector<onnx::Dimension> inputDim = node->inputs()[0]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  const auto iN = inputDim[0].dim;
  const auto iC = inputDim[1].dim;
  const auto iH = inputDim[2].dim;
  const auto iW = inputDim[3].dim;

  const auto weightDim = node->inputs()[1]->sizes();
  const auto wN = weightDim[0].dim;
  const auto wC = weightDim[1].dim;
  auto kH = weightDim[2].dim;
  auto kW = weightDim[3].dim;

  int64_t sH(1), sW(1);
  // pads for x_begin, y_begin, x_end, y_end
  int64_t xb(0), yb(0), xe(0), ye(0);

  if (node->hasAttribute(onnx::Symbol("kernel_shape"))) {
    auto &i = node->is(onnx::Symbol("kernel_shape"));
    kH = i[0];
    kW = i[1];
  }

  if (node->hasAttribute(onnx::Symbol("strides"))) {
    auto &i = node->is(onnx::Symbol("strides"));
    sH = i[0];
    sW = i[1];
  }

  if (node->hasAttribute(onnx::Symbol("pads"))) {
    auto &i = node->is(onnx::Symbol("pads"));
    xb = i[0];
    yb = i[1];
    xe = i[2];
    ye = i[3];
  }

  int64_t oN = iN;
  int64_t oC = wN;
#if 1
  // NOTE: It is for bmkernel only padding on both ends
  int64_t oH = (iH - kH + 2 * xb) / sH + 1;
  int64_t oW = (iW - kW + 2 * yb) / sW + 1;
#else
  int64_t oH = (iH - kH + xb + xe) / sH + 1;
  int64_t oW = (iW - kW + yb + ye) / sW + 1;
#endif

  std::vector<onnx::Dimension> outDims{ onnx::Dimension(oN),
                                        onnx::Dimension(oC),
                                        onnx::Dimension(oH),
                                        onnx::Dimension(oW) };

  const onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  _updateOutputInfo(node->outputs(), outDims, inputType);
}

void updatePoolOutputInfo(onnx::Node *const node) {
  const std::vector<onnx::Dimension> inputDim = node->inputs()[0]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  const auto iN = inputDim[0].dim;
  const auto iC = inputDim[1].dim;
  const auto iH = inputDim[2].dim;
  const auto iW = inputDim[3].dim;
  const auto kH = node->is(onnx::Symbol("kernel_shape"))[0];
  const auto kW = node->is(onnx::Symbol("kernel_shape"))[1];

  int64_t sH(1), sW(1);
  // pads for x_begin, y_begin, x_end, y_end
  int64_t xb(0), yb(0), xe(0), ye(0);

  if (node->hasAttribute(onnx::Symbol("strides"))) {
    auto &i = node->is(onnx::Symbol("strides"));
    sH = i[0];
    sW = i[1];
  }

  if (node->hasAttribute(onnx::Symbol("pads"))) {
    auto &i = node->is(onnx::Symbol("pads"));
    xb = i[0];
    yb = i[1];
    xe = i[2];
    ye = i[3];
  }

  int64_t oN = iN;
  int64_t oC = iC;
#if 1
  // NOTE: It is for bmkernel only padding on both ends
  int64_t oH = static_cast<int64_t>(ceil(static_cast<float>(
      iH - kH + 2 * xb) / sH)) + 1;
  int64_t oW = static_cast<int64_t>(ceil(static_cast<float>(
      iW - kW + 2 * yb) / sW)) + 1;
#else
  int64_t oH = static_cast<int64_t>(ceil(static_cast<float>(
      iH - kH + xb + xe) / sH)) + 1;
  int64_t oW = static_cast<int64_t>(ceil(static_cast<float>(
      iW - kW + yb + ye) / sW)) + 1;
#endif

  std::vector<onnx::Dimension> outDims{ onnx::Dimension(oN),
                                        onnx::Dimension(oC),
                                        onnx::Dimension(oH),
                                        onnx::Dimension(oW) };

  const onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  _updateOutputInfo(node->outputs(), outDims, inputType);
}

void updateGemmOutputInfo(onnx::Node *const node) {

  const std::vector<onnx::Dimension> aDim = node->inputs()[0]->sizes();
  const std::vector<onnx::Dimension> bDim = node->inputs()[1]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == aDim.size() || 0 == bDim.size())
    return;
  // A: M x K
  int64_t oM = aDim[0].dim;
  if (node->hasAttribute(onnx::Symbol("transA"))) {
    auto transA = node->i(onnx::Symbol("transA"));
    oM = transA ? aDim[1].dim : aDim[0].dim;
  }
  // B: K x N
  int64_t oN = bDim[1].dim;
  if (node->hasAttribute(onnx::Symbol("transB"))) {
    auto transB = node->i(onnx::Symbol("transB"));
    oN = transB ? bDim[0].dim : bDim[1].dim;
  }

  std::vector<onnx::Dimension> outDims{ onnx::Dimension(oM),
                                        onnx::Dimension(oN) };
  const onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  _updateOutputInfo(node->outputs(), outDims, inputType);
}

void updateOutputInfo(onnx::Graph &graph) {
  for (onnx::graph_node_list_iterator it = graph.begin(), ie = graph.end();
       it != ie; ++it) {
    onnx::Node *node = *it;
    auto symbol = node->kind();

    if (symbol == onnx::Symbol("Conv")) {
      updateConvOutputInfo(node);
    } else if (symbol == onnx::Symbol("Relu")) {
      updateOutputInfoByInput(node);
    } else if (symbol == onnx::Symbol("LRN")) {
      updateOutputInfoByInput(node);
    } else if (symbol == onnx::Symbol("MaxPool")) {
      updatePoolOutputInfo(node);
    } else if (symbol == onnx::Symbol("Dropout")) {
      updateOutputInfoByInput(node);
    } else if (symbol == onnx::Symbol("Gemm")) {
      updateGemmOutputInfo(node);
    } else if (symbol == onnx::Symbol("Softmax")) {
      updateOutputInfoByInput(node);
    } else {
      std::cerr << "unimplemented type: " << symbol.toString() << std::endl;
    }
  }
}
} // end updateOutputInfoPass namespace

// FIXME remove this when pass finish
// remove unsed node in reference
namespace removeUnusedNodePass {

void removeUnusedNode(onnx::Graph &graph) {
  for (auto it = graph.begin(), ie = graph.end(); it != ie; ++it) {
    auto *node = *it;
    auto symbol = node->kind();
    if (symbol == onnx::Symbol("Dropout")) {
      // Dropout has multiple outputs
      node->outputs()[0]->replaceAllUsesWith(node->input());
      it.destroyCurrent();
    }
  }
}

} // end removeUnusedNodePass namespace


} // end anonymous namespace

using MemTable = std::map<std::string, uint64_t>;
class TGBackend {

private:
  std::vector<std::unique_ptr<Operator> > m_instructions;
  MemTable m_globalMemLayout;
  std::shared_ptr<onnx::Graph> m_onnxGraph;
  TargetLowering *m_TLI;
  TGCodeEmitter *m_CE;

public:
  TGBackend(const onnx::ModelProto &model);
  ~TGBackend();
  TGBackend &ddrAllocInfo(void);
  TGBackend &lowering(void);
  void codeEmit(void);
  MemTable &getMemLayout() { return m_globalMemLayout; }
  std::vector<std::unique_ptr<Operator> > &getInsts() { return m_instructions; }
  TargetLowering *getTargetLowering() {return m_TLI;}

  // FIXME for unit test
  static void ddrAllocInfo(onnx::Graph &graph, MemTable &memTable);

private:
  void bmkernelContextPrepare(void);
};
