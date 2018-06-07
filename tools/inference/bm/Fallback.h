//===- Fallback.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef FALLBACK_H
#define FALLBACK_H

#include <onnc/Core/ModulePass.h>
#include <onnc/JSON/Object.h>
#include <onnc/JSON/Value.h>

#include <caffe2/core/init.h>
#include <caffe2/core/net.h>

namespace onnc {
ModulePass *createFallbackPass(float *pData, size_t pSize, json::Object pJRoot);
}

namespace onnc {

class Fallback : public ModulePass
{
public:
  static char ID;
  Fallback(float *pData, size_t pSize, const json::Object pJRoot)
      : ModulePass(ID), m_pData(pData), m_InputSize(pSize), m_JRoot(pJRoot)
  {
  }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

private:
  void prepareInput(void);

  bool executeLayers(void);

private:
  using BlobData =
      std::unordered_map<std::string, std::vector<caffe2::TensorCPU> >;
  BlobData m_BlobData;

  float *m_pData;
  size_t m_InputSize;
  json::Object m_JRoot;
};
} // namespace onnc

#endif // FALLBACK
