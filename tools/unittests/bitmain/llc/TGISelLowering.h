#pragma once

#include "onnx/common/ir.h"
#include <memory>

using MemTable = std::map<std::string, uint64_t>;

class TargetLowering {

  public:
    TargetLowering(){};
    ~TargetLowering(){};
    void CodeGenAndEmitInst(onnx::Graph *graph,
                            std::vector<std::unique_ptr<Operator> > &instList);
    virtual void LowerOperation(const onnx::Node &node, std::vector<std::unique_ptr<Operator> > &instList) = 0;
};

class TGTargetLowering : public TargetLowering {

public:
  TGTargetLowering(onnx::Graph *onnxGraph);
  void
  LowerOperation(const onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) override;

  // FOR UNITTEST
  MemTable &getMemLayout(void) { return m_globalMemLayout; }

private:
  Operator *LowerHelper(const onnx::Node &node, MemTable &memTabl);
  void ddrScanAndAlloc(MemTable &memTable, onnx::Graph &graph);
  MemTable m_globalMemLayout;
};
