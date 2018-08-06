//===- LowerRegistry.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/LowerRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LowerRegistry
//===----------------------------------------------------------------------===//
LowerRegistry::LowerRegistry()
  : m_LowerList() {
}

LowerRegistry::~LowerRegistry()
{
  clear();
}

LowerRegistry::iterator LowerRegistry::begin()
{
  return m_LowerList.begin();
}

LowerRegistry::iterator LowerRegistry::end()
{
  return m_LowerList.end();
}

LowerRegistry::const_iterator LowerRegistry::begin() const
{
  return m_LowerList.begin();
}

LowerRegistry::const_iterator LowerRegistry::end() const
{
  return m_LowerList.end();
}

bool LowerRegistry::empty() const
{
  return m_LowerList.empty();
}

unsigned int LowerRegistry::size() const
{
  return m_LowerList.size();
}

void LowerRegistry::clear()
{
  iterator lower, lEnd = end();
  for (lower = begin(); lower != lEnd; ++lower) {
    delete *lower;
  }
  m_LowerList.clear();
}

Lower* LowerRegistry::lookup(const xNode& pNode)
{
  int max = 0;
  Lower* target = nullptr;
  for (Lower* lower : m_LowerList) {
    int score = lower->isMe(pNode);
    if (score > max)
      target = lower;
  }
  return target;
}

const Lower* LowerRegistry::lookup(const xNode& pNode) const
{
  int max = 0;
  const Lower* target = nullptr;
  for (const Lower* lower : m_LowerList) {
    int score = lower->isMe(pNode);
    if (score > max)
      target = lower;
  }
  return target;
}
