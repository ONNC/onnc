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
