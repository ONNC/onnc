//===- Profile.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Profile.h>

using namespace onnc;

char Profile::ID = 0;

//===----------------------------------------------------------------------===//
// Profile
//===----------------------------------------------------------------------===//
Profile::Profile()
  : ComputeOperator("Profile", ID) {
}

Profile::Profile(const Profile& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Profile::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Profile::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
