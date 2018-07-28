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

  virtual void encodeInstructions(std::ostream &pOS) = 0;

  virtual void genWeightBin(const std::string &pOutputFilename) { return; }

  virtual void genRuntimeInfo(const ::onnx::Graph *pOnnxGraph,
                              std::ostream &pOS) = 0;

private:
  // void *m_BmkernelHandle;
  // TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
