//===- TransformUtils.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_UTILS_H
#define ONNC_TRANSFORM_UTILS_H
#include <onnc/Config/ONNX.h>

#include <memory>

namespace onnc {

inline void dropOutputs(xNode& node)
{
  while (0 < node.outputs().size()) {
    const auto lastIndex = node.outputs().size() - 1;
    auto lastValue = std::unique_ptr<xValue>{node.outputs()[lastIndex]};
    node.eraseOutput(lastIndex);
  }
}

} // namespace onnc

#endif
