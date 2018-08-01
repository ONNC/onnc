//===- GenRuntimeInfoPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GenRuntimeInfoPass.h"

using namespace onnc;

char BM188X::GenRuntimeInfoPass::ID = 0;

//===----------------------------------------------------------------------===//
// GenRuntimeInfoPass
//===----------------------------------------------------------------------===//
BM188X::GenRuntimeInfoPass::GenRuntimeInfoPass(TGBackend* pBackend,
                                               const Path &pOutFile)
    : ModulePass(ID), m_pBackend(pBackend), m_OutFile(pOutFile)
{
}

Pass::ReturnType BM188X::GenRuntimeInfoPass::runOnModule(Module &pModule)
{
  return kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenRuntimeInfoPass(TGBackend* pBackend, const Path& pOutFile)
{
  return new GenRuntimeInfoPass(pBackend, pOutFile);
}
