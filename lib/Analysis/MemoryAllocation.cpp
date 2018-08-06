//===- MemoryAllocation.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Analysis/MemoryAllocation.h>
#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/Analysis/SplitNode.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <iomanip>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MemoryAllocation
//===----------------------------------------------------------------------===//
MemoryAllocation::MemoryAllocation(DLATargetBackend* pDLATB)
  : ModulePass(ID), m_GraphMemAllocList(), m_DLATB(pDLATB) {
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
  for (const MemAllocEntry *entry : pAllocs) {
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

uint64_t MemoryAllocation::allocByLiveness(xGraph &pGraph,
                                           ValMemSizeMap &pValMemSizeMap,
                                           GraphLivenessAnalysis &pLiveAnaly)
{
  MemAllocList memAllocList;

  // by liverange analysis, we can get minimun requirement memory size.
  size_t minSize = 0;

  // allocate memory considering liveness.
  auto &livesInfo = pLiveAnaly.getLiveIntervals();
  for (const LiveInterval* li : livesInfo) {
    const xValue *v = &li->getValue();
    if (!pValMemSizeMap.count(v))
      continue;

    size_t required = pValMemSizeMap[v].size,
           startAddr = 0;

    MemRegionList conflicts = GetUsedMemRegions(memAllocList, *li);

    // Note: conflicts has been sorted by starting address in GetUsedMemRegions.
    for (const MemRegion &reg : conflicts) {
      if (!HasConflict(reg.start, reg.size, startAddr, required))
        break;
      startAddr = reg.start + reg.size;
    }

    // Allocate new memory region.
    memAllocList.push_back(new MemAllocEntry(startAddr, required, *li));
    minSize = std::max(minSize, startAddr + required);
  }

  clearGraphAlloc(&pGraph);
  m_GraphMemAllocList[&pGraph] = std::move(memAllocList);
  return minSize;
}

Pass::ReturnType MemoryAllocation::runOnModule(Module& pModule)
{
  if (!m_DLATB) {
    errs() << "No backend infomation that is needed for memory allcation.\n";
    return kPassFailure;
  }

  GraphLivenessAnalysis *liveAnaly = getAnalysis<GraphLivenessAnalysis>();
  NodeIRScheduler *scheduler = getAnalysis<NodeIRScheduler>();

  clear();

  SplitGraphManager sgMgr(*pModule.getGraphIR(), *m_DLATB);

  const uint64_t localMemSize = m_DLATB->getMemInfo()
                                       ->getLocalMemSize();
  std::vector<SplitGraph*> worklist(sgMgr.getSplitGraphs());
  while (!worklist.empty()) {
    SplitGraph *spGraph = worklist.back();
    worklist.pop_back();

    scheduler->runOnGraph(spGraph->getGraph());
    liveAnaly->runOnGraph(spGraph->getGraph());

    // per graph
    int64_t prevMinSize = 0;
    const float threshold = 0.9f; // 90% splitting threshold.

    outs() << "Allocate graph: " << spGraph->getGraph().name() << "\n";
    while (true) {
      ValMemSizeMap valMemSMap;
      spGraph->getMemUsage(valMemSMap);

      // Try to allocate.
      uint64_t minSize = allocByLiveness(spGraph->getGraph(),
                                         valMemSMap, *liveAnaly);
      outs() << " -> " << (float)minSize / 1024.f << " kb\n";
      if (minSize < localMemSize) {
        spGraph->setAllocStatus(true, minSize);
        break;
      }

      // If new allocation is not smaller enough than previous allocation, then
      // create new sub graph
      if (prevMinSize && (float)minSize / prevMinSize > threshold) {
        prevMinSize = 0;

        SplitGraph *newSpGraph = sgMgr.splitNewSubGraph(*spGraph);

        if (newSpGraph) {
          worklist.push_back(newSpGraph);

          spGraph->resetToOrigSize();
          scheduler->runOnGraph(spGraph->getGraph());
          liveAnaly->runOnGraph(spGraph->getGraph());
          continue;
        }
        else {
          outs() << "[MemoryAllocation] Unable to allocate memory for graph.\n";
          spGraph->setAllocStatus(false, minSize);
          break;
        }
      }
      prevMinSize = minSize;

      spGraph->shrinkSize();
    }
  }
  return kModuleChanged;
}

void MemoryAllocation::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(NodeIRScheduler::ID);
  pUsage.addRequiredID(GraphLivenessAnalysis::ID);
  pUsage.addRequiredID(UpdateGraphOutputSize::ID);
}

void MemoryAllocation::printGraphAlloc(OStream &pOS,
                                       const xGraph *pGraph) const
{
  const auto &it = m_GraphMemAllocList.find(const_cast<xGraph*>(pGraph));
  assert(it != m_GraphMemAllocList.end() &&
         "No memory allocation info for Graph");

  pOS << "Memory Allocation for " << pGraph->name() << " (" << pGraph << ")\n";
  for (const MemAllocEntry *e : it->second) {
    const LiveInterval &li = e->liveIntrvl;
    pOS << std::setw(20) << std::left << li.getValue().uniqueName()
        << " [" << std::setw(8) << std::right << e->startAddr << ", "
        << std::setw(8) << std::right << e->startAddr + e->size << ")"
        << " (total: " << std::setw(8) << std::right << e->size << ")"
        << " [ " << std::setw(3) << std::right << li.getStart() << ", "
        << std::setw(3) << std::right << li.getEnd() << "]"
        << "\n";
  }
  pOS << "\n";
}

void MemoryAllocation::print(OStream& pOS) const
{
  for (const auto &it : m_GraphMemAllocList)
    printGraphAlloc(pOS, it.first);
}

void MemoryAllocation::clearGraphAlloc(xGraph *pGraph)
{
  for (MemAllocEntry* entry : m_GraphMemAllocList[pGraph])
    delete entry;

  m_GraphMemAllocList[pGraph].clear();
}

void MemoryAllocation::clear()
{
  for (auto & it : m_GraphMemAllocList)
    clearGraphAlloc(it.first);
  m_GraphMemAllocList.clear();
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
