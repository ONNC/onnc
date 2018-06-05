//===- SplitNode.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/SplitNode.h>
#include <onnc/IR/OnnxUtils.h>
#include <onnc/Support/IOStream.h>
#include <limits>
#include <iomanip> // for setw

using namespace onnc;

static const onnx::NodeKind g_LoadKind = onnx::Symbol("Load");
static const onnx::NodeKind g_StoreKind = onnx::Symbol("Store");

static LongInts GetOutputValueSizes(const onnx::Node& pN)
{
  if (pN.outputs().size())
    return GetValueSizes(*pN.outputs()[0]);
  return {};
}

//===----------------------------------------------------------------------===//
// SplitNode
//===----------------------------------------------------------------------===//
SplitNode::SplitNode(onnx::Node& pN)
  : m_OutSizes(GetOutputValueSizes(pN)), m_Node(pN) {
  m_NewOutSizes = m_OutSizes;
}

bool SplitNode::useNewOutSize(const LongInts& pNewOutSize)
{
  m_NewOutSizes = pNewOutSize;
  return true;
}

LongInts SplitNode::calNewInputSize(unsigned pIdx) const
{
  return m_NewOutSizes;
}

LongInts SplitNode::getNewOutputSize(unsigned pIdx) const
{
  return m_NewOutSizes;
}

/// Factory
static SplitNode* SplitNodeCreator(onnx::Node& pN);

//===----------------------------------------------------------------------===//
// SplitNodeManager
//===----------------------------------------------------------------------===//
void SplitGroup::resetToOrigSize()
{
  for (unsigned i = 0; i < m_Stores.size(); ++i) {
    m_CurSplitAxis[i] = 0;
    m_CurSplitFactor[i] = 1;
  }

  std::vector<SplitNode *> worklist(m_Stores);
  while (!worklist.empty()) {
    SplitNode *sn = worklist.back();
    worklist.pop_back();

    sn->resetSize();
    // reset upper layer.
    for (onnx::Value *v : sn->getNode().inputs()) {
      onnx::Node *n = v->node();
      if (m_SnMgr.hasSplitNode(n) || n->kind() == g_LoadKind)
        continue;

      worklist.push_back(m_SnMgr.getSplitNode(n));
    }
  }
}

void SplitGroup::getMemUsage(const TargetTransformInfo *pTTI,
                             ValMemSizeMap &pVMSMap)
{
  std::vector<SplitNode *> worklist(m_Stores);
  while (!worklist.empty()) {
    SplitNode *sn = worklist.back();
    worklist.pop_back();

    onnx::Node &n = sn->getNode();

    // get required memory size of each input.
    for (unsigned i = 0; i < n.inputs().size(); ++i) {
      onnx::Value *v = n.inputs()[i];
      pVMSMap[v] = pTTI->getOperatorInputMemUsage(&n, i,
                                                  sn->calNewInputSize(i));
    }

    // get required memory size of each output.
    for (unsigned i = 0; i < n.outputs().size(); ++i) {
      onnx::Value *v = n.outputs()[i];
      pVMSMap[v] = pTTI->getOperatorOutputMemUsage(&n, i,
                                                   sn->calNewInputSize(i));
    }

    // add upper layer to worklist until load ir.
    for (onnx::Value *v : n.inputs()) {
      onnx::Node *upperN = v->node();
      if (m_SnMgr.hasSplitNode(upperN) || upperN->kind() == g_LoadKind)
        continue;

      worklist.push_back(m_SnMgr.getSplitNode(upperN));
    }
  }
}

void SplitGroup::shrinkSize()
{
  // Shrink size, and propogate backward.
  for (unsigned i = 0; i < m_Stores.size(); ++i) {
    SplitNode *sn = m_Stores[i];

    const TensorSizes &origSizes = sn->getNode().inputs()[0]->sizes();
    unsigned &factor = m_CurSplitFactor[i],
             &splitAxis = m_CurSplitAxis[i];
    ++factor;
    // Can't divide this axis further, try next axis.
    if (origSizes[splitAxis].dim < factor) {
      ++splitAxis;
      factor = 1;
    }

    // Unable divide further.
    if (origSizes.size() == splitAxis)
      continue;

    m_SnMgr.splitNodeByFactor(&sn->getNode(), splitAxis, factor, true);
  }
}

void SplitGroup::addStore(SplitNode *pStore)
{
  m_Stores.push_back(pStore);
  m_CurSplitAxis.push_back(0);
  m_CurSplitFactor.push_back(1);
}

SplitGroup *SplitGroup::splitNewGroup()
{
  // choose a good split point.
  SplitGroup *newgroup = nullptr;
  return newgroup;
}

