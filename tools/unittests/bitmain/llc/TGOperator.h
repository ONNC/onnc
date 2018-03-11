#pragma once

#include "onnx/common/ir.h"

using MemTable = std::map<std::string, uint64_t>;

class TGOperator {
public:
  static TGOperator *makeTGOperator(const onnx::Node &node, MemTable &memTable);
  TGOperator(const onnx::Node &node, const std::string &name);
  virtual ~TGOperator() {};
  const std::string &getName(void) { return m_name; }
  virtual void emit(void) const = 0;

private:
  std::string m_name;
};

#include "TGConv.h"
#include "TGRelu.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGGemm.h"
#include "TGSoftmax.h"
