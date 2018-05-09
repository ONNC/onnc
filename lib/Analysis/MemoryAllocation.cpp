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
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Support/IOStream.h>
#include <limits>
#include <vector>
#include <unordered_map>

using TensorSizes = std::vector<onnx::Dimension>;
using TP_DataTy = onnx::TensorProto_DataType;

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

// FIXME: Query from TargetInfo
size_t GetElemSize(TP_DataTy pTy)
{
  return 4;
}

static size_t GetTensorMemSize(const TensorSizes &pTS, TP_DataTy pTy)
{
  size_t s = GetElemSize(pTy);
  for (const onnx::Dimension &dim : pTS)
    s *= dim.dim;
  return s;
}

using ValMemSizeMap = std::unordered_map<const onnx::Value *, size_t>;

static void GetMemoryUsageForAllValues(onnx::Graph &pGraph,
                                       ValMemSizeMap &pVMSMap)
{
  // Try to allocate virtual memory according to liveness range.
  for (onnx::Node* n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    // get required memory size of each input.
    for (onnx::Value *v : n->inputs())
      pVMSMap[v] = GetTensorMemSize(v->sizes(), v->elemType());

    // get required memory size of each output.
    for (onnx::Value *v : n->outputs())
      pVMSMap[v] = GetTensorMemSize(v->sizes(), v->elemType());
  }
}

//===----------------------------------------------------------------------===//
// MemoryAllocation
//===----------------------------------------------------------------------===//
MemoryAllocation::MemoryAllocation(TargetBackend* pTB)
  : ModulePass(ID), m_MemAllocList() {
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

bool MemoryAllocation::runOnModule(Module& pModule)
{
  clear();

  // getAnalysis<GraphLivenessAnalysis>(pModule)
  GraphLivenessAnalysis *liveAnaly = getAnalysis<GraphLivenessAnalysis>();

  ValMemSizeMap valMemSMap;
  GetMemoryUsageForAllValues(*pModule.getGraph(), valMemSMap);

  // allocate memory considering liveness.
  auto &livesInfo = liveAnaly->getLiveIntervals();
  for (const LiveInterval* li : livesInfo) {
    auto v = &li->getValue();
    size_t required = valMemSMap[v],
           startAddr = 0;

    MemRegionList conflicts = GetUsedMemRegions(m_MemAllocList, *li);

    for (const MemRegion &reg : conflicts) {
      if (!HasConflict(reg.start, reg.size, startAddr, required))
        break;
      startAddr = reg.start + reg.size;
    }

    // Allocate new memory region.
    m_MemAllocList.push_back(new MemAllocEntry(startAddr,
                                               required, *li));
  }

  return false;
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

MemoryAllocation* onnc::CreateMemoryAllocationPass()
{
  return new MemoryAllocation();
}
