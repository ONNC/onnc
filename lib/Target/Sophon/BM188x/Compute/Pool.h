//===- Pool.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TLPOOL_H
#define ONNC_TARGET_BM1880_TLPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>

namespace onnc {
namespace BM188X {

class Pool : public ComputeOperator
{
public:
  static char ID;

public:
  Pool(const IntAttr& pIFmapAddr,
       const IntAttr& pOFmapAddr,
       const IntsAttr& pInDim,
       const IntsAttr& pOutDim,
       bool pIsAvgPooling,
       const StringAttr& pSpliteName);

  const IntAttr &getIFmapAddr() const { return m_IFmapAddr; }

  const IntAttr &getOFmapAddr() const { return m_OFmapAddr; }

  const IntsAttr &getInDim() const { return m_InDim; }

  const IntsAttr &getOutDim() const { return m_OutDim; }

  const BoolAttr &getIsAvgPooling() const { return m_IsAvgPooling; }

  const StringAttr &getSplitName() const { return m_SplitName; }

  void setKernelShape(const IntsAttr &pKS) { m_KernelShape = pKS; }

  const IntsAttr &getKernelShape() const { return m_KernelShape; }

  void setSlidePads(const IntsAttr &pSlidePads) { m_SlidePads = pSlidePads; }

  const IntsAttr &getSlidePads() const { return m_SlidePads; }

  void setStrides(const IntsAttr &pStrides) { m_Strides = pStrides; }

  const IntsAttr &getStrides() const { return m_Strides; }

  void setRShiftWidth(int pRShiftWidth) { m_RShiftWidth = pRShiftWidth; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setThresholdXQuantized(int pTXQ) { m_ThresholdXQuantized = pTXQ; }

  int getThresholdXQuantized() const { return m_ThresholdXQuantized; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_IFmapAddr;
  IntAttr m_OFmapAddr;
  IntsAttr m_InDim;
  IntsAttr m_OutDim;
  BoolAttr m_IsAvgPooling;
  StringAttr m_SplitName;

  IntsAttr m_KernelShape;
  IntsAttr m_SlidePads;
  IntsAttr m_Strides;

  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
