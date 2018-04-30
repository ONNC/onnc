//===- MemoryAllocation.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MEMORY_ALLOCATION_H
#define ONNC_MEMORY_ALLOCATION_H
#include <onnc/Core/ModulePass.h>
#include <onnx/common/ir.h>

namespace onnc {

/** \class MemoryAllocation
 *  Perform memory allocation and generate allocation map.
 */
class MemoryAllocation : public ModulePass
{
public:
  static char ID;

public:
  MemoryAllocation();

  bool runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;
};

MemoryAllocation* CreateMemoryAllocationPass();

} // namespace of onnc

#endif
