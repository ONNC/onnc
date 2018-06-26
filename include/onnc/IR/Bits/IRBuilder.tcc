//===- IRBuilder.tcc ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
template<typename OpType, typename ... CtorParams>
OpType* IRBuilder::AddComputeOp(CtorParams&& ... pParams)
{
  if (!hasComputeGraph())
    return nullptr;

  OpType* op = getComputeGraph()->addOperator<OpType>(pParams...);
  RegisterComputeOperator(op);
  m_pTargetCNode = op;
  return op;
}

template<typename OpType, typename ... CtorParams>
OpType* IRBuilder::AddComputeOp(const ::onnx::Node& pNode, CtorParams&& ... pParams)
{
  OpType* op = AddComputeOp<OpType>(pParams...);
  op->connect(pNode);
  return op;
}

template<typename OpndType, typename OpType, typename ... CtorParams> OpndType*
IRBuilder::AddComputeOpnd(OpType& pFrom, OpType& pTo, CtorParams&& ... pParams)
{
  if (!hasComputeGraph())
    return nullptr;

  OpndType* opnd = getComputeGraph()->addOperand<OpndType>(pFrom, pTo, pParams...);
  return opnd;
}

template<typename OpType>
OpType* IRBuilder::CloneComputeOp(const OpType& pOp)
{
  OpType* result = new OpType(pOp);
  RegisterComputeOperator(result);
  m_pTargetCNode = result;
  return result;
}
