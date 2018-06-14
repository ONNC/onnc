//===- ${OperatorName}.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/${OperatorName}.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ${OperatorName}
//===----------------------------------------------------------------------===//
${OperatorName}::${OperatorName}()
  : ComputeOperator("${OperatorName}")${CallAttributesDefaultConstructor} {
}

${ConstructorByAttributesImplementation}

void ${OperatorName}::print(std::ostream& pOS) const
{
}
