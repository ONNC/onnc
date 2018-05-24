//===- TargetMemInfo.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_MEMORY_INFO_H
#define ONNC_TARGET_TARGET_MEMORY_INFO_H
#include <onnx/common/ir.h>
#include <onnc/IR/Quadruple.h>

namespace onnc {

using TP_DataTy = onnx::TensorProto_DataType;

/** \struct MemSize
 *
 */
struct MemSize
{
  unsigned alignment;
  unsigned size;

  MemSize(unsigned alignment = 0, unsigned size = 0)
    : alignment(alignment), size(size) {}
};

/** \class TargetMemInfo
 *  \brief TargetMemInfo provides interfaces to describe memory hierarchy.
 *
 */
class TargetMemInfo
{
public:
  virtual unsigned getGlobalMemSize() const { return 0; }

  virtual unsigned getLocalMemSize() const { return 0; }

  virtual unsigned getElemSize(TP_DataTy pTy) const { return 0; }

  virtual unsigned getAlignment(TP_DataTy pTy) const { return 0; }

  /// Return actual memory size and alignment requirement of onnx::Value.
  virtual MemSize getValueMemorySize(onnx::Value *pValue)
  {
    return MemSize(0, 0);
  }
};

} // namespace of onnc

#endif
