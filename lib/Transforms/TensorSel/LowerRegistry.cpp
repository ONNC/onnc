//===- LowerRegistry.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/LowerRegistry.h>
#include <onnc/Support/ManagedStatic.h>

using namespace onnc;

static ManagedStatic<LowerRegistry::LowerList> s_LowerList;

//===----------------------------------------------------------------------===//
// LowerRegistry
//===----------------------------------------------------------------------===//
void
LowerRegistry::RegisterLower(Lower& pLower, Lower::QualityMatchFnTy pMatchFn)
{
  pLower.setQualityMatchFn(pMatchFn);
  s_LowerList->push_back(&pLower);
}

LowerRegistry::iterator LowerRegistry::Begin()
{
  return s_LowerList->begin();
}

LowerRegistry::iterator LowerRegistry::End()
{
  return s_LowerList->end();
}

bool LowerRegistry::IsEmpty()
{
  return s_LowerList->empty();
}

unsigned int LowerRegistry::Size()
{
  return s_LowerList->size();
}

Lower* LowerRegistry::LookUp(const ::onnx::Node& pNode)
{
  int max = 0;
  Lower* target = nullptr;
  for (Lower* lower : *s_LowerList) {
    int score = lower->isMe(pNode);
    if (score > max)
      target = lower;
  }
  return target;
}
