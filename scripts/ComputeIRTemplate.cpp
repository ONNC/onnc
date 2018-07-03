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

${OperatorName}::${OperatorName}(const ${OperatorName}& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */${CallAttributesCopyConstructor} {
}

void ${OperatorName}::print(std::ostream& pOS) const
{
  pOS << name()${PrintAttributes};
}
