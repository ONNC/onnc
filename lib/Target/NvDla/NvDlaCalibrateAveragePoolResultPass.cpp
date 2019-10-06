//===- NvDlaCalibrateAveragePoolResultPass.cpp ----------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "NvDlaCalibrateAveragePoolResultPass.h"

#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>

#include <algorithm>
#include <iterator>
#include <memory>

namespace onnc {
namespace internal {

bool shouldCalibrateResult(const AveragePool& averagePool)
{
  if (4 != averagePool.getX()->getNumOfDimensions()) {
    return false;
  }

  // only supports float tensor
  const auto* const output = dynamic_cast<const FloatTensor*>(averagePool.getY());
  if (output == nullptr) {
    return false;
  }

  const bool shouldCountIncludePad = static_cast<bool>(averagePool.getCountIncludePad().value());
  if (shouldCountIncludePad) {
    return false;
  }

  using std::begin;
  using std::end;
  const IntsAttr::VectorType& pads = averagePool.getPads().vector();
  return std::any_of(begin(pads), end(pads), [](IntsAttr::ValueType value) { return value != 0; });
}

std::unique_ptr<Tensor> getCalibrationScalar(const AveragePool& averagePool)
{
  const IntsAttr::VectorType& pads = averagePool.getPads().vector();
  assert(pads.size() == 4);

  const Tensor::Dimension padTop    = static_cast<Tensor::Dimension>(pads[0]);
  const Tensor::Dimension padLeft   = static_cast<Tensor::Dimension>(pads[1]);
  const Tensor::Dimension padBottom = static_cast<Tensor::Dimension>(pads[2]);
  const Tensor::Dimension padRight  = static_cast<Tensor::Dimension>(pads[3]);
  assert(0 <= padTop && 0 <= padLeft && 0 <= padBottom && 0 <= padRight);

  const IntsAttr::VectorType& kernelShape = averagePool.getKernelShape().vector();
  assert(kernelShape.size() == 2);

  const IntsAttr::VectorType& strides = averagePool.getStrides().vector();
  assert(strides.size() == 2);

  const Tensor::Dimension kernelHeight              = static_cast<Tensor::Dimension>(kernelShape[0]);
  const Tensor::Dimension kernelWidth               = static_cast<Tensor::Dimension>(kernelShape[1]);
  const Tensor::Dimension inputHeight               = averagePool.getX()->dimension(2);
  const Tensor::Dimension inputWidth                = averagePool.getX()->dimension(3);
  const Tensor::Dimension inputHeightIncludePadding = padTop + inputHeight + padBottom;
  const Tensor::Dimension inputWidthIncludePadding  = padLeft + inputWidth + padRight;

  const Tensor::Dimension verticalStride   = static_cast<Tensor::Dimension>(strides[0]);
  const Tensor::Dimension horizontalStride = static_cast<Tensor::Dimension>(strides[1]);

  using include_flags_type = std::vector<bool>;

  // create a flag map to keep track which elements should be counted when doing average (=true)
  // for example:
  //
  //   X = [?, ?, 2, 2], pads = [1, 1, 1, 1], kernel_shape = [4, 4], strides = [1, 1]
  //
  // the includeFlags will be:
  //
  //   [0, 0, 0, 0,
  //    0, 1, 1, 0,
  //    0, 1, 1, 0,
  //    0, 0, 0, 0]
  //
  include_flags_type includeFlags(inputHeightIncludePadding * inputWidthIncludePadding, false);
  for (Tensor::Dimension y = 0; y < inputHeight; ++y) {
    for (Tensor::Dimension x = 0; x < inputWidth; ++x) {
      const include_flags_type::size_type offset = (padTop + y) * inputWidthIncludePadding + (padLeft + x);
      assert(offset < includeFlags.size());

      includeFlags[offset] = true;
    }
  }

  const Tensor::Dimension outputHeight = averagePool.getY()->dimension(2);
  const Tensor::Dimension outputWidth  = averagePool.getY()->dimension(3);

  using calibration_scalar_type  = float;
  using calibration_scalars_type = std::vector<calibration_scalar_type>;
  calibration_scalars_type calibrationScalars;
  calibrationScalars.reserve(outputHeight * outputWidth);

  // use kernel to scan each position and calculate calibration scalars
  // for example:
  //
  //   X = [?, ?, 2, 2], pads = [1, 1, 1, 1], kernel_shape = [4, 4], strides = [1, 1]
  //
  // there will be only 1 calibration scalar and its value is: (16/4) = 4
  //
  for (Tensor::Dimension kernelTop = 0; (kernelTop + kernelHeight) <= inputHeightIncludePadding;
       kernelTop += verticalStride) {
    for (Tensor::Dimension kernelLeft = 0; (kernelLeft + kernelWidth) <= inputWidthIncludePadding;
         kernelLeft += horizontalStride) {
      unsigned numCountedElements = 0;
      for (Tensor::Dimension y = 0; y < kernelHeight; ++y) {
        for (Tensor::Dimension x = 0; x < kernelWidth; ++x) {
          const include_flags_type::size_type offset = (kernelTop + y) * inputWidthIncludePadding + (kernelLeft + x);
          assert(offset < includeFlags.size());

          if (includeFlags[offset]) {
            ++numCountedElements;
          }
        }
      }
      assert(numCountedElements <= kernelHeight * kernelWidth);

      calibrationScalars.emplace_back(static_cast<calibration_scalar_type>(kernelHeight * kernelWidth) /
                                      numCountedElements);
    }
  }
  assert(calibrationScalars.size() == outputHeight * outputWidth);

  auto tensor = std::unique_ptr<FloatTensor>(dynamic_cast<FloatTensor*>(averagePool.getY()->create()));
  tensor->setName(averagePool.getY()->getName() + "(average-pool-calibration-scalar)");

  const Tensor::Dimension outputChannel = averagePool.getY()->dimension(1);
  tensor->setDimensions({1, outputChannel, outputHeight, outputWidth});
  tensor->getValues().reserve(outputChannel * outputHeight * outputWidth);

  using std::begin;
  using std::end;
  for (unsigned count = outputChannel; 0 < count; --count) {
    std::copy(begin(calibrationScalars), end(calibrationScalars), std::back_inserter(tensor->getValues()));
  }
  assert(tensor->getValues().size() == outputChannel * outputHeight * outputWidth);

  return tensor;
}

// calibrate AveragePool's result if necessary
//
// group before calibration:
//
//   AveragePool -> (wrong result)
//
// graph after calibration:
//
//   AveragePool         -> Mul -> (calibrated result)
//   (calibraton scalar) ->
//
void calibrateResult(ComputeGraph& graph, AveragePool& averagePool)
{
  // no need to calibrate result if there are no paddings
  const IntsAttr::VectorType& pads = averagePool.getPads().vector();
  assert(pads.size() == 4);

  using std::begin;
  using std::end;
  if (std::count(begin(pads), end(pads), 0) == pads.size()) {
    averagePool.setCountIncludePad(IntAttr(1));
    return;
  }

  Tensor* const output = averagePool.getY();

  Tensor* const      scalar = graph.addValue(getCalibrationScalar(averagePool).release());
  Initializer* const init   = graph.addOperator<Initializer>();
  init->addOutput(*scalar);

  Tensor* const calibrated = output->create();
  calibrated->setName(output->getName() + "(average-pool-calibrated-output)");
  calibrated->setDimensions(output->getDimensions());
  graph.addValue(calibrated);

  output->replaceAllUsesWith(*calibrated);
  output->getUses().clear();

  Mul* const multiply = graph.addOperator<Mul>();
  multiply->addInput(*output);
  multiply->addInput(*scalar);
  multiply->addOutput(*calibrated);
}

} // namespace internal

Pass::ReturnType NvDlaCalibrateAveragePoolResultPass::runOnModule(Module& module)
{
  Pass::ReturnType    ret = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = module.cgEnd();
  for (cg = module.cgBegin(); cg != cgEnd; ++cg)
    ret |= runOnComputeGraph(*cg->value());

  return ret;
}

Pass::ReturnType NvDlaCalibrateAveragePoolResultPass::runOnComputeGraph(ComputeGraph& graph)
{
  using namespace internal;

  std::vector<AveragePool*> averagePools;
  for (ComputeOperator& computeOperator : graph) {
    if (AveragePool* averagePool = dyn_cast<AveragePool>(&computeOperator)) {
      if (shouldCalibrateResult(*averagePool)) {
        averagePools.emplace_back(averagePool);
      }

      averagePool->setCountIncludePad(IntAttr(1));
    }
  }

  if (averagePools.empty()) {
    return kModuleNoChanged;
  }

  for (AveragePool* averagePool : averagePools) {
    calibrateResult(graph, *averagePool);
  }

  graph.topologicalSort();

  return kModuleChanged;
}

} // namespace onnc