//===----------------------------------------------------------------------===//
// SplitNodeManager
//===----------------------------------------------------------------------===//
SplitNodeManager::SplitNodeManager(onnx::Graph& pGraph,
                                   DLATargetBackend& pDLATB)
  : m_DLATB(pDLATB), m_Graph(pGraph)
{
  m_Groups.push_back(new SplitGroup(*this));

  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    SplitNode *sn = SplitNodeCreator(*n);
    m_SplitInfos[n] = sn;

    if (n->kind() == g_StoreKind)
      m_Groups[0]->addStore(sn);
  }
}

SplitNodeManager::~SplitNodeManager()
{
  clear();
}

void SplitNodeManager::clear()
{
  for (auto snIt : m_SplitInfos)
    delete snIt.second;
  m_SplitInfos.clear();

  for (SplitGroup *sg : m_Groups)
    delete sg;
  m_Groups.clear();
}

SplitNode* SplitNodeManager::getSplitNode(onnx::Node* pN)
{
  assert(m_SplitInfos.find(pN) != m_SplitInfos.end() &&
         "onnx::Node doesn't exist in SplitNodeManager.");
  return m_SplitInfos[pN];
}

const SplitNode* SplitNodeManager::getSplitNode(onnx::Node* pN) const
{
  auto it = m_SplitInfos.find(pN);
  assert(m_SplitInfos.find(pN) != m_SplitInfos.end() &&
         "onnx::Node doesn't exist in SplitNodeManager.");
  return it->second;
}

void SplitNodeManager::splitNodeByFactor(onnx::Node* pN, unsigned pAxis,
                                         unsigned pFactor, bool pUpdateUpper)
{
  SplitNode* ns = getSplitNode(pN);
  LongInts newS = ns->m_NewOutSizes;
  newS[pAxis] = (newS[pAxis] + pFactor - 1)/ pFactor;
  splitNodeBySize(pN, newS, pUpdateUpper);
}

bool SplitNodeManager::hasSplitNode(onnx::Node *pN) const
{
  return m_SplitInfos.find(pN) != m_SplitInfos.end();
}

bool SplitNodeManager::splitNodeBySize(onnx::Node* pN,
                                       const LongInts& pNewOutSize,
                                       bool pUpdateUpper)
{
  SplitNode* ns = getSplitNode(pN);
  if (!ns->useNewOutSize(pNewOutSize))
    return false;

  // Load IR is a boundary, it is paired with Store IR and form a subgraph.
  if (!pUpdateUpper || pN->kind() == g_LoadKind)
    return true;

  bool status = true;
  for (unsigned i = 0; i < ns->m_Node.inputs().size(); ++i) {
    if (onnx::Node* child = ns->m_Node.inputs()[i]->node()) {
      if (child->kind() == onnx::kParam)
        continue;
      LongInts newInS = ns->calNewInputSize(i);
      status &= splitNodeBySize(child, newInS, true);
    }
  }
  return status;
}

void SplitNodeManager::dump() const
{
  print(outs());
}

void PrintAttr(OStream &pOS, const onnx::Node &pN)
{
  for (auto attrId : pN.attributeNames()) {
    pOS << attrId.toString() << ": ";
    switch (pN.kindOf(attrId)) {
    case onnx::AttributeKind::f   :
      pOS << pN.f(attrId) << " ";
      break;
    case onnx::AttributeKind::fs  :
      for (float f : pN.fs(attrId))
        pOS << f << " ";
      break;
    case onnx::AttributeKind::i   :
      pOS << pN.i(attrId) << " ";
      break;
    case onnx::AttributeKind::is  :
      for (int i : pN.is(attrId))
        pOS << i << " ";
      break;
    case onnx::AttributeKind::s   :
      pOS << pN.s(attrId) << " ";
      break;
    case onnx::AttributeKind::ss  :
      for (auto &s : pN.ss(attrId))
        pOS << s << " ";
      break;
    default :
      pOS << "[TODO]";
    }
    pOS << " ";
  }
}

