//===- SlicedConv.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_SLICED_CONV_H
#define ONNC_TARGET_BM1880_SLICED_CONV_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>

namespace onnc {
namespace BM188X {

class SlicedConv : public ComputeOperator
{
public:
  static char ID;

public:
  SlicedConv(const IntAttr& pIFmapAddr,
             const IntAttr& pOFmapAddr,
             const IntAttr& pWeightAddr,
             const IntAttr& pDoResultAdd,
             const IntsAttr& pInDim,
             const IntsAttr& pOutDim,
             const StringAttr& pSpliteName);

  const IntAttr &getIFmapAddr() const { return m_IFmapAddr; }

  const IntAttr &getOFmapAddr() const { return m_OFmapAddr; }

  const IntAttr &getWeightAddr() const { return m_WeightAddr; }

  const IntsAttr &getInDim() const { return m_InDim; }

  const IntsAttr &getOutDim() const { return m_OutDim; }

  const StringAttr &getSplitName() const { return m_SplitName; }

  void setKernelShape(const IntsAttr &pKS) { m_KernelShape = pKS; }

  const IntsAttr &getKernelShape() const { return m_KernelShape; }

  void setSlidePads(const IntsAttr &pSlidePads) { m_SlidePads = pSlidePads; }

  const IntsAttr &getSlidePads() const { return m_SlidePads; }

  void setStrides(const IntsAttr &pStrides) { m_Strides = pStrides; }

  const IntsAttr &getStrides() const { return m_Strides; }

  const IntAttr &getBiasAddr() const { return m_BiasAddr; }

	void setBias(const IntAttr& pBiasAddr)
	{
		m_DoBias = true;
		m_BiasAddr = pBiasAddr;
	}

  bool getDoRelu() const { return m_DoRelu; }

  void setDoRelu() { m_DoRelu = true; }

  void setGroups(const IntAttr &pGroups) { m_Groups = pGroups; }

  const IntAttr &getGroups() const { return m_Groups; }

  void setRShiftWidth(int pRShiftWidth) { m_RShiftWidth = pRShiftWidth; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_IFmapAddr;
  IntAttr m_OFmapAddr;
  IntAttr m_WeightAddr;
	BoolAttr m_DoResultAdd;
  IntsAttr m_InDim;
  IntsAttr m_OutDim;

  bool m_DoRelu;
	BoolAttr m_DoBias;
  IntAttr m_BiasAddr;
  StringAttr m_SplitName;

  IntsAttr m_KernelShape;
  IntsAttr m_SlidePads;
  IntsAttr m_Strides;

	IntAttr m_Groups;
  int m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif