//===- Scan.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Scan.h>

using namespace onnc;

char Scan::ID = 0;

//===----------------------------------------------------------------------===//
// Scan
//===----------------------------------------------------------------------===//
Scan::Scan(const GraphAttr& pBody, const IntAttr& pNumScanInputs)
  : ComputeOperator("Scan", ID),
    m_Body(pBody),
    m_Directions(),
    m_NumScanInputs(pNumScanInputs) {
}

Scan::Scan(const GraphAttr& pBody,
           const IntsAttr& pDirections,
           const IntAttr& pNumScanInputs)
  : ComputeOperator("Scan", ID),
    m_Body(pBody),
    m_Directions(pDirections),
    m_NumScanInputs(pNumScanInputs) {
}

Scan::Scan(const Scan& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Body(pCopy.getBody()),
    m_Directions(pCopy.getDirections()),
    m_NumScanInputs(pCopy.getNumScanInputs()) {
}

void Scan::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "body: " << getBody() << ", " "directions: " << getDirections() << ", " "num_scan_inputs: " << getNumScanInputs()<< '>';
}

bool Scan::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
