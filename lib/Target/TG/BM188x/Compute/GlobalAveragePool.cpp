//===- GlobalAveragePool.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GlobalAveragePool.h"
#include "PatternMatch.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Debug.h>
#define DEBUG_TYPE "tg_global_average_pool"

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;

//===----------------------------------------------------------------------===//
// GlobalAveragePool
//===----------------------------------------------------------------------===//
BM188X::GlobalAveragePool::GlobalAveragePool()
    : onnc::GlobalAveragePool(), m_EnableRelu(0),
      m_RShiftWidth(0), m_ThresholdXQuantized(0)
{
}

BM188X::GlobalAveragePool::~GlobalAveragePool()
{
}

void BM188X::GlobalAveragePool::init(const ::onnx::Node &pNode)
{
  if (pNode.hasAttribute(::onnx::Symbol("enable_relu")))
    m_EnableRelu = pNode.i(::onnx::Symbol("enable_relu"));
}

void BM188X::GlobalAveragePool::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::GlobalAveragePool::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::GlobalAveragePool::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}
