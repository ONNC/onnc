//===- Store.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TL_STORE_H
#define ONNC_TARGET_BM1880_TL_STORE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>

namespace onnc {
namespace BM188X {

class Store : public ComputeOperator
{
public:
  static char ID;

public:
  Store(const IntAttr& pDstGOffset,
        const IntAttr& pSrcLAddr,
        const BoolAttr& pDoTranspose,
        const BoolAttr& pIsAligned,
        const BoolAttr& pIsNeuron,
        const IntsAttr& pLocalDim,
        const IntsAttr& pGlobalDim,
        const StringAttr& pSplitName);

  const IntAttr& getDstGOffset() const { return m_DstGOffset; }

  const IntAttr& getSrcLAddr() const { return m_SrcLAddr; }

  const BoolAttr& getDoTranspose() const { return m_DoTranspose; }

  const BoolAttr& getIsAligned() const { return m_IsAligned; }

  const BoolAttr& getIsNeuron() const { return m_IsNeuron; }

  const IntsAttr& getLocalDim() const { return m_LocalDim; }

  const IntsAttr& getGlobalDim() const { return m_GlobalDim; }

  const StringAttr& getSplitName() const { return m_SplitName; }

  void setDstGOffset(const IntAttr& pOffset) { m_DstGOffset = pOffset; }

  void setSrcLAddr(const IntAttr& pSrcLAddr) { m_SrcLAddr = pSrcLAddr; }

  void setDoTranspose(const BoolAttr& pT) { m_DoTranspose = pT; }

  void setIsAligned(bool pIsAligned) { m_IsAligned = pIsAligned; }

  void setIsNeuron(bool pIsNeuron) { m_IsNeuron = pIsNeuron; }

  void setSplitName(const std::string& pSplitName) { m_SplitName = pSplitName; }

  void setLocalDim(const IntsAttr& pLocalDim) { m_LocalDim = pLocalDim; }

  void setGlobalDim(const IntsAttr& pGlobalDim) { m_GlobalDim = pGlobalDim; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_DstGOffset;
  IntAttr m_SrcLAddr;
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
