//===- TargetMemInfo.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_MEMORY_INFO_H
#define ONNC_TARGET_TARGET_MEMORY_INFO_H
#include <onnc/IR/Quadruple.h>
#include <onnc/Config/ONNX.h>

namespace onnc {

typedef xTensorProtoDataType TP_DataTy;

/** \struct MemSize
 *
 */
struct MemSize
{
  uint64_t alignment;
  uint64_t size;

  MemSize(uint64_t alignment = 0, uint64_t size = 0)
    : alignment(alignment), size(size) {
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

  virtual uint64_t getElemSize(xTensorProtoDataType pTy) const {
    return 0;
  }

  virtual uint64_t getAlignment(TP_DataTy pTy) const { return 0; }

  /// Return actual memory size and alignment requirement of onnx::Value.
  virtual MemSize getValueMemorySize(xValue *pValue) {
    return MemSize();
  }
};

} // namespace onnc

#endif
