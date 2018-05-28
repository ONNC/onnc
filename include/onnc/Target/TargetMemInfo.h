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

/** \struct MemSize
 *
 */
struct MemSize
{
  uint64_t alignment;
  uint64_t size;

  MemSize(uint64_t alignment, uint64_t size) : alignment(alignment), size(size)
  {
  }
};

/** \class TargetMemInfo
 *  \brief TargetMemInfo provides interfaces to describe memory hierarchy.
 *
 */
class TargetMemInfo
{
public:
  virtual uint64_t getGlobalMemSize() const { return 0; }

  virtual uint64_t getLocalMemSize() const { return 0; }

  virtual uint64_t getElemSize(onnx::TensorProto_DataType pTy) const
  {
    return 0;
  }

  /// Return actual memory size and alignment requirement of onnx::Value.
  virtual MemSize getValueMemorySize(onnx::Value *pValue)
  {
    return MemSize(0, 0);
  }
};

} // namespace of onnc

#endif
