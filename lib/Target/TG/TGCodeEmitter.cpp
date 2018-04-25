#include "TGCodeEmitter.h"
#include "plat-bm168x/BM168xBackendContext.hpp"
#include "plat-bm188x/BM188xBackendContext.hpp"
#include "utils/io.hpp"
#include <bm_kernel.h>
#include <cstdint>
#include <fstream>

using namespace onnc;

TGCodeEmitter::TGCodeEmitter(TGBackend *tgBackend) : m_tgBackend(tgBackend) {}

void TGCodeEmitter::encodeInstructions(Path &outputPath) {
  auto m_outputPath = Path("cmdbuf.bin");
  if (!outputPath.empty())
    m_outputPath = outputPath;
  // tap2
  std::vector<float> weight_data;
  // ReadFloatDataFromBinaryFile(weight, weight_data);
  bmnet::BM168xBackendContext ctx(BM_CHIP_BM1680, 1, weight_data);
  tg_kernel::getInstance().ctx = &ctx;
  // StartInst::encode()
  kernel_enter(ctx.get_bmkernel_handle());
  // onnc part
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
                                    m_outputPath.generic_string());
}
