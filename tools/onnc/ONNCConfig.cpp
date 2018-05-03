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
  : m_Inputs(), m_Output(), m_Quadruple(), m_Arch() {
}

ONNCConfig::~ONNCConfig()
{
}

ONNCConfig& ONNCConfig::addInput(const Path& pFilePath)
{
  m_Inputs.push_back(pFilePath);
  return *this;
}

std::vector<Path> ONNCConfig::inputs() const
{
  return m_Inputs;
}

void ONNCConfig::setQuadruple(const std::string& pValue)
{
  m_Quadruple = Quadruple(pValue);
}
