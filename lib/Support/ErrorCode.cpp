//===- ErrorCode.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/ErrorCode.h>
#include <cstring>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SystemError
//===----------------------------------------------------------------------===//
const char* SystemError::msg() const
{
  if (kNotStartedYet == m_Code)
    return "action not started yet";
  else if (kAddrNotParsable == m_Code)
    return "cannot parse the address string";
  else if (kSuccess == m_Code)
    return "Execution successfully";
  else
    return strerror(m_Code);
}

void SystemError::print(std::ostream& pOS) const
{
  pOS << msg();
}
