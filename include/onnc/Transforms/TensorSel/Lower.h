//===- Lower.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_LOWER_H
#define ONNC_TRANSFORM_LOWER_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Module.h>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

/** \class Lower
 *  \brief Lower provides abstract interface for lowering an ONNX node.
 */
class Lower
{
public:
  typedef int (*QualityMatchFnTy)(const ::onnx::Node&);

public:
  virtual ~Lower() = 0;

  int isMe(const ::onnx::Node& pNode) const { return m_MatchFn(pNode); }

  void setQualityMatchFn(QualityMatchFnTy pFn) { m_MatchFn = pFn; }

  virtual ComputeOperator* act(Module& pModule, ::onnx::Node& pNode) const = 0;

protected:
  std::string m_Name;
  QualityMatchFnTy m_MatchFn;
};

}

#endif
