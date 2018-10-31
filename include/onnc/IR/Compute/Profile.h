//===- Profile.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===--------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_PROFILE_H
#define ONNC_IR_COMPUTE_OPERATOR_PROFILE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Profile : public ComputeOperator
{
public:
  static char ID;

public:
  Profile();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Profile(const Profile &pCopy);

  ~Profile() { }

  // clang-format off
  // Attributes getters
  

  // Attributes setters
  

  // Inputs setters

  // Outputs setters

  // clang-format on

  void print(std::ostream& pOS) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  
  // clang-format on
};

} // namespace of onnc

#endif
