#include <bm_kernel.h>
#include <bmkernel_api.h>
#include <cstring>
#include <cassert>
#include <fstream>
#include <host_util.h>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include "TGBackend.h"
#include "reader_helper.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/optimizer/optimize.h"


namespace {

void updateOutputDimByInputDim(onnx::Node *const node) {
  const std::vector<onnx::Dimension> inputDim = node->inputs()[0]->sizes();
  // FIXME workaorund unimplemented type
  if (0 == inputDim.size())
    return;
  auto outputs = node->outputs();
  for (auto outVal : outputs) {
    if (0 == outVal->sizes().size())
      outVal->setSizes(inputDim);
  }
}

void updatePoolOutputDim(onnx::Node *const node) {
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
  int64_t oH = (iH - kH + xb + xe) / sH + 1;
  int64_t oW = (iW - kW + yb + ye) / sW + 1;

  std::vector<onnx::Dimension> outDims{ onnx::Dimension(oN),
                                        onnx::Dimension(oC),
                                        onnx::Dimension(oH),
                                        onnx::Dimension(oW) };

  auto outputs = node->outputs();
  for (auto outVal : outputs) {
    if (0 == outVal->sizes().size())
      outVal->setSizes(outDims);
  }
}

void updateGemmOutputDim(onnx::Node *const node) {

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

  auto outputs = node->outputs();
  for (auto outVal : outputs) {
    if (0 == outVal->sizes().size())
      outVal->setSizes(outDims);
  }
}

void updateOutputDim(onnx::Graph &graph) {
  for (onnx::graph_node_list_iterator it =graph.begin(), ie = graph.end(); it != ie;
       ++it) {
    onnx::Node* node = *it;
    auto symbol = node->kind();

    if (symbol == onnx::Symbol("Conv")) {
      const std::vector<onnx::Dimension> inputDim = node->inputs()[0]->sizes();
      // FIXME workaorund unimplemented type
      if (0 == inputDim.size())
        continue;
      const auto iN = inputDim[0].dim;
      const auto iC = inputDim[1].dim;
      const auto iH = inputDim[2].dim;
      const auto iW = inputDim[3].dim;

      const auto weightDim = node->inputs()[1]->sizes();
      const auto wN = weightDim[0].dim;
      const auto wC = weightDim[1].dim;
      auto kH = weightDim[2].dim;
      auto kW = weightDim[3].dim;

      int64_t sH(1) ,sW(1);
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
      int64_t oC = wC;
      int64_t oH = (iH - kH + xb + xe) / sH + 1;
      int64_t oW = (iW - kW + yb + ye) / sW + 1;

      std::vector<onnx::Dimension> outDims{ onnx::Dimension(oN),
                                            onnx::Dimension(oC),
                                            onnx::Dimension(oH),
                                            onnx::Dimension(oW) };

      auto outputs = node->outputs();
      for (auto outVal : outputs) {
        if (0 == outVal->sizes().size())
          outVal->setSizes(outDims);
      }
    } else if (symbol == onnx::Symbol("Relu")) {
      updateOutputDimByInputDim(node);
    } else if (symbol == onnx::Symbol("LRN")) {
      updateOutputDimByInputDim(node);
    } else if (symbol == onnx::Symbol("MaxPool")) {
      updatePoolOutputDim(node);
    } else if (symbol == onnx::Symbol("Gemm")) {
      updateGemmOutputDim(node);
    } else if (symbol == onnx::Symbol("Softmax")) {
      updateOutputDimByInputDim(node);
    }
  }
}

} // end anonymous namespace

