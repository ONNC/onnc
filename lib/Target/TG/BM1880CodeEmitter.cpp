#include "BM1880CodeEmitter.h"
#include "plat-bm188x/BM188xBackendContext.hpp"
#include "utils/io.hpp"
#include <bm_kernel.h>
#include <cstdint>
#include <fstream>

#define DEBUG_TYPE "bm1880_code_emitter"
#include <onnc/Support/Debug.h>
using namespace onnc;

BM1880CodeEmitter::BM1880CodeEmitter(BM1880Backend *pBackend)
    : TGCodeEmitter(pBackend), m_Backend(pBackend)
{
}

void BM1880CodeEmitter::encodeInstructions(const Path &pOutputPath)
{
  Path output_path("cmdbuf.bin");
  if (!pOutputPath.empty())
    output_path = pOutputPath;

  DEBUG(dbgs() << "BM1880Backend BM1880CodeEmitter::encodeInstructions\n");
// TODO
#if 0
  std::vector<int8_t> weight_data;
  // ReadInt8DataFromBinaryFile(weight, weight_data);
  bmnet::BM188xBackendContext ctx(BM_CHIP_BM1880, 1, weight_data);
  bm1880_kernel::getInstance().m_Ctx = &ctx;
  // StartInst::encode()
  kernel_enter(ctx.get_bmkernel_handle());

  auto &instList = m_tgBackend->getInsts();
  for (auto const &i : instList)
    i->emit();
  instList.clear();
  // EndInst::encode()
  kernel_submit();
  kernel_exit();
  // output
  std::vector<uint8_t> cmdbuf;
  ctx.read_cmdbuf(cmdbuf);
  bmnet::WriteFloatDataToBinaryFile(cmdbuf.data(), cmdbuf.size(),
                                    output_path.generic_string());
#endif
}
