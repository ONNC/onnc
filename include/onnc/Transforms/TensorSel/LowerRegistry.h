//===- LowerRegistry.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_LOWER_REGISTRY_H
#define ONNC_TRANSFORM_LOWER_REGISTRY_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/ADT/StringRef.h>

namespace onnc {

/** \class LowerRegistry
 */
class LowerRegistry
{
public:
  typedef std::vector<Lower*> LowerList;
  typedef LowerList::iterator iterator;

public:
  static void RegisterLower(Lower& pLower, Lower::QualityMatchFnTy pMatchFn);

  static iterator Begin();

  static iterator End();

  static bool IsEmpty();

  static unsigned int Size();

  static Lower* LookUp(const ::onnx::Node& pNode);
};

} // namespace of onnc

#endif
