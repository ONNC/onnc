//===- NvDlaComputeVisitor.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_COMPUTE_VISITOR_H
#define TARGET_NVDLA_NVDLA_COMPUTE_VISITOR_H

#include <onnc/IR/CustomVisitor.h>

namespace onnc {

class NvDlaConvRelu;

class NvDlaComputeVisitor : public onnc::CustomVisitor<NvDlaComputeVisitor>
{
public:
  virtual void visit(const NvDlaConvRelu&) {}
};

} // namespace onnc

#endif
