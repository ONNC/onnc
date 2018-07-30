//===- Weight.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_BM188X_WEIGHT_H
#define ONNC_TARGET_TG_BM188X_WEIGHT_H
#include "ComputeOperator.h"
#include "../TGBackend.h"
#include <onnc/Support/DataTypes.h>
#include <vector>

namespace onnc {
namespace BM188X {

class Weight
{
public:
  typedef int8_t DataType;
  typedef std::vector<DataType> WeightType;

public:
  static void prepare8bitWeight(const MemOperand &pMemOp, WeightType& pThis);

  static void prepare16bitWeight(const MemOperand &pMemOp, WeightType& pThis);

  void prepareWeight(TGBackend::Instructions& pInstructions,
                     TGBackend::MemOperands& pMemOperands);

  void genWeightBin(const std::string &pOutputFilename,
                    TGBackend::Instructions& pInstructions,
                    TGBackend::MemOperands& pMemOperands);

private:
  WeightType m_Weight;
};

} // namespace of BM188X
} // namespace of onnc

#endif
