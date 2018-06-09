#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>
#include <vector>
#include "TG.h"
#include "ONNXIRPrinter.h"

using namespace onnc;

namespace {

class UpdateOutputInfo : public ModulePass
{
public:
  static char ID;

public:
  UpdateOutputInfo() : ModulePass(ID) {}

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  void updateInfo(::onnx::ArrayRef<::onnx::Value *> &&outputs,
                        const std::vector<::onnx::Dimension> &dims,
                        ::onnx::TensorProto_DataType type);
  void updateInfoByInput(::onnx::Node *const node);
  void updateConvInfo(::onnx::Node *const node);
  void updatePoolInfo(::onnx::Node *const node);
  void updateGemmInfo(::onnx::Node *const node);
};

void
UpdateOutputInfo::updateInfo(::onnx::ArrayRef<::onnx::Value *> &&outputs,
                                   const std::vector<::onnx::Dimension> &dims,
                                   ::onnx::TensorProto_DataType type) {
  for (auto outVal : outputs) {
    outVal->setElemType(type);
    if (0 == outVal->sizes().size())
      outVal->setSizes(dims);
  }
}

void UpdateOutputInfo::updateInfoByInput(::onnx::Node *const node) {
  auto input = node->inputs()[0];
  const std::vector<::onnx::Dimension> inputDim = input->sizes();
  const ::onnx::TensorProto_DataType inputType = input->elemType();
  assert(inputType != ::onnx::TensorProto_DataType_UNDEFINED);
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  updateInfo(node->outputs(), inputDim, inputType);
}

void UpdateOutputInfo::updateConvInfo(::onnx::Node *const node) {
  const std::vector<::onnx::Dimension> inputDim = node->inputs()[0]->sizes();
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

  if (node->hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = node->is(::onnx::Symbol("kernel_shape"));
    kH = i[0];
    kW = i[1];
  }

  if (node->hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = node->is(::onnx::Symbol("strides"));
    sH = i[0];
    sW = i[1];
  }

  if (node->hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = node->is(::onnx::Symbol("pads"));
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

  std::vector<::onnx::Dimension> outDims{ ::onnx::Dimension(oN),
                                        ::onnx::Dimension(oC),
                                        ::onnx::Dimension(oH),
                                        ::onnx::Dimension(oW) };

  const ::onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  updateInfo(node->outputs(), outDims, inputType);
}

void UpdateOutputInfo::updatePoolInfo(::onnx::Node *const node) {
  const std::vector<::onnx::Dimension> inputDim = node->inputs()[0]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  const auto iN = inputDim[0].dim;
  const auto iC = inputDim[1].dim;
  const auto iH = inputDim[2].dim;
  const auto iW = inputDim[3].dim;
  const auto kH = node->is(::onnx::Symbol("kernel_shape"))[0];
  const auto kW = node->is(::onnx::Symbol("kernel_shape"))[1];

  int64_t sH(1), sW(1);
  // pads for x_begin, y_begin, x_end, y_end
  int64_t xb(0), yb(0), xe(0), ye(0);

  if (node->hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = node->is(::onnx::Symbol("strides"));
    sH = i[0];
    sW = i[1];
  }

  if (node->hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = node->is(::onnx::Symbol("pads"));
    xb = i[0];
    yb = i[1];
    xe = i[2];
    ye = i[3];
  }

  int64_t oN = iN;
  int64_t oC = iC;
#if 1
  // NOTE: It is for bmkernel only padding on both ends
  int64_t oH =
      static_cast<int64_t>(ceil(static_cast<float>(iH - kH + 2 * xb) / sH)) + 1;
  int64_t oW =
      static_cast<int64_t>(ceil(static_cast<float>(iW - kW + 2 * yb) / sW)) + 1;
#else
  int64_t oH =
      static_cast<int64_t>(ceil(static_cast<float>(iH - kH + xb + xe) / sH)) +
      1;
  int64_t oW =
      static_cast<int64_t>(ceil(static_cast<float>(iW - kW + yb + ye) / sW)) +
      1;
#endif

  std::vector<::onnx::Dimension> outDims{ ::onnx::Dimension(oN),
                                        ::onnx::Dimension(oC),
                                        ::onnx::Dimension(oH),
                                        ::onnx::Dimension(oW) };

  const ::onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  updateInfo(node->outputs(), outDims, inputType);
}

void UpdateOutputInfo::updateGemmInfo(::onnx::Node *const node) {

  const std::vector<::onnx::Dimension> aDim = node->inputs()[0]->sizes();
  const std::vector<::onnx::Dimension> bDim = node->inputs()[1]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == aDim.size() || 0 == bDim.size())
    return;
  // A: M x K
  int64_t oM = aDim[0].dim;
  if (node->hasAttribute(::onnx::Symbol("transA"))) {
    auto transA = node->i(::onnx::Symbol("transA"));
    oM = transA ? aDim[1].dim : aDim[0].dim;
  }
  // B: K x N
  int64_t oN = bDim[1].dim;
  if (node->hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = node->i(::onnx::Symbol("transB"));
    oN = transB ? bDim[0].dim : bDim[1].dim;
  }

  std::vector<::onnx::Dimension> outDims{ ::onnx::Dimension(oM),
                                        ::onnx::Dimension(oN) };
  const ::onnx::TensorProto_DataType inputType = node->inputs()[0]->elemType();
  updateInfo(node->outputs(), outDims, inputType);
}

Pass::ReturnType UpdateOutputInfo::runOnModule(Module &pModule) {

  ::onnx::Graph *graph = pModule.getGraphIR().get();

  // workaround code for update alexnet input dimension
  if (graph->has_name() && std::string::npos != graph->name().find("alexnet")) {
    ::onnx::ArrayRef<::onnx::Value*> inputValues = graph->inputs();
    for (auto &v : inputValues) {
      if (v->has_unique_name() && v->uniqueName() == std::string("data_0")) {
          std::vector<::onnx::Dimension> dims = { 10, 3, 227, 227 };
          v->setSizes(dims);
          break;
      }
    }
  }

  for (::onnx::graph_node_list_iterator it = graph->begin(), ie = graph->end();
       it != ie; ++it) {
    ::onnx::Node *node = *it;
    auto symbol = node->kind();

    if (symbol == ::onnx::Symbol("Conv")) {
      updateConvInfo(node);
    } else if (symbol == ::onnx::Symbol("Relu")) {
      updateInfoByInput(node);
    } else if (symbol == ::onnx::Symbol("LRN")) {
      updateInfoByInput(node);
    } else if (symbol == ::onnx::Symbol("MaxPool")) {
      updatePoolInfo(node);
    } else if (symbol == ::onnx::Symbol("Dropout")) {
      updateInfoByInput(node);
    } else if (symbol == ::onnx::Symbol("Gemm")) {
      updateGemmInfo(node);
    } else if (symbol == ::onnx::Symbol("Softmax")) {
      updateInfoByInput(node);
    } else {
      std::cerr << "unimplemented type: " << symbol.toString() << std::endl;
    }
  }

  // TODO use IR printer pass, or add passes by option (ex. printf-after-all)
  std::cout << "==================after UpdateOutputInfoPass: =====================" << std::endl;
  ONNXIRPrinter::dumpGraph(pModule.getGraphIR());

  return Pass::kModuleChanged;
}

} // anonymous namespace

char UpdateOutputInfo::ID = 0;

ModulePass *onnc::createUpdateOutputInfoPass() { return new UpdateOutputInfo(); }
