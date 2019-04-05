//===- OperatorCodeGen.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CLANG_OPERATOR_GENERATOR_H
#define TARGET_CLANG_OPERATOR_GENERATOR_H

#include <string>

namespace onnc {

class OperatorCodeGen
{
public:
  std::string generate(std::string OperatorName);
};

} // namespace onnc

#endif
