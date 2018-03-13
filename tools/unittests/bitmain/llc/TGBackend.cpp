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
#include "TGISelLowering.h"
#include "reader_helper.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/optimizer/optimize.h"

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

TGBackend::TGBackend(const onnx::ModelProto &model) : m_bmkernelHandle(nullptr), m_TLI(new TGTargetLowering(this)) {

  // test onnx opt passes
  std::cout << "before onnx IR optimization" << std::endl;
  onnx::optimization::Optimizer onnxOptimizer;
  std::vector<std::string> passNames{"eliminate_nop_transpose", "fuse_consecutive_transposes", "fuse_transpose_into_gemm"};
  auto optModel = onnxOptimizer.optimize(model, passNames);
  std::cout << "after onnx IR optimization" << std::endl;
  dumpONNXProto(optModel);

  // transfer pb to onnx ir
  m_onnxGraph = std::move(onnx::ImportModelProto(optModel));

  ::removeUnusedNodePass::removeUnusedNode(*const_cast<onnx::Graph *>(m_onnxGraph.get()));
  ::updateOutputInfoPass::updateOutputInfo(*const_cast<onnx::Graph *>(m_onnxGraph.get()));

  std::cout << "after ours IR passes" << std::endl;
  // TODO, add IR dumpper
  onnx::ModelProto lastModel;
  onnx::ExportModelProto(&lastModel, m_onnxGraph);
  dumpONNXProto(lastModel);

}

TGBackend::~TGBackend() {
  delete m_TLI;
  kernel_exit();
}



TGBackend &TGBackend::ddrAllocInfo(void) {
  ddrAllocInfo(*m_onnxGraph.get(), m_globalMemLayout);
  return *this;
}

void TGBackend::ddrAllocInfo(onnx::Graph &graph, MemTable &memTable) {
  // Definition fom BM168xBackendContext.hpp
  // TAG will be masked by runtime while processing cmdbuf.
  const int GLOBAL_NEURON_TAG = 0x1;
  const int GLOBAL_WEIGHT_TAG = 0x2;
  const int GLOBAL_ARM_TAG = 0x3;

  // allocate spaces for weight
  unsigned int weight_offset = 0;
  // BMKernel only supports DATA_FMT_F32 & DATA_FMT_I1
  int F32_SIZE = 4;
  std::string tab = "\t";

  std::cout << __func__ << " dump global memory layout:" << std::endl;

  for (auto i : graph.initializers()) {

    memTable[i.name()] = weight_offset + GLOBAL_WEIGHT_TAG;
    std::cout << tab << i.name() << " = " << weight_offset;

    assert(i.elem_type() == onnx::TensorProto_DataType_FLOAT);
    if (i.sizes().size() > 0) {
      int tensor_size = F32_SIZE;
      std::cout << " <";
      for (auto dim : i.sizes()) {
        std::cout << dim << ",";
        tensor_size *= dim;
      }
      std::cout << ">" << std::endl;
      weight_offset += tensor_size;
    } else {
      std::cout << std::endl;
    }
  }

  unsigned int neuron_offset = 0;
  std::unordered_set<std::string> initNames(graph.initializer_names().begin(),
                                            graph.initializer_names().end());
  // allocate space for inputs
  for (auto i : graph.inputs()) {
    if (0 == initNames.count(i->uniqueName())) {

      memTable[i->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << i->uniqueName() << " = " << neuron_offset;

      assert(i->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (i->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto &dim : i->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  // allocate space for outputs
  for (auto i : graph.nodes()) {
    if (i->kind() == onnx::Symbol("Undefined"))
      continue;

    for (auto o : i->outputs()) {

      memTable[o->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << o->uniqueName() << " = " << neuron_offset;

      // FIXME: remove this after output dimension is fixed
      assert(o->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (o->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto dim : o->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  std::cout << tab << "weight size: " << weight_offset << std::endl;
  std::cout << tab << "neuron size: " << neuron_offset << std::endl;
}

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
  m_TLI->CodeGenAndEmitInst(m_onnxGraph.get(), m_instructions);
  return *this;
}

