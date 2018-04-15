#pragma once

#include <onnx/common/ir.h>
#include <memory>
#include "TGBackend.h"
#include "TargetLowering.h"

namespace onnc {
class TGBackend;
using MemTable = std::map<std::string, uint64_t>;

class TGTargetLowering : public TargetLowering {

public:
  TGTargetLowering(TGBackend *tgBackend)
      : TargetLowering(), m_tgBackend(tgBackend) {}
  void
  LowerOperation(const onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) override;

private:
  TGBackend *m_tgBackend;
  Operator *LowerHelper(const onnx::Node &node, MemTable &memTabl);
  void ddrScanAndAlloc(MemTable &memTable, onnx::Graph &graph);
};
} // namespace onnc
