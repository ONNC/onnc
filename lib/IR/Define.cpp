//===- Define.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Define.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Define
//===----------------------------------------------------------------------===//
Define::Define(StringRef pName)
  : m_Name(pName) {
}

Define::Define(const Define& pCopy)
  : m_Name(pCopy.name()) {
}
