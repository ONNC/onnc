#pragma once

#include "BM1880Backend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnx/common/ir.h>

namespace onnc {

class BM1880Backend;

using MemTable = std::map<std::string, uint64_t>;

class BM1880ISelLowering : public TargetLowering
{
public:
  BM1880ISelLowering(BM1880Backend *pBackend) : TargetLowering(pBackend) {}

  void
  LowerOperation(const ::onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) override;

  void PrepareCodeGenAndEmitInst(Module &pModule) override;

private:
  void ddrScanAndAlloc(MemTable &memTable, ::onnx::Graph &graph);

private:
  Operator *LowerHelper(const ::onnx::Node &node, MemTable &memTabl);
};

} // namespace onnc
