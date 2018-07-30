//===- ${OperatorName}.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/${OperatorName}.h>

using namespace onnc;

char ${OperatorName}::ID = 0;

//===----------------------------------------------------------------------===//
// ${OperatorName}
//===----------------------------------------------------------------------===//
${OperatorName}::${OperatorName}(${ConstructorRequireAttributes})
  : ComputeOperator("${OperatorName}", ID)${CallAttributesConstructor} {
}

${ConstructorByAttributesImplementation}

${OperatorName}::${OperatorName}(const ${OperatorName}& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */${CallAttributesCopyConstructor} {
}

void ${OperatorName}::print(std::ostream& pOS) const
{
  pOS << name()${PrintAttributes};
}

bool ${OperatorName}::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
