#ifndef ONNC_TARGET_TG_CODE_EMITTER_H
#define ONNC_TARGET_TG_CODE_EMITTER_H
#include <memory>
#include <onnc/Support/Path.h>
#include <vector>

namespace onnc {

class TGBackend;

class TGCodeEmitter
{
public:
  TGCodeEmitter(TGBackend *pBackend)
  {
    (void)pBackend; // WIP
  }

  virtual ~TGCodeEmitter() = default;

  virtual void encodeInstructions(const Path &pOutputPath) = 0;

  virtual void genRuntimeInfo(const ::onnx::Graph *pOnnxGraph) = 0;

private:
  // void *m_BmkernelHandle;
  // TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
