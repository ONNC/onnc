//===- MemoryAllocation.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/MemoryAllocation.h>
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/IR/OnnxUtils.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetMemInfo.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <limits>
#include <vector>
#include <unordered_map>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Extension IR for onnx
//===----------------------------------------------------------------------===//
static const onnx::Symbol g_LoadKind("Load");
static const onnx::Symbol g_StoreKind("Store");

//===----------------------------------------------------------------------===//
// SplitNode
//===----------------------------------------------------------------------===//
class SplitNode
{
friend class SplitNodeManager;

public:
  SplitNode(onnx::Node& pN)
    : m_OutSizes(GetValueSizes(*pN.outputs()[0])), m_Node(pN) {
    m_NewOutSizes = m_OutSizes;
  }

  virtual ~SplitNode() {}

  virtual bool useNewOutSize(const LongInts& pNewOutSize);

  /// Calculate required input size based on new output size.
  /// pIdx: Which input.
  virtual LongInts calNewInputSize(unsigned pIdx) const;

protected:
  LongInts m_NewOutSizes;
  const LongInts m_OutSizes;

  onnx::Node& m_Node;
};

bool SplitNode::useNewOutSize(const LongInts& pNewOutSize)
{
  m_NewOutSizes = pNewOutSize;
  return true;
}

/// Calculate required input size based on new output size.
/// pIdx: Which input.
LongInts SplitNode::calNewInputSize(unsigned pIdx) const
{
  return m_NewOutSizes;
}

/// Factory
static SplitNode* SplitNodeCreator(onnx::Node& pN);

//===----------------------------------------------------------------------===//
// SplitNodeManager
//===----------------------------------------------------------------------===//
class SplitNodeManager
{
public:
  using SplitInfoHash = std::unordered_map<onnx::Node*, SplitNode*>;

  SplitNodeManager(onnx::Graph& pGraph);
  ~SplitNodeManager();

  /// @param pN Split from node pN.
  /// @param pUpdateUpper Propogate new size to upper levels.
  void splitNodeByFactor(onnx::Node* pN, unsigned pAxis, unsigned pFactor,
                         bool pUpdateUpper = true);

  bool splitNodeBySize(onnx::Node* pN, const LongInts& pNewOutSize,
                       bool pUpdateUpper = true);

private:
  SplitNode* getSplitNode(onnx::Node* pN);
  const SplitNode* getSplitNode(onnx::Node* pN) const;
  void clear();

  SplitInfoHash m_SplitInfos;
};

SplitNodeManager::SplitNodeManager(onnx::Graph& pGraph)
{
  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    m_SplitInfos[n] = SplitNodeCreator(*n);
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
  LongInts newS = ns->m_OutSizes;
  newS[pAxis] = (newS[pAxis] + pFactor - 1)/ pFactor;
  splitNodeBySize(pN, newS, pUpdateUpper);
}

bool SplitNodeManager::splitNodeBySize(onnx::Node* pN,
                                       const LongInts& pNewOutSize,
                                       bool pUpdateUpper)
{
  SplitNode* ns = getSplitNode(pN);
  if (!ns->useNewOutSize(pNewOutSize))
    return false;

  if (!pUpdateUpper)
    return true;

  bool status = true;
  for (unsigned i = 0; i < ns->m_Node.inputs().size(); ++i) {
    if (onnx::Node* child = ns->m_Node.inputs()[i]->node()) {
      if (child->kind() == onnx::kParam)
        continue;
      SplitNode* childNs = getSplitNode(child);
      LongInts newInS = childNs->calNewInputSize(i);
      status &= splitNodeBySize(child, newInS, true);
    }
  }
  return status;
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
      const TensorSizes &Cdim = m_Node.inputs()[2]->sizes();
      return {Cdim[0].dim, Cdim[1].dim};
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
      newIS[i + 2] = (m_NewOutSizes[i + 2] - 1) * m_Stride[i]
                     - m_PadBegin[i] - m_PadEnd[i] + m_KShape[i];
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
    assert(pIdx == 0 && "SplitReshape::calNewInputSize: Invalid input id.");

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

    LongInts newIS(4); // common case: N C H W
    const TensorSizes &data = m_Node.inputs()[0]->sizes();
    newIS[0] = m_NewOutSizes[0];

    // newIS[1] remains unchanged.
    int64_t C = data[1].dim;
    newIS[1] = C;

    int64_t HW = m_NewOutSizes[1] / C;
    newIS[2] = HW/2;
    newIS[3] = HW/2;
    return newIS;
  }
};

static SplitNode* SplitNodeCreator(onnx::Node& pN)
{
  if (OutputSizeIsInputSize(pN))
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

  assert(false && "Unsupport node.");
  return nullptr;
}

