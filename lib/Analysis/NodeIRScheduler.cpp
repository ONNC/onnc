//===- NodeIRScheduler.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/IR/Dump.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <iomanip> // for setw
#include <limits>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static bool HasInsertedLoadStoreNode(onnx::Graph &pGraph)
{
  return pGraph.nodes().rbegin()->kind() == onnx::Symbol("Store");
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
    onnx::Node* loadN = pGraph.create(onnx::Symbol("Load"));
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
    onnx::Node* storeN = pGraph.create(onnx::Symbol("Store"), {v}, 0);
    //storeN->output()->copyMetadata(v);
    storeN->insertBefore(last);
  }
}

//===----------------------------------------------------------------------===//
// NodeIRScheduler
//===----------------------------------------------------------------------===//
NodeIRScheduler::NodeIRScheduler(DLATargetBackend* pDLATB)
  : ModulePass(ID), m_DLATB(pDLATB), m_CurCycle(0) {
}

NodeIRScheduler::~NodeIRScheduler()
{
}

using DegreeMap = std::unordered_map<onnx::Node *, unsigned>;

static DegreeMap BuildDegreeMap(onnx::Graph &pGraph)
{
  DegreeMap dmap;
  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    unsigned degcnt = n->inputs().size();
    for (onnx::Value *v : n->inputs())
      if (v->node() == nullptr) {
        outs() << "Warning! " << n->kind().toString()
               << " use a value = " << v->uniqueName()
               << ", which doen't bind to a node";
        --degcnt;
      }
    dmap[n] = degcnt;
  }
  return dmap;
}

bool NodeIRScheduler::isExeResAvailable(const ExeResource *pExeRes) const
{
  return m_ExeResUsers.find(pExeRes)->second.size() < pExeRes->numUnits;
}

void NodeIRScheduler::addExeResUser(const ExeResource *pExeRes,
                                    onnx::Node *pUser)
{
  uint64_t c = m_DLATB->getTTI()
                     ->getOperatorCost(pUser, TargetTransformInfo::kCycleCount);

  m_ExeResUsers[pExeRes].emplace_back(pUser, c);
  m_SchedTimeLine.emplace_back(pUser, m_CurCycle, m_CurCycle + c);
}

Nodes NodeIRScheduler::issue()
{
  // find min cycle count
  uint64_t minCycle = std::numeric_limits<uint64_t>::max();
  for (auto &it : m_ExeResUsers)
    for (auto &user : it.second)
      minCycle = std::min(minCycle, user.remainCycles);

  m_CurCycle += minCycle;

  // update cycle count based on min cycle count.
  Nodes complete;
  for (auto &it : m_ExeResUsers) {
    std::vector<unsigned> rmList;
    auto &userList = it.second;
    for (unsigned i = 0; i < userList.size(); ++i) {
      userList[i].remainCycles -= minCycle;
      if (userList[i].remainCycles == 0) {
        rmList.push_back(i);
        complete.push_back(userList[i].user);
      }
    }

    // release exe resource.
    for (int i = rmList.size() - 1; i >= 0; --i)
      userList.erase(userList.begin() + i);
  }
  return complete;
}

Nodes NodeIRScheduler::greedyPickNextNodes(Nodes &pCands)
{
  Nodes next;
  Ints rmList;

  // Greedy pick from beginning of the list.
  for (unsigned i = 0; i < pCands.size(); ++i) {
    onnx::Node *n = pCands[i];
    const ExeResource *res = m_DLATB->getTTI()->queryExeResType(n);
    // initialize entry with empty user.
    if (m_ExeResUsers.find(res) == m_ExeResUsers.end())
      m_ExeResUsers[res] = {};

    if (isExeResAvailable(res)) {
      addExeResUser(res, n);
      next.push_back(n);
      rmList.push_back(i);
    }
  }

  // Remove picked node in backward direction.
  for (auto it = rmList.rbegin(); it != rmList.rend(); ++it)
    pCands.erase(pCands.begin() + *it);

  return next;
}

bool NodeIRScheduler::isAllExeResEmpty() const
{
  for (auto &it : m_ExeResUsers) {
    auto &userList = it.second;
    if (!userList.empty())
      return false;
  }
  return true;
}

Pass::ReturnType NodeIRScheduler::runOnModule(Module& pModule)
{
  if (!m_DLATB) {
    errs() << "No backend infomation that is needed for memory allcation.\n";
    return kPassFailure;
  }

  clear();

  onnx::Graph &graph = *pModule.getGraph();
  if (!HasInsertedLoadStoreNode(graph));
    InsertLoadStoreNode(graph);

  DegreeMap dmap = BuildDegreeMap(graph);
  Nodes worklist;

  // add degree = 0 to worklist in graph order.
  for (onnx::Node *n : graph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    if (dmap[n] == 0)
      worklist.push_back(n);
  }

  while (!worklist.empty() || !isAllExeResEmpty()) {
    // worklist is modified in greedyPickNextNodes
    greedyPickNextNodes(worklist);
    Nodes completes = issue();

    for (onnx::Node *n : completes) {
      for (onnx::Value *v : n->outputs()) {
        // Update degree map.
        for(auto u : v->uses()) {
          if (u.user->kind() == onnx::kReturn)
            continue;
          auto it = dmap.find(u.user);
          assert(it != dmap.end() &&
                 "Node doesn't exist in dmap!?");
          // --Degree
          it->second -= 1;
          if (it->second == 0)
            worklist.push_back(it->first);
        } // for each user of this value.
      } // for each output value.
    } // for each picked node.
  } // while !worklist.empty() || !isAllExeResEmpty()

  // Reorder the IR position based on scheduling result.
  auto it = graph.begin();
  if (it->kind() == onnx::kUndefined)
    ++it;

  for (unsigned i = 0; i < m_SchedTimeLine.size(); ++i) {
    onnx::Node *n = m_SchedTimeLine[i].node;
    if (*it != n)
      n->moveBefore(*it);
    else
      ++it;
  }

  return kModuleChanged;
}

void NodeIRScheduler::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(UpdateGraphOutputSize::ID);
}

void NodeIRScheduler::print(OStream& pOS) const
{
  auto normalize = [] (uint64_t c) -> unsigned {
                     // 1 million cycle.
                     const uint64_t cycleUnit = 1000000;
                     return (c + cycleUnit - 1) / cycleUnit;
                   };

  // print schedule diagram
  for (const ExeCycle &exe : m_SchedTimeLine) {
    pOS << std::setw(normalize(exe.begin));
    for (unsigned i = 0; i < normalize(exe.end - exe.begin); ++i)
      pOS << '*';
    PrintNode(pOS, *exe.node);
  }
}

void NodeIRScheduler::inorderSingleIssueSchedule(Module& pModule)
{
  onnx::Graph &graph = *pModule.getGraph();
  int i = 0;
  uint64_t curCycle = 0;
  m_SchedTimeLine.clear();
  for (onnx::Node *n : graph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    uint64_t c = m_DLATB->getTTI()
                        ->getOperatorCost(n, TargetTransformInfo::kCycleCount);

    m_SchedTimeLine.emplace_back(n, curCycle, curCycle + c);
    curCycle += c;
    ++i;
  }
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char NodeIRScheduler::ID = 0;

namespace onnc
{
  INITIALIZE_DLA_PASS(NodeIRScheduler, "NodeIRScheduler")
}

NodeIRScheduler *onnc::CreateNodeIRSchedulerPass(DLATargetBackend *pDLATB)
{
  return new NodeIRScheduler(pDLATB);
}
