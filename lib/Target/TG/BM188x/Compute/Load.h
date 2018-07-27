//===- Load.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_LOAD_H
#define ONNC_TARGET_BM1880_LOAD_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>

namespace onnc {
namespace BM188X {

/** \class Load
 *  \brief BM188X-dependent Load operator
 */
class Load : public ComputeOperator
{
public:
  Load(const IntAttr &pSrcGOffset,
       const IntAttr &pDstLAddr,
       const BoolAttr &pDoTranspose,
       const BoolAttr &pIsAligned,
       const BoolAttr &pIsNeuron,
       const IntsAttr &pLocalDim,
       const IntsAttr &pGlobalDim,
       const StringAttr &pSplitName);

  const IntAttr &getSrcGOffset() const { return m_SrcGOffset; }

  const IntAttr &getDstLAddr() const { return m_DstLAddr; }

  const BoolAttr &getDoTranspose() const { return m_DoTranspose; }

  const BoolAttr &getIsAligned() const { return m_IsAligned; }

  const BoolAttr &getIsNeuron() const { return m_IsNeuron; }

  const IntsAttr &getLocalDim() const { return m_LocalDim; }

  const IntsAttr &getGlobalDim() const { return m_GlobalDim; }

  const StringAttr &getSplitName() const { return m_SplitName; }

private:
  IntAttr m_SrcGOffset;
  IntAttr m_DstLAddr;
  BoolAttr m_DoTranspose;
  BoolAttr m_IsAligned;
  BoolAttr m_IsNeuron;
  IntsAttr m_LocalDim;
  IntsAttr m_GlobalDim;
  StringAttr m_SplitName;
};

} // namespace BM188X
} // namespace onnc

#endif
