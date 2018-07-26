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
    kStdLower = 10,
    kTargetLow = 20,
    kTargetNormal = 30,
    kTargetHigh = 40
  };

public:
  virtual ~Lower() = 0;

  /// If a backend doesn't want to use single quailty-match function, then
  /// we shall allow them to override this function.
  virtual int isMe(const ::onnx::Node& pNode) const { return m_MatchFn(pNode); }

  /// In case a backend defines a single quality-match function.
  void setQualityMatchFn(QualityMatchFnTy pFn) { m_MatchFn = pFn; }

  /// Abstract interface for lowering process.
  /// Create corresponding ComputeOperator according to @ref pNode and insert it
  /// in @ref pCG.
  ///
  /// @return The created ComputeOperator.
  /// @retval nullptr failed
  virtual ComputeOperator* activate(ComputeGraph& pCG, ::onnx::Node& pNode) const = 0;

  std::string name() { return m_Name; }

  const std::string& name() const { return m_Name; }

  void setName(const std::string& pName) { m_Name = pName; }

protected:
  std::string m_Name;
  QualityMatchFnTy m_MatchFn;
};

} // namespace of onnc

#endif
