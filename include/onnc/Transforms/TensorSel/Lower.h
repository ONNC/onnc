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
#include <onnc/IR/ComputeGraph.h>
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

  enum Score : int {
    kUndefined = -1,
    kNotMe = 0,
    kStdLower = 10
  };

public:
  virtual ~Lower() = 0;

  int isMe(const ::onnx::Node& pNode) const { return m_MatchFn(pNode); }

  void setQualityMatchFn(QualityMatchFnTy pFn) { m_MatchFn = pFn; }

  virtual ComputeOperator* lower(InsertionPoint& pISP, ::onnx::Node& pNode) const = 0;

  std::string name() { return m_Name; }

  const std::string& name() const { return m_Name; }

  void setName(const std::string& pName) { m_Name = pName; }

protected:
  std::string m_Name;
  QualityMatchFnTy m_MatchFn;
};

}

#endif
