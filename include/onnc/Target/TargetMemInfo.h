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

namespace onnc {

/** \class TargetMemInfo
 *  \brief TargetMemInfo provides interfaces to describe memory hierarchy.
 *
 */
class TargetMemInfo
{
public:
  virtual unsigned getGlobalMemSize() const { return 0; }

  virtual unsigned getLocalMemSize() const { return 0; }
};

} // namespace of onnc

#endif
