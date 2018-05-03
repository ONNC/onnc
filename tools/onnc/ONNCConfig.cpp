//===- ONNCConfig.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCConfig.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNCConfig
//===----------------------------------------------------------------------===//
ONNCConfig::ONNCConfig()
  : m_Input(), m_Output(), m_Quadruple(), m_Arch(), m_TargetOptions() {
}

ONNCConfig::~ONNCConfig()
{
}

void ONNCConfig::setQuadruple(const std::string& pValue)
{
  m_Quadruple = Quadruple(pValue);
}
