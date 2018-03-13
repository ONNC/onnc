#pragma once

#include "TGBackend.h"
#include <vector>
#include <memory>

class TGBackend;
class TGCodeEmitter {
public:
  TGCodeEmitter(TGBackend *tgBackend) : m_tgBackend(tgBackend) {}
  void encodeInstructions(void);

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
