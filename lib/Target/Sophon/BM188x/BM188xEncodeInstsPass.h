//===- EncodeInstructions.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SOPHON_ENCODE_INSTRUCTIONS_PASS_H
#define ONNC_SOPHON_ENCODE_INSTRUCTIONS_PASS_H
#include "BM188xBackend.h"
#include "CodeEmitVisitor.h"
#include "EncodeInstructionsPass.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Support/OFStream.h>
#include <onnc/Support/Path.h>

namespace onnc {
namespace BM188X {

class BM188xEncodeInsts : public EncodeInstructions
{
public:
  static char ID;

public:
  BM188xEncodeInsts(BM1880Backend *pBackend,
                    BM188X::CodeEmitVisitor *pInstVisitor,
                    const std::string &pFilename);

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

protected:
  void beforeEmit(const ::onnc::ComputeOperator *pOp) override;

private:
  BM1880Backend *m_pBackend;
  const std::string m_FileName;
};

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass *CreateEncodeInstsPass(BM1880Backend *pBackend,
                                  BM188X::CodeEmitVisitor *pVisitor,
                                  const std::string &pOutFile);

} // namespace BM188X
} // namespace onnc

#endif
