//===- ONNIConfig.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIConfig.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNIConfig
//===----------------------------------------------------------------------===//
ONNIConfig::ONNIConfig()
  : m_Model(), m_Input(), m_Output(),
    m_Quadruple(), m_Arch(), m_TargetOptions(),
    m_Verbose(), m_DryRun(), m_OnnxOpt() {
}

ONNIConfig::~ONNIConfig()
{
}

void ONNIConfig::setQuadruple(const std::string& pValue)
{
  m_Quadruple = Quadruple(pValue);
}
