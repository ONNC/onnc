#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include "TGBackend.h"
#include <onnc/Support/Path.h>
#include <vector>
#include <memory>

namespace bmnet {

class BM168xBackendContext;

} // namespace of bmnet

namespace onnc {

class TGBackend;

class TGCodeEmitter
{
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

private:
  void *m_bmkernelHandle;
  TGBackend *m_tgBackend;
};

class tg_kernel
{
public:
  static tg_kernel &getInstance() {
    static tg_kernel instance; // Guaranteed to be destroyed.
                               // Instantiated on first use.
    return instance;
  }

public:
  bmnet::BM168xBackendContext *ctx;

private:
  tg_kernel(tg_kernel const &) = delete;

  void operator=(tg_kernel const &) = delete;

  tg_kernel() {} // Constructor? (the {} brackets) are needed here.

  ~tg_kernel() {}
};

} // namespace onnc

#endif
