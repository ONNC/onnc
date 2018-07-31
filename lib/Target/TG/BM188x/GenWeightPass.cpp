//===- GenWeightPass.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GenWeightPass.h"
#include <onnc/Target/TG/io.hpp>

using namespace onnc;

char BM188X::GenWeightPass::ID = 0;

//===----------------------------------------------------------------------===//
// GenWeightPass
//===----------------------------------------------------------------------===//
BM188X::GenWeightPass::GenWeightPass(const Path &pOutFile)
    : ModulePass(ID), m_OutFile(pOutFile), m_Weight(), m_DoneOpndSet()
{
}

Pass::ReturnType BM188X::GenWeightPass::runOnModule(Module &pModule)
{
  fillWeight(pModule);
  if (!m_Weight.empty())
    bmnet::WriteInt8DataToBinaryFile(&m_Weight, m_OutFile.c_str());
  return kModuleNoChanged;
}

void BM188X::GenWeightPass::fillWeight(const Module& pModule)
{
  // TODO
}

bool BM188X::GenWeightPass::isWritten(const ComputeMemOperand* pOpnd) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(pOpnd));
}

void BM188X::GenWeightPass::setWritten(const ComputeMemOperand* pOpnd)
{
  m_DoneOpndSet.insert(pOpnd);
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenWeightPass(const Path& pOutFile)
{
  return new GenWeightPass(pOutFile);
}
