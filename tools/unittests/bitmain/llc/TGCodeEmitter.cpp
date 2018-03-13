#include "TGCodeEmitter.h"
#include <fstream>
#include <host_util.h>
#include <cstdint>
#include <bm_kernel.h>
#include <bmkernel_api.h>

#define CMD_BUF_NAME "cmdbuf.bin"

void TGCodeEmitter::sendCmdBuf(void *userData, const void *cmdBuf,
                               uint32_t len) {
  std::cout << __func__ << std::endl;
  std::cout << "save to " << CMD_BUF_NAME << std::endl;
  std::fstream output(CMD_BUF_NAME,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  output.write((char *)cmdBuf, len);
}

void TGCodeEmitter::emitCmdBuf(void *userData, void *cmdBuf, uint32_t len) {
  sendCmdBuf(userData, cmdBuf, len);
  /* free implicitly */
  free(cmdBuf);
}

void TGCodeEmitter::freeCmdBuf(void *userData, void *cmdBuf) {
  /* free without emit */
  emitCmdBuf(userData, cmdBuf, 0);
}

void *TGCodeEmitter::allocCmdBuf(void *userData, uint32_t size) {
  void *cmdBuf = malloc(size);
  return cmdBuf;
}

void TGCodeEmitter::hostSync(void) {}

void TGCodeEmitter::emitDebugInfo(void *userData, char const *info, int nodeId,
                                  long long unsigned int fwAddr, bool isFloat) {
}

void TGCodeEmitter::bmkernelContextPrepare(void) {
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
  bmkernelInfo.alloc_cmd_buf = TGCodeEmitter::allocCmdBuf;
  bmkernelInfo.emit_cmd_buf = TGCodeEmitter::emitCmdBuf;
  bmkernelInfo.free_cmd_buf = TGCodeEmitter::freeCmdBuf;
  bmkernelInfo.sync = TGCodeEmitter::hostSync;
  bmkernelInfo.debug = TGCodeEmitter::emitDebugInfo;

  bmkernel_register(&bmkernelInfo, BMKERNEL_NODE_ID_HOST, (void *)0,
                    &m_bmkernelHandle);
  std::cout << "bmkernel_register done" << std::endl;
}

void TGCodeEmitter::encodeInstructions(void) {
  // init bm1680 context
  bmkernelContextPrepare();
  kernel_enter(m_bmkernelHandle);
  auto &instList = m_tgBackend->getInsts();
  for (auto const &i : instList) {
    i->emit();
  }
  instList.clear();
  kernel_submit();
  kernel_exit();
}
