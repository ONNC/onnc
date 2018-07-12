//===- TensorSel.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Core/PassSupport.h>

using namespace onnc;

char TensorSel::ID = 0;

//===----------------------------------------------------------------------===//
// TensorSel
//===----------------------------------------------------------------------===//
TensorSel::TensorSel(const TargetBackend* pBackend)
  : ModulePass(ID), m_pBackend(pBackend), m_LowerRegistry() {
  if (nullptr != m_pBackend) {
    m_pBackend->RegisterLowers(m_LowerRegistry);
  }
}

TensorSel::~TensorSel()
{
  m_LowerRegistry.clear();
}

Pass::ReturnType TensorSel::runOnModule(::onnc::Module &pModule)
{
  // TODO
  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_TB_PASS(TensorSel, "TensorSel");

ModulePass *onnc::CreateTensorSel(const TargetBackend* pBackend)
{
  return new TensorSel(pBackend);
}
