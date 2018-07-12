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
  typedef LowerList::const_iterator const_iterator;

public:
  LowerRegistry();

  ~LowerRegistry();

  template<typename LowerType, typename ... LowerParams>
  Lower* emplace(LowerParams&& ... pParams);

  template<typename LowerType, typename ... LowerParams>
  Lower* emplace(Lower::QualityMatchFnTy pMatchFn, LowerParams&& ... pParams);

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  bool empty() const;

  unsigned int size() const;

  void clear();

  Lower* lookup(const ::onnx::Node& pNode);

  const Lower* lookup(const ::onnx::Node& pNode) const;

private:
  LowerList m_LowerList;
};

template<typename LowerType, typename ... LowerParams>
Lower* LowerRegistry::emplace(LowerParams&& ... pParams)
{
  LowerType* lower = new LowerType(pParams...);
  m_LowerList.push_back(lower);
  return lower;
}

template<typename LowerType, typename ... LowerParams> Lower*
LowerRegistry::emplace(Lower::QualityMatchFnTy pMatchFn, LowerParams&& ... pParams)
{
  LowerType* lower = new LowerType(pParams...);
  lower->setQualityMatchFn(pMatchFn);
  m_LowerList.push_back(lower);
  return lower;
}

} // namespace of onnc

#endif