static void tryToSplitGraph(onnx::Graph &pGraph,
                            DLATargetBackend* pDLATB)
{
  const unsigned lsize = pDLATB->getMemInfo()->getLocalMemSize();

  SplitNodeManager snMgr(pGraph);
  // Try to split backward with greedy. Try to split first dim.

  for (onnx::Value* v : pGraph.outputs()) {
    if (v->node())
      snMgr.splitNodeByFactor(v->node(), 0, 2, true);
  }
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
using ValMemSizeMap = std::unordered_map<const onnx::Value *, MemSize>;

static void GetMemoryUsageForAllValues(onnx::Graph &pGraph,
                                       ValMemSizeMap &pVMSMap,
                                       DLATargetBackend* pDLATB)
{
  for (onnx::Node* n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    // get required memory size of each input.
    for (onnx::Value *v : n->inputs())
      pVMSMap[v] = pDLATB->getMemInfo()->getValueMemorySize(v);

    // get required memory size of each output.
    for (onnx::Value *v : n->outputs())
      pVMSMap[v] = pDLATB->getMemInfo()->getValueMemorySize(v);
  }
}

static void InsertLoadStoreNode(onnx::Graph &pGraph)
{
  for (onnx::Value* v : pGraph.inputs()) {
    onnx::Node* first = nullptr;
    for(auto u : v->uses()) {
      if (!first) {
        first = u.user;
        continue;
      }

      if (!first->isBefore(u.user))
        first = u.user;
    }

    // Create load node and insert before the first use node.
    onnx::Node* loadN = pGraph.create(g_LoadKind);
    loadN->insertBefore(first);
    loadN->output()->copyMetadata(v);
    v->replaceAllUsesWith(loadN->output());
  }

  for (onnx::Value* v : pGraph.outputs()) {
    onnx::Node* last = nullptr;
    for(auto u : v->uses()) {
      if (!last) {
        last = u.user;
        continue;
      }

      if (last->isBefore(u.user))
        last = u.user;
    }

    // Create store node and insert before the last use node.
    onnx::Node* storeN = pGraph.create(g_StoreKind, {v}, 0);
    storeN->insertBefore(last);
  }
}

//===----------------------------------------------------------------------===//
// MemoryAllocation
//===----------------------------------------------------------------------===//
MemoryAllocation::MemoryAllocation(DLATargetBackend* pDLATB)
  : ModulePass(ID), m_MemAllocList(), m_DLATB(pDLATB) {
}

MemoryAllocation::~MemoryAllocation()
{
  clear();
}

struct MemRegion {
  size_t start, size;
  MemRegion(size_t pStart, size_t pSize)
    : start(pStart), size(pSize) {
  }
  MemRegion() {}
};

using MemRegionList = std::vector<MemRegion>;

static MemRegionList GetUsedMemRegions(const MemAllocList& pAllocs,
                                       const LiveInterval &pIntrvl)
{
  MemRegionList regions;
  for (auto entry : pAllocs) {
    const LiveInterval& liveIntrvl = entry->liveIntrvl;

    if (!liveIntrvl.intersect(pIntrvl))
      continue;

    regions.emplace_back(entry->startAddr, entry->size);
  }

  // sort by starting address
  std::sort(regions.begin(), regions.end(),
            [] (const MemRegion& ra, const MemRegion& rb) {
              return ra.start < rb.start;
            });
  return regions;
}

static bool HasConflict(size_t pStartA, size_t pSizeA,
                        size_t pStartB, size_t pSizeB)
{
  size_t endA = pStartA + pSizeA,
         endB = pStartB + pSizeB;

  return !(endA <= pStartB || endB <= pStartA);
}

Pass::ReturnType MemoryAllocation::runOnModule(Module& pModule)
{
  if (!m_DLATB) {
    errs() << "No backend infomation that is needed for memory allcation.\n";
    return kPassFailure;
  }

  clear();

  onnx::Graph& graph = *pModule.getGraph();
  GraphLivenessAnalysis *liveAnaly = getAnalysis<GraphLivenessAnalysis>();

  ValMemSizeMap valMemSMap;
  GetMemoryUsageForAllValues(graph, valMemSMap, m_DLATB);

  InsertLoadStoreNode(graph);

  // get maximum requirements.
  unsigned maxSize = 0;
  for (auto & req : valMemSMap)
    maxSize += req.second.size;

  // by liverange analysis, we can get minimun requirement memory size.
  unsigned minSize = 0;

  // allocate memory considering liveness.
  auto &livesInfo = liveAnaly->getLiveIntervals();
  for (const LiveInterval* li : livesInfo) {
    auto v = &li->getValue();
    unsigned required = valMemSMap[v].size,
           startAddr = 0;

    MemRegionList conflicts = GetUsedMemRegions(m_MemAllocList, *li);

    // Note: conflicts has been sorted by starting address in GetUsedMemRegions.
    for (const MemRegion &reg : conflicts) {
      if (!HasConflict(reg.start, reg.size, startAddr, required))
        break;
      startAddr = reg.start + reg.size;
    }

    // Allocate new memory region.
    m_MemAllocList.push_back(new MemAllocEntry(startAddr,
                                               required, *li));
    minSize = std::max(minSize, startAddr + required);
  }

  if (minSize > m_DLATB->getMemInfo()->getLocalMemSize()) {
    tryToSplitGraph(graph, m_DLATB);
  }

  outs() << "Size req. Min = " << minSize << "(" << (float)minSize/(1024.f*1024.f) << " mb)"
         << " Max = " <<  maxSize << "(" << (float)maxSize/(1024.f*1024.f) << " mb)\n";

  return kModuleNoChanged;
}

void MemoryAllocation::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(GraphLivenessAnalysis::ID);
  pUsage.addRequiredID(UpdateGraphOutputSize::ID);
}

void MemoryAllocation::print(std::ostream& pOS) const
{
  for (const MemAllocEntry *e : m_MemAllocList) {
    const LiveInterval &li = e->liveIntrvl;
    pOS << li.getValue().uniqueName() << ": \t"
        << "[" << e->startAddr << ", " << e->startAddr + e->size << ")\t"
        << "(total: " << e->size << ")\t"
        << " [" << li.getStart() << ", " << li.getEnd() << "]"
        << "\n";
  }
}

void MemoryAllocation::clear()
{
  for (MemAllocEntry* entry: m_MemAllocList) {
    delete entry;
  }
  m_MemAllocList.clear();
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char MemoryAllocation::ID = 0;

namespace onnc
{
  INITIALIZE_DLA_PASS(MemoryAllocation, "MemoryAllocation")
}

MemoryAllocation* onnc::CreateMemoryAllocationPass(DLATargetBackend* pDLATB)
{
  return new MemoryAllocation(pDLATB);
}
