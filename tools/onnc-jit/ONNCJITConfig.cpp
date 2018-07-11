//===- ONNCJITConfig.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCJITConfig.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNCJITConfig
//===----------------------------------------------------------------------===//
ONNCJITConfig::ONNCJITConfig()
  : m_Input(), m_Output(), m_Quadruple(), m_Arch(), m_TargetOptions() {
}

ONNCJITConfig::~ONNCJITConfig()
{
}

void ONNCJITConfig::setQuadruple(const std::string& pValue)
{
  m_Quadruple = Quadruple(pValue);
}