void SplitNodeManager::print(OStream &pOS) const
{
  size_t graphOldS = 0, graphNewS = 0;
  for (const onnx::Node *n : m_Graph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    if (n->kind() == g_LoadKind ||
        n->kind() == g_StoreKind)
      continue;

    std::vector<LongInts> newInputSizes;
    const SplitNode *sn = getSplitNode((onnx::Node*)n);
    pOS << n->kind().toString() << ": ";
    PrintAttr(pOS, *n);
    pOS << "\n";

    pOS << "  inputs:\n";
    for (int i = 0; i < n->inputs().size(); ++i) {
      LongInts newInS = sn->calNewInputSize(i);
      newInputSizes.push_back(newInS);

      const onnx::Value *v = n->inputs()[i];

      pOS << "    " << std::setw(12) << std::left << v->uniqueName() << "(";
      for (auto d : v->sizes())
        pOS << std::setw(5) << std::right << d.dim;

      pOS << ") -> (";
      for (int64_t s : newInS)
        pOS << std::setw(5) << std::right << s;
      pOS << ")\n";
    }

    pOS << "  outputs:\n";
    for (int i = 0; i < n->outputs().size(); ++i) {
      const onnx::Value *v = n->outputs()[i];

      pOS << "    " << std::setw(12) << std::left << v->uniqueName() << "(";
      for (auto s : v->sizes())
        pOS << std::setw(5) << std::right << s.dim;

      pOS << ") -> (";
      for (int64_t s : sn->m_NewOutSizes)
        pOS << std::setw(5) << std::right << s;
      pOS << ")\n";
    }

    MemSize newS =
      m_DLATB.getTTI()->getOperatorMemUsage(n, newInputSizes,
                                            {sn->m_NewOutSizes});
    MemSize oldS = m_DLATB.getTTI()->getOperatorMemUsage(n);

    graphOldS += oldS.size;
    graphNewS += newS.size;
    pOS << "  total: " << oldS.size/1024.f << " kb -> "
        << newS.size/1024.f << " kb" << "\n";
  }
  pOS << "Graph total size: " << (float)graphOldS/1024.f << " kb -> "
      << (float)graphNewS/1024.f << " kb" << "\n";
}

//===----------------------------------------------------------------------===//
// Derived of SplitNode
//===----------------------------------------------------------------------===//
class SplitConv : public SplitNode {
public:
  SplitConv(onnx::Node& pN)
    : SplitNode(pN) {
    assert(pN.kind() == onnx::kConv && "This is not a convolution node.");

    GetConvKernelShape(pN, m_KShape);
    GetAttrVals(pN, onnx::kstrides, m_Stride);
    GetPads(pN, m_PadBegin, m_PadEnd);
  }

  LongInts calNewInputSize(unsigned pIdx) const override
  {
    // Conv inputs:
    //  0   x:T   (N x C x D1 x D2 .. Dn)
    //  1   w:T   (M x C x k1 x k2 .. kn)
    //  2   B:T   (M)
    //
    //  kernel_shape  [k1 x k2 .. kn]
    //  pads          [x1_begin, x2_begin .. x1_end, x2_end]
    //  strides       [s1 x s2 .. sn]
    //
    // Conv output:
    //  0   y:T   (N x M x [(D1 - K1 + x1_begin + x1_end)/s1 + 1] x ..)
    //
    // Please also ref: UpdateGraphOutputSize.cpp:UpdateConvOutputInfo.
    switch (pIdx) {
    case 0: {
      LongInts newIS(4); // common case: N C H W
      const TensorSizes &xDim = m_Node.inputs()[0]->sizes();
      newIS[0] = m_NewOutSizes[0];
      newIS[1] = xDim[1].dim;
      const size_t numAxis = xDim.size() - 2;
      for (int i = 0; i < numAxis; ++i) {
        newIS[i + 2] = (m_NewOutSizes[i + 2] - 1) * m_Stride[i]
                       - m_PadBegin[i] - m_PadEnd[i] + m_KShape[i];
      }
      return newIS;
    }
    case 1: {
      const TensorSizes &wDim = m_Node.inputs()[1]->sizes();
      LongInts newIS(wDim.size());
      newIS[0] = m_NewOutSizes[1];
      for (int i = 1; i < wDim.size(); ++i)
        newIS[i] = wDim[i].dim;
      return newIS;
    }
    case 2:
      return {m_NewOutSizes[1]};
    default:
      assert(false && "Error in SplitConv::calNewInputSize. Invalid input id.");
      return {};
    }
  }

private:
  LongInts m_PadBegin, m_PadEnd;
  LongInts m_KShape;
  LongInts m_Stride;
};

class SplitGemm : public SplitNode {
public:
  SplitGemm(onnx::Node& pN)
    : SplitNode(pN) {
  }

  LongInts calNewInputSize(unsigned pIdx) const override
  {
    // Gemm inputs:
    //  0   A:T   (M x K)
    //  1   B:T   (K x N)
    //  2   C:T   (M x N)
    //
    //  broadcast  [int]
    //  transA     [int]
    //  transB     [int]
    //
    // Gemm output:
    //  0   Y:T   (M x N)
    const TensorSizes &aDim = m_Node.inputs()[0]->sizes();
    const int64_t K = IsTranspose(m_Node, onnx::ktransA) ?
                        aDim[0].dim : aDim[1].dim;

    switch (pIdx) {
    case 0: {
      // Get new size of A.
      if (IsTranspose(m_Node, onnx::ktransA))
        return {K, m_NewOutSizes[0]};
      return {m_NewOutSizes[0], K};
    }
    case 1: {
      // Get new size of B.
      if (IsTranspose(m_Node, onnx::ktransB))
        return {m_NewOutSizes[1], K};
      return {K, m_NewOutSizes[1]};
    }
    case 2: {
      // [FIXME] We use original value. Should we change this?
      return GetValueSizes(*m_Node.inputs()[2]);
    }
    default:
      assert(false && "Error in SplitGemm::calNewInputSize. Invalid input id.");
      return {};
    }
  }
};

