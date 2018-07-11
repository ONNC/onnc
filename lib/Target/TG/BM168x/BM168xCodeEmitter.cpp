#include "BM168xCodeEmitter.h"
#include <bm_kernel.h>
#include <bmnet/targets/plat-bm168x/BM168xBackendContext.hpp>
#include <bmnet/utils/io.hpp>
#include <cstdint>
#include <fstream>

#define DEBUG_TYPE "bm168x_code_emitter"
#include <onnc/Support/Debug.h>
using namespace onnc;

BM168xCodeEmitter::BM168xCodeEmitter(TGBackend *pBackend)
    : TGCodeEmitter(pBackend), m_pBackend(pBackend)
{
}

void BM168xCodeEmitter::encodeInstructions(const Path &pOutputPath)
{
  Path output_path("cmdbuf.bin");
  if (!pOutputPath.empty())
    output_path = pOutputPath;

  // TODO refactor
  if (dynamic_cast<BM1680Backend *>(m_pBackend)) {
    DEBUG(dbgs() << "BM1680Backend BM168xCodeEmitter::encodeInstructions\n");
    // tap2
    std::vector<float> weight_data;
    // ReadFloatDataFromBinaryFile(weight, weight_data);
    bmnet::BM168xBackendContext ctx(BM_CHIP_BM1680, 1, weight_data);
    bm168x_kernel::getInstance().m_CTX = &ctx;
    // StartInst::encode()
    kernel_enter(ctx.get_bmkernel_handle());

    // XXX: we use auto only when the type is keeping changing.
    // onnc part
    auto &instList = m_pBackend->getInsts();
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
  // TODO BM1682
  assert(0);
}

void BM168xCodeEmitter::genRuntimeInfo(const ::onnx::Graph *pOnnxGraph)
{
  // TODO: Generate runtime info for BM168x.
  return;
}
