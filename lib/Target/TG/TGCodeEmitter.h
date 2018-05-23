#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include <onnc/Support/Path.h>
#include <vector>
#include <memory>

namespace onnc {

class TGBackend;

class TGCodeEmitter
{
public:
  TGCodeEmitter(TGBackend *pBackend) : m_pBackend(pBackend) {}

  virtual ~TGCodeEmitter() = default;

  virtual void encodeInstructions(const Path &pOutputPath) = 0;

private:
  void *m_bmkernelHandle;
  TGBackend *m_pBackend;
};

} // namespace onnc

#endif
