#ifndef ONNX_BM1880_TLLOAD_H
#define ONNX_BM1880_TLLOAD_H

#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

class TLLoad : public BM188xComputeOperator
{
public:
  TLLoad(const ::onnx::Node &pNode);

  void emit() const override;
  void print(OStream &pOS) const override;
  TLLoad *addMemOperands(MemOperand *pInput, MemOperand *pOutput);

private:
  uint64_t m_SrcGOffset;
  uint64_t m_DstLAddr;
  bool m_DoTranspose;
  int m_LocalN;
  int m_LocalC;
  int m_LocalH;
  int m_LocalW;
  int m_GlobalC;
  int m_GlobalH;
  int m_GlobalW;
  std::string m_SplitName;
};

} // namespace BM188X
} // namespace onnc

#endif
