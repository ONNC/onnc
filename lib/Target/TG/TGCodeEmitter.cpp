#include "TGCodeEmitter.h"
#include "TGBackend.h"
#include "plat-bm168x/BM168xBackendContext.hpp"
#include "utils/io.hpp"
#include <bm_kernel.h>
#include <cstdint>
#include <fstream>

#define DEBUG_TYPE "tg_code_emitter"
#include <onnc/Support/Debug.h>
using namespace onnc;

TGCodeEmitter::TGCodeEmitter(TGBackend *pTgBackend) : m_pTGBackend(pTgBackend)
{
}

void TGCodeEmitter::encodeInstructions(const Path &pOutputPath)
{
  Path output_path("cmdbuf.bin");
  if (!pOutputPath.empty())
    output_path = pOutputPath;

  // TODO refactor
  if (dynamic_cast<BM1680Backend *>(m_pTGBackend)) {
    DEBUG(dbgs() << "BM1680Backend TGCodeEmitter::encodeInstructions\n");
    // tap2
    std::vector<float> weight_data;
    // ReadFloatDataFromBinaryFile(weight, weight_data);
    bmnet::BM168xBackendContext ctx(BM_CHIP_BM1680, 1, weight_data);
    tg_kernel::getInstance().ctx = &ctx;
    // StartInst::encode()
    kernel_enter(ctx.get_bmkernel_handle());

    // XXX: we use auto only when the type is keeping changing.
    // onnc part
    auto &instList = m_pTGBackend->getInsts();
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
    return;
  }
  assert(0);
}
