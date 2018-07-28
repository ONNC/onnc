//===- Initializer.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

char Initializer::ID = 0;

//===----------------------------------------------------------------------===//
// Initializer
//===----------------------------------------------------------------------===//
Initializer::Initializer()
  : ComputeOperator("Initializer", ID),
    m_Name() {
}

Initializer::Initializer(const StringAttr& pName)
  : ComputeOperator("Initializer", ID),
    m_Name(pName) {
}

Initializer::~Initializer()
{
}

void Initializer::setTensor(onnc::Tensor& pTensor)
{
  if (!m_Outputs.empty()) {
    // fatal error
    return;
  }
  m_Outputs.push_back(&pTensor);
}

void Initializer::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Initializer::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