void TGBackend::sendCmdBuf(void *userData, const void *cmdBuf, uint32_t len) {
  std::cout << __func__ << std::endl;
  std::cout << "save to " << CMD_BUF_NAME << std::endl;
  std::fstream output(CMD_BUF_NAME,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  output.write((char *)cmdBuf, len);
}

void TGBackend::emitCmdBuf(void *userData, void *cmdBuf, uint32_t len) {
  sendCmdBuf(userData, cmdBuf, len);
  /* free implicitly */
  free(cmdBuf);
}

void TGBackend::freeCmdBuf(void *userData, void *cmdBuf) {
  /* free without emit */
  emitCmdBuf(userData, cmdBuf, 0);
}

void *TGBackend::allocCmdBuf(void *userData, uint32_t size) {
  void *cmdBuf = malloc(size);
  return cmdBuf;
}

void TGBackend::hostSync(void) {}

void TGBackend::emitDebugInfo(void *userData, char const *info, int nodeId,
                              long long unsigned int fwAddr, bool isFloat) {}

void TGBackend::bmkernelContextPrepare(void) {
  bmkernel_info_t bmkernelInfo;
  hw_info_t hwInfo;

  memcpy(&hwInfo, get_hw_info(), sizeof(hw_info_t));
  std::cout << "default hw_info:" << std::endl;
  std::cout << "  chip_version " << hwInfo.chip_version << std::endl
            << "  nodechip_shift " << hwInfo.nodechip_shift << std::endl
            << "  npu_shift " << hwInfo.npu_shift << std::endl << "  eu_shift "
            << hwInfo.eu_shift << std::endl << "  local_mem_shift "
            << hwInfo.local_mem_shift << std::endl << "  local_mem_banks "
            << hwInfo.local_mem_banks << std::endl << "  global_mem_size 0x"
            << std::hex << hwInfo.global_mem_size << std::endl
            << "  nodechip_num " << hwInfo.nodechip_num << std::endl
            << "  npu_num " << hwInfo.npu_num << std::endl << "  eu_num "
            << hwInfo.eu_num << std::endl << "  local_mem_size 0x" << std::hex
            << hwInfo.local_mem_size << std::endl;

  bmkernelInfo.chip_version = hwInfo.chip_version;
  bmkernelInfo.eu_shift = hwInfo.eu_shift;
  bmkernelInfo.npu_shift = hwInfo.npu_shift;
  bmkernelInfo.node_shift = hwInfo.nodechip_shift;
  bmkernelInfo.local_mem_shift = hwInfo.local_mem_shift;
  bmkernelInfo.local_mem_banks = hwInfo.local_mem_banks;
  bmkernelInfo.cmd_buf_size = 0x10000000;
  bmkernelInfo.emit_on_update = false;
  bmkernelInfo.alloc_cmd_buf = TGBackend::allocCmdBuf;
  bmkernelInfo.emit_cmd_buf = TGBackend::emitCmdBuf;
  bmkernelInfo.free_cmd_buf = TGBackend::freeCmdBuf;
  bmkernelInfo.sync = TGBackend::hostSync;
  bmkernelInfo.debug = TGBackend::emitDebugInfo;

  bmkernel_register(&bmkernelInfo, BMKERNEL_NODE_ID_HOST, (void *)0,
                    &m_bmkernelHandle);
  std::cout << "bmkernel_register done" << std::endl;
}

TGBackend::TGBackend(const onnx::ModelProto &model) : m_bmkernelHandle(nullptr) {

  // test onnx opt passes
  std::cout << "before onnx IR optimization" << std::endl;
  onnx::optimization::Optimizer onnxOptimizer;
  std::vector<std::string> passNames{"eliminate_nop_transpose", "fuse_consecutive_transposes", "fuse_transpose_into_gemm"};
  auto optModel = onnxOptimizer.optimize(model, passNames);
  std::cout << "after onnx IR optimization" << std::endl;
  dumpONNXProto(optModel);

  // transfer pb to onnx ir
  m_onnxGraph = std::move(onnx::ImportModelProto(optModel));

  updateOutputDim(*const_cast<onnx::Graph *>(m_onnxGraph.get()));
}

TGBackend::~TGBackend() { kernel_exit(); }

void TGBackend::codeEmit(void) {
  // init bm1680 context
  bmkernelContextPrepare();
  kernel_enter(m_bmkernelHandle);
  for (auto const &i : m_instructions) {
    i->emit();
  }
  m_instructions.clear();
  kernel_submit();
}

TGBackend &TGBackend::lowering(void) {
  uint64_t offset = 0;
  for (auto it = m_onnxGraph->begin(), ie = m_onnxGraph->end(); it != ie;
       ++it) {
    const onnx::Node *const node = *it;
    std::unique_ptr<TGOperator> tgOp(TGOperator::makeTGOperator(*node, offset));
    // FIXME walkaound for Dropout node
    if (nullptr == tgOp)
      continue;
    assert(nullptr != tgOp);
    offset += tgOp->getTotalWeightSize();
    std::cout << "lowering: " << tgOp->getName() << ", offset: " << offset
              << std::endl;
    m_instructions.push_back(std::move(tgOp));
  }
  return *this;
}

