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

#include "TGConv.hpp"

class TGRelu : public TGOperator {
public:
  TGRelu(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};

class TGLRN : public TGOperator {
public:
  TGLRN(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};

class TGMaxPool : public TGOperator {
public:
  TGMaxPool(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};

class TGGemm : public TGOperator {
public:
  TGGemm(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
private:
  std::vector<uint64_t> m_weightOffset;
};

class TGSoftmax : public TGOperator {
public:
  TGSoftmax(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
};
