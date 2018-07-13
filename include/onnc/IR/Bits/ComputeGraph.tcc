//===- ComputeGraph.tcc ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
template<typename OpType, typename ... NodeCtorParams>
OpType* ComputeGraph::addOperator(NodeCtorParams&& ... pParams)
{
  // 1. find an available free node
  OpType* result = new OpType(pParams...);
  this->addOperator(*result);
  return result;
}

template<typename OpType>
ComputeGraph& ComputeGraph::addOperator(OpType& pOperator)
{
  // 1. find an available free node
  OpType* result = &pOperator;
  m_NodeList.insert(result);

  // 2. set up linkages
  result->prev = m_pNodeRear;
  result->next = nullptr;
  result->first_in = nullptr;
  result->last_in = nullptr;
  result->first_out = nullptr;
  result->last_out = nullptr;

  // 3. reset rear node
  if (nullptr != m_pNodeRear) {
    m_pNodeRear->next = result;
  }
  m_pNodeRear = result;

  if (nullptr == m_pNodeHead)
    m_pNodeHead = result;

  return *this;
}

template<typename ValueType, typename ... ValueCtorParams>
ValueType* ComputeGraph::addValue(ValueCtorParams&& ... pParams)
{
  ValueType* result = new ValueType(pParams...);
  this->addValueToModule(result);
  return result;
}

template<typename ValueType>
ValueType* ComputeGraph::getValue(StringRef pName)
{
  ValueList::iterator entry = m_ValueList.find(pName);
  if (m_ValueList.end() == entry)
    return nullptr;

  return static_cast<ValueType*>(entry->value());
}

template<typename ValueType>
const ValueType* ComputeGraph::getValue(StringRef pName) const
{
  ValueList::iterator entry = m_ValueList.find(pName);
  if (m_ValueList.end() == entry)
    return nullptr;

  return static_cast<const ValueType*>(entry->value());
}

template<typename OpndType, typename ... ArcCtorParams>
OpndType* ComputeGraph::addOperand(Node& pU, Node& pV, ArcCtorParams&& ... pParams)
{
  // 1. create operand and insert into arc list
  OpndType* result = new OpndType(pParams...);
  m_ArcList.insert(result);

  // 2. set up arc
  result->source = &pU;
  result->target = &pV;
  result->prev_in = nullptr;
  result->next_in = nullptr;
  result->prev_out = nullptr;
  result->next_out = nullptr;

  // 3. set up fan-out linked list
  result->prev_out = pU.last_out;
  if (nullptr != pU.last_out) {
    pU.last_out->next_out = result;
  }
  else { // last_out is nullptr => a node without fan-out arcs.
    pU.first_out = result;
  }
  pU.last_out = result;

  // 4. set up fan-in linked list
  result->prev_in = pV.last_in;
  if (nullptr != pV.last_in) {
    pV.last_in->next_in = result;
  }
  else { // last_in is nullptr => a node without fan-in arcs
    pV.first_in = result;
  }
  pV.last_in = result;

  return result;
}
