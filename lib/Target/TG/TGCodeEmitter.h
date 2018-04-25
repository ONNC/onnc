#pragma once

#include "TGBackend.h"
#include <onnc/Support/Path.h>
#include <vector>
#include <memory>
namespace bmnet{
  class BM168xBackendContext;
}
namespace onnc {
class TGBackend;
class TGCodeEmitter {
public:
  TGCodeEmitter(TGBackend *tgBackend);
  void encodeInstructions(Path &m_outputPath);

private:
  static void sendCmdBuf(void *userData, const void *cmdBuf, uint32_t len);
  static void emitCmdBuf(void *userData, void *cmdBuf, uint32_t len);
  static void freeCmdBuf(void *userData, void *cmdBuf);
  static void *allocCmdBuf(void *userData, uint32_t size);
  static void hostSync(void);
  static void emitDebugInfo(void *userData, char const *info, int nodeId,
                            long long unsigned int fwAddr, bool isFloat);
  void bmkernelContextPrepare(void);
  void *m_bmkernelHandle;
  TGBackend *m_tgBackend;
};

class tg_kernel {
public:
  static tg_kernel &getInstance() {
    static tg_kernel instance; // Guaranteed to be destroyed.
                               // Instantiated on first use.
    return instance;
  }

private:
  tg_kernel() {} // Constructor? (the {} brackets) are needed here.

public:
  tg_kernel(tg_kernel const &) = delete;
  void operator=(tg_kernel const &) = delete;
bmnet::BM168xBackendContext *ctx;
};
} // namespace onnc
