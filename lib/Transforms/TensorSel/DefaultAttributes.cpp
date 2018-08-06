//===- DefaultAttributes.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "DefaultAttributes.h"
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/LpPool.h>
#include <onnc/IR/Compute/Transpose.h>

using namespace onnc;

namespace {

class DefaultAttributes : public ComputeVisitor {
public:
  DefaultAttributes(xNode &pNode) : m_Node(pNode) {}

  virtual void visit(Conv &pConv) override {
    this->setDilations(pConv);
    this->setKernelShape(pConv);
    this->setPads(pConv);
    this->setStrides(pConv);
  }

  virtual void visit(MaxPool &pMaxPool) override {
    this->setPads(pMaxPool);
    this->setStrides(pMaxPool);
  }

  virtual void visit(LpPool &pLpPool) override {
    this->setPads(pLpPool);
    this->setStrides(pLpPool);
  }

  virtual void visit(AveragePool &pAveragePool) override {
    this->setPads(pAveragePool);
    this->setStrides(pAveragePool);
  }

  virtual void visit(Transpose &pTranspose) override {
    if (m_Node.hasAttribute(xSymbol("perm"))) {
      pTranspose.setPerm(m_Node.is(xSymbol("perm")));
    } else {
      xValue &input = *m_Node.inputs()[0];
      std::vector<int64_t> v(input.sizes().size());
      for (int i = 0; i < v.size(); ++i) {
        v[i] = v.size() - i - 1;
      }
      pTranspose.setPerm(std::move(v));
    }
  }

private:
  template<typename T>
  void setDilations(T &pOp) {
    if (m_Node.hasAttribute(xSymbol("dilations"))) {
      pOp.setDilations(m_Node.is(xSymbol("dilations")));
    } else {
      xValue &input = *m_Node.inputs()[0];
      std::vector<int64_t> v(input.sizes().size() - 2, 1);
      pOp.setDilations(std::move(v));
    }
  }

  template<typename T>
  void setKernelShape(T &pOp) {
    if (m_Node.hasAttribute(xSymbol("kernel_shape"))) {
      pOp.setKernelShape(m_Node.is(xSymbol("kernel_shape")));
    } else {
      xValue &weight = *m_Node.inputs()[1];
      std::vector<int64_t> v(weight.sizes().size() - 2);
      for (int i = 0; i < v.size(); ++i) {
        v[i] = weight.sizes()[i + 2].dim;
      }
      pOp.setKernelShape(std::move(v));
    }
  }

  template<typename T>
  void setPads(T &pOp) {
    if (m_Node.hasAttribute(xSymbol("pads"))) {
      pOp.setPads(m_Node.is(xSymbol("pads")));
    } else {
      xValue &input = *m_Node.inputs()[0];
      std::vector<int64_t> v((input.sizes().size() - 2) * 2, 0);
      pOp.setPads(std::move(v));
    }
  }

  template<typename T>
  void setStrides(T &pOp) {
    if (m_Node.hasAttribute(xSymbol("strides"))) {
      pOp.setStrides(m_Node.is(xSymbol("strides")));
    } else {
      xValue &input = *m_Node.inputs()[0];
      std::vector<int64_t> v(input.sizes().size() - 2, 1);
      pOp.setStrides(std::move(v));
    }
  }

private:
  xNode &m_Node;
};

} // anonymous namespace

namespace onnc {

void SetDefaultAttributes(xNode &pNode, ComputeOperator& pOp) {
  DefaultAttributes da(pNode);
  pOp.accept(da);
}

}
