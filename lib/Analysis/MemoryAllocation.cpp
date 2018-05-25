//===- MemoryAllocation.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/MemoryAllocation.h>
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Analysis/SplitNode.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TargetTransformInfo.h>

using namespace onnc;

using TensorSizes = std::vector<onnx::Dimension>;

//===----------------------------------------------------------------------===//
// Extension IR for onnx
//===----------------------------------------------------------------------===//
static const onnx::Symbol g_LoadKind("Load");
static const onnx::Symbol g_StoreKind("Store");

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
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

size_t MemoryAllocation::allocByLiveness(ValMemSizeMap &pValMemSizeMap)
{
  clear();

  GraphLivenessAnalysis *liveAnaly = getAnalysis<GraphLivenessAnalysis>();

  // by liverange analysis, we can get minimun requirement memory size.
  size_t minSize = 0;

  // allocate memory considering liveness.
  auto &livesInfo = liveAnaly->getLiveIntervals();
  for (const LiveInterval* li : livesInfo) {
    auto v = &li->getValue();
    size_t required = pValMemSizeMap[v].size,
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
  return minSize;
}

Pass::ReturnType MemoryAllocation::runOnModule(Module& pModule)
{
  if (!m_DLATB) {
    errs() << "No backend infomation that is needed for memory allcation.\n";
    return kPassFailure;
  }

  clear();

  onnx::Graph& graph = *pModule.getGraph();

  SplitNodeManager snMgr(graph, *m_DLATB);

  bool stop = false, success = false;
  unsigned splitAxis = 0, factor = 1;
  size_t prevMinSize = 0;

  // 90% splitting threshold.
  float threshold = 0.9f;

  while (!stop) {
    ValMemSizeMap valMemSMap;
    snMgr.getMemoryUsageForAllValues(valMemSMap);

    // get maximum requirements.
    size_t maxSize = 0;
    for (auto & req : valMemSMap)
      maxSize += req.second.size;

    size_t minSize = allocByLiveness(valMemSMap);
    if (minSize < m_DLATB->getMemInfo()->getLocalMemSize()) {
      success = true;
      break;
    }

    // If new allocation is not smaller enough than previous allocation, then
    // create group
    if (prevMinSize && (float)minSize / prevMinSize > threshold)
      break;

    prevMinSize = minSize;

    // Try to split backward. Try to split from first dim.
    for (onnx::Value* v : graph.outputs()) {
      if (v->node()) {
        const TensorSizes &origSizes = v->sizes();
        ++factor;
        // Can't divide this axis further, try next axis.
        if (origSizes[splitAxis].dim < factor) {
          ++splitAxis;
          factor = 1;
        }

        if (origSizes.size() == splitAxis) {
          stop = true;
          break;
        }

        snMgr.splitNodeByFactor(v->node(), splitAxis, factor, true);
      }
    }

    outs() << "Size req. Min = " << minSize << "(" << (float)minSize/(1024.f*1024.f) << " mb)"
           << " Max = " <<  maxSize << "(" << (float)maxSize/(1024.f*1024.f) << " mb)\n";
  }
  snMgr.dump();
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
