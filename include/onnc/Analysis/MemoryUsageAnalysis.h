//===- MemoryUsageAnalysis.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MEMORY_USAGE_ANALYSIS_H
#define ONNC_MEMORY_USAGE_ANALYSIS_H
#include <onnc/Core/ModulePass.h>
#include <onnx/common/ir.h>
#include <iosfwd>
#include <unordered_map>

namespace onnc {

using TensorSize = std::vector<::onnx::Dimension>;
using TP_DataTy = ::onnx::TensorProto_DataType;

/** \class MemoryUsageAnalysis
 */
class MemoryUsageAnalysis : public ModulePass
{
public:
  static char ID;

  using ValueMemUsageMap = std::unordered_map<::onnx::Value*, size_t>;

public:
  MemoryUsageAnalysis();

  ReturnType runOnModule(Module& pModule) override;

  const ValueMemUsageMap& getMemUsages() const { return m_MemUsageMap; }

  void print(std::ostream& pOS) const;

private:
  void clear();

  void calculateMemoryUsages(::onnx::Graph &pGraph);

private:
  ValueMemUsageMap m_MemUsageMap;
};

MemoryUsageAnalysis* CreateMemoryUsageAnalysisPass();

} // namespace of onnc

#endif
