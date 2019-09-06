//===- DivideGlobalAPIntoAPs.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIVIDE_GLOBALAP_INTO_APS
#define ONNC_DIVIDE_GLOBALAP_INTO_APS
#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {

/** \class DivideGlobalAPIntoAPs
 *  \brief When the kernel size of GlobalAveragePool > threshold,
 *         divide it into several AveragePool with all kernel sizes <= threshold 
 *
 *         Currently only support the input Tensor which follows the conditions:
 *         1. input tensor is in four dimensions: (N, C, H, W)
 *         2. the last two dims are the same (K)
 *         3. the constraint of hardware is that kernel size cannot be bigger
 *            than (maxKernelSize, maxKernelSize)
 *         4. the type of tensors should be float or double
 *
 *         TODO:
 *         1. input tensor can be in any dimensions: (N, C, D0, ...)
 */
class DivideGlobalAPIntoAPs: public CustomPass<DivideGlobalAPIntoAPs>
{
public:
  typedef int64_t ValueType;
  typedef std::vector<ValueType> VectorType;

public:
  DivideGlobalAPIntoAPs(const ValueType& maxKernelSize): m_MaxKernelSize(maxKernelSize) {}
  DivideGlobalAPIntoAPs(): DivideGlobalAPIntoAPs(8) {}

  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

  // Given a kernel size, return a list of kernel sizes of AveragePool
  static VectorType divideKernelSizeOf(const ValueType& kernelSize, const ValueType& maxKernelSize);

  // Given a kernel size, return a list of kernel sizes of AveragePool
  // These number can compose the kernel size without pads
  // If there is no solution, the size of return IntsAttr will be zero
  static VectorType canBeComposedOf(const ValueType& kernelSize, const ValueType& maxKernelSize);

  // Get k, where maxKernelSize ^ (k-1) < kernelSize <= maxKernelSize ^ k
  static ValueType getBestSize(const ValueType& kernelSize, const ValueType& maxKernelSize);

private:
  template <int T = 0>
  std::pair<ComputeOperator*, ComputeOperator*>
    genListOfAPsMul(ComputeGraph& pCG,
                    const Tensor* inputTensor,
                    const VectorType& lstOfKernels);

  template <typename FirstTensorType, typename... RestTensorTypes, int T = 0>
  std::pair<ComputeOperator*, ComputeOperator*>
    genListOfAPsMul(ComputeGraph& pCG,
                    const Tensor* inputTensor,
                    const VectorType& lstOfKernels);
                                            
private:
  ValueType m_MaxKernelSize;
  static unsigned tensorIdx;
};

} // namespace of onnc

#endif // ONNC_DIVIDE_GLOBALAP_INTO_APS
