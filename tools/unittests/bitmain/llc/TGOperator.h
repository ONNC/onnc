#pragma once

#include "onnx/common/ir.h"

class TGOperator {
public:
  static TGOperator *makeTGOperator(const onnx::Node &node, uint64_t offset);
  TGOperator(const onnx::Node &node, const std::string &name);
  virtual ~TGOperator() {};
  uint64_t getTotalWeightSize(void) { return m_totalWeightSize; }
  const std::string &getName(void) { return m_name; }
  virtual void emit(void) const = 0;

protected:
  uint64_t updateWeightSize(const onnx::Node &node, uint64_t offset, std::vector<uint64_t> &weightOffset);
  uint64_t m_totalWeightSize;

private:
  std::string m_name;
};

#include "TGConv.h"
#include "TGRelu.h"
#include "TGLRN.h"

class TGMaxPool : public TGOperator {
public:
  TGMaxPool(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};

#include "TGGemm.h"

class TGSoftmax : public TGOperator {
public:
  TGSoftmax(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};