class SplitPool : public SplitNode {
public:
  SplitPool(onnx::Node& pN)
    : SplitNode(pN) {
    assert(pN.kind() == onnx::Symbol("MaxPool") && "This is not a pool node.");

    GetAttrVals(pN, onnx::kkernel_shape, m_KShape);
    GetAttrVals(pN, onnx::kstrides, m_Stride);
    GetPads(pN, m_PadBegin, m_PadEnd);
  }

  LongInts calNewInputSize(unsigned pIdx) const override
  {
    assert(pIdx == 0 && "SplitPool::calNewInputSize: Invalid input id.");

    LongInts newIS(4); // common case: N C H W
    const TensorSizes &xDim = m_Node.inputs()[0]->sizes();
    newIS[0] = m_NewOutSizes[0];
    newIS[1] = m_NewOutSizes[1];
    const size_t numAxis = xDim.size() - 2;
    for (int i = 0; i < numAxis; ++i) {
      newIS[i + 2] = m_NewOutSizes[i + 2] * m_Stride[i]
                     - m_PadBegin[i] - m_PadEnd[i] + 2 * (m_KShape[i] / 2);
    }
    return newIS;
  }

private:
  LongInts m_PadBegin, m_PadEnd;
  LongInts m_KShape;
  LongInts m_Stride;
};

class SplitReshape : public SplitNode {
public:
  SplitReshape(onnx::Node& pN)
    : SplitNode(pN) {
  }

  LongInts calNewInputSize(unsigned pIdx) const override
  {
    assert(pIdx <= 1 && "SplitReshape::calNewInputSize: Invalid input id.");

    if (pIdx == 1)
      return {};

    // [FIXME] It is hard to compute new input size for reshape. Here has some
    //         assumptions and is buggy.
    // [TODO] If there is un-handled case, just give up.
    //
    // Assume we are handling this kind of case:
    // Reshape
    //   pool5_1     : size = 4 (   10  256    6    6)
    //   OC2_DUMMY_1 : size = 1 (    2)
    //   OC2_DUMMY_0 : size = 2 (   10 9216)
    // Gemm
    //   OC2_DUMMY_0 : size = 2 (   10 9216)
    //   fc6_w_0     : size = 2 ( 4096 9216)
    //   fc6_b_0     : size = 1 ( 4096)
    //   fc6_1       : size = 2 (   10 4096)
    assert(m_OutSizes.size() == 2 && "Reshape size assumption.");

    onnx::Value *v = m_Node.inputs()[0];
    const TensorSizes &origSizes = v->sizes();
    LongInts newIS(origSizes.size());

    newIS[0] = m_NewOutSizes[0];

    int64_t origCHWSize = 1, newCHWSize = 1;
    for (int i = 1; i < m_NewOutSizes.size(); ++i) {
      origCHWSize *= m_NewOutSizes[i];
      newCHWSize *= m_OutSizes[i];
    }

    assert(origCHWSize >= newCHWSize && "SplitReshape: Invalid resize.");

    if (origCHWSize % newCHWSize != 0) {
      errs() << "SplitReshape: origCHWSize mod newCHWSize is not zero!\n"
             << "  " << origCHWSize << " % " << newCHWSize << "\n";
    }

    int64_t resizeFactor = origCHWSize / newCHWSize;

    if (origSizes[1].dim % resizeFactor != 0) {
      errs() << "SplitReshape: origSizes[1].dim mod resizeFactor is not zero!\n"
             << "  " << origSizes[1].dim << " % " << resizeFactor << "\n";
    }

    newIS[1] = origSizes[1].dim / resizeFactor;
    newIS[2] = origSizes[2].dim;
    newIS[3] = origSizes[3].dim;
    return newIS;
  }
};

static SplitNode* SplitNodeCreator(onnx::Node& pN)
{
  if (OutputSizeIsInputSize(pN) ||
      pN.kind() == g_LoadKind ||
      pN.kind() == g_StoreKind)
    return new SplitNode(pN);

  const auto kind = pN.kind();
  if (kind == onnx::kConv) {
    return new SplitConv(pN);
  } else if (kind == onnx::Symbol("MaxPool")) {
    return new SplitPool(pN);
  } else if (kind == onnx::kGemm) {
    return new SplitGemm(pN);
  } else if (kind == ::onnx::kReshape) {
    return new SplitReshape(pN);
  }

  errs() << "Unsupport node: " << kind.toString() << "\n";
  assert(false && "Unsupport node.");
  return nullptr;
}
