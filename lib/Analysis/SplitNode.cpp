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
#include <tuple>
#include <onnc/IR/Dump.h>

using namespace onnc;

typedef std::vector<onnx::Node *> Nodes;
typedef std::unordered_set<onnx::Node *> NodeSet;
typedef std::unordered_map<onnx::Node *, unsigned> DegreeMap;

static const onnx::NodeKind g_LoadKind = onnx::Symbol("Load");
static const onnx::NodeKind g_StoreKind = onnx::Symbol("Store");
static const onnx::NodeKind g_SubGraphKind = onnx::Symbol("SubGraph");

static LongInts GetOutputValueSizes(const onnx::Node& pN)
{
  if (pN.outputs().size())
    return GetValueSizes(*pN.outputs()[0]);
  if (pN.kind() == g_StoreKind)
    return GetValueSizes(*pN.inputs()[0]);
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
// Split Graph
//===----------------------------------------------------------------------===//
using NodeNodeMap = std::unordered_map<onnx::Node*, onnx::Node*>;

static void cloneNodeAndSuccessors(onnx::Node *pNode, onnx::Graph *pNewGraph,
                                   NodeNodeMap &pOldNewMap, NodeSet &pHasCloned)
{
  std::vector<onnx::Node *> worklist;

  worklist.reserve(8);
  worklist.push_back(pNode);

  while (!worklist.empty()) {
    onnx::Node *oldN = worklist.back();
    worklist.pop_back();
    if (pHasCloned.count(oldN))
      continue;

    pHasCloned.insert(oldN);

    onnx::Node *newN = pNewGraph->create(oldN->kind(), oldN->outputs().size());
    newN->copyAttributes(*oldN);
    pNewGraph->appendNode(newN);
    pOldNewMap[oldN] = newN;

    for (unsigned i = 0; i < oldN->outputs().size(); ++i) {
      onnx::Value *outv = oldN->outputs()[i];
      newN->outputs()[i]->copyMetadata(outv);

      for (auto u : outv->uses())
        worklist.push_back(u.user);
    }
  }
}

static void rebuildInputs(NodeNodeMap &pOldNewMap)
{
  // Rebuild inputs for newly created nodes from pOldNewMap.
  onnx::Node *oldRetN = nullptr;
  for (auto &it : pOldNewMap) {
    onnx::Node *oldN = it.first, *newN = it.second;
    for (onnx::Value *oldv : oldN->inputs()) {
      auto valIt = pOldNewMap.find(oldv->node());
      if (valIt == pOldNewMap.end()) {
        outs() << "[Warning] rebuildInputs: required input value = "
               << oldv->uniqueName() << " is not found in new nodes map.\n";
        continue;
      }
      // [FIXME] We should remember newN's input[i] maps to which output[j] of
      //         newN's parent node.
      if (valIt->first->outputs().size() > 1) {
        outs() << "[Warning] rebuildInputs: parent node "
               << valIt->first->outputs()[0]->uniqueName()
               << " has more than one output value.\n";
      }

      // A graph has only one return node.
      if (newN->kind() == onnx::kReturn)
        oldRetN = oldN;

      newN->addInput(valIt->second->outputs()[0]);
    }
  }

  if (oldRetN) {
    onnx::Node *newRetN = pOldNewMap[oldRetN];
    onnx::Node *graphRetN = newRetN->owningGraph()->return_node();
    for (onnx::Value *input : newRetN->inputs())
      graphRetN->addInput(input);

    newRetN->destroy();
    pOldNewMap.erase(oldRetN);
  }
}

static void removeNodeAndSuccessors(onnx::Node *pNode, NodeSet &pHasRemoved)
{
  std::vector<onnx::Node *> worklist;

  worklist.reserve(8);
  worklist.push_back(pNode);

  while (!worklist.empty()) {
    onnx::Node *oldN = worklist.back();
    worklist.pop_back();
    // Can't delete return node.
    if (oldN->kind() == onnx::kReturn)
      continue;

    if (pHasRemoved.count(oldN))
      continue;

    for (onnx::Value *outv : oldN->outputs())
      for (auto u : outv->uses()) {
        u.user->removeAllInputs();
        worklist.push_back(u.user);
      }

    oldN->destroy();
    pHasRemoved.insert(oldN);
  }
}

using LoadStorePair = std::pair<onnx::Node*, onnx::Node*>;

static void createLoadStoreAtNode(onnx::Graph &pGraph, onnx::Node &pN,
                                  std::vector<LoadStorePair> &pNewLoadStores)
{
  // Create new store and load pairs.
  for (onnx::Value *outv : pN.outputs()) {
    onnx::Node* first = nullptr;
    for(auto u : outv->uses()) {
      if (!first) {
        first = u.user;
        continue;
      }

      if (!first->isBefore(u.user))
        first = u.user;
    }

    // Create load node and insert before the first use node.
    // FIXME: the first using should be in the same group, should we need to
    //        check this?
    onnx::Node* loadN = pGraph.create(onnx::Symbol("Load"));
    loadN->insertBefore(first);
    loadN->output()->copyMetadata(outv);
    outv->replaceAllUsesWith(loadN->output());

    // create store after creating load (since replaceAllUseWith).
    onnx::Node* storeN = pGraph.create(onnx::Symbol("Store"), {outv});
    storeN->insertAfter(&pN);

    pNewLoadStores.emplace_back(loadN, storeN);
  }
}

static DegreeMap BuildDegreeMap(onnx::Graph &pGraph);

static void TopologicalSort(onnx::Graph &pGraph)
{
  DegreeMap dmap = BuildDegreeMap(pGraph);
  Nodes worklist;

  // Add degree = 0 to worklist in graph order.
  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    if (dmap[n] == 0)
      worklist.push_back(n);
  }

  // topological sort.
  Nodes orderedList;
  while (!worklist.empty()) {
    onnx::Node *n = worklist.back();
    worklist.pop_back();
    orderedList.push_back(n);
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
  }

  // Reorder the IR position based on topological sort.
  auto it = pGraph.begin();
  if (it->kind() == onnx::kUndefined)
    ++it;

  for (unsigned i = 0; i < orderedList.size(); ++i) {
    onnx::Node *n = orderedList[i];
    if (*it != n)
      n->moveBefore(*it);
    else
      ++it;
  }
}

static onnx::Graph *SplitGraph(onnx::Graph &pGraph, Nodes &pSplitPts)
{
  // Create new sub graph.
  // Note: 1. new sub graph does not include split points.
  //       2. new sub graph should be deleted by DeleteSubGraph pass.
  // TODO: DeleteSubGraph
  onnx::Graph *newGraph = new onnx::Graph();

  std::vector<LoadStorePair> newLoadStores;

  // Create load/store to split graph.
  for (onnx::Node *spNode : pSplitPts) {
    if (spNode->kind() == g_LoadKind)
      newLoadStores.emplace_back(spNode, nullptr);
    else
      createLoadStoreAtNode(pGraph, *spNode, newLoadStores);
  }

  // <old node, new node>
  NodeNodeMap OldNewMap;

  // Now, clone loads and it's successors to new graph.
  NodeSet hasClonedSet;
  for (auto &ldst : newLoadStores)
    cloneNodeAndSuccessors(ldst.first, newGraph, OldNewMap, hasClonedSet);

  rebuildInputs(OldNewMap);

  // Create a new node to contain the subgraph
  onnx::Node *subgN = pGraph.create(onnx::Symbol("SubGraph"));
  subgN->g_(subgN->kind(), std::unique_ptr<onnx::Graph>(newGraph));

  // remove load and it's successors from old graph, and connect store to the
  // new subgraph node.
  NodeSet hasRemovedSet;
  for (auto &ldst : newLoadStores) {
    removeNodeAndSuccessors(ldst.first, hasRemovedSet);
    if (ldst.second)
      subgN->addInput(ldst.second->output());
  }

  subgN->insertBefore(pGraph.return_node());
  pGraph.return_node()->addInput(subgN->output());

  TopologicalSort(*newGraph);

  return newGraph;
}

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
      if (m_SnMgr.hasSplitNode(n))
        continue;

      worklist.push_back(m_SnMgr.getSplitNode(n));
    }
  }
}

// TODO: getMemUsage: use a graph, and traverse graph
void SplitGroup::getMemUsage(const TargetTransformInfo *pTTI,
                             ValMemSizeMap &pVMSMap)
{
  std::vector<SplitNode *> worklist;
  for (SplitNode *store : m_Stores)
    for (onnx::Value *v : store->getNode().inputs())
      worklist.push_back(m_SnMgr.getSplitNode(v->node()));

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
      if (!m_SnMgr.hasSplitNode(upperN))
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

SplitGroup *SplitGroup::splitNewGroup(const TargetTransformInfo *pTTI)
{
  // choose a split point. Currently choose a point that can split the group
  // (roughly) evenly in terms of memory size.
  std::vector<onnx::Node *> worklist;
  for (SplitNode *store : m_Stores)
    for (onnx::Value *v : store->getNode().inputs())
      worklist.push_back(v->node());

  std::vector<onnx::Node *> halfPtList;
  while (!worklist.empty()) {
    onnx::Node *n = worklist.back();
    worklist.pop_back();
    halfPtList.push_back(findHalfSizePoints(pTTI, n));
  }

  SplitGroup *newgroup = m_SnMgr.createNewGroup();

  for (onnx::Node *halfpt : halfPtList)
    m_SnMgr.createLoadStoreAtNode(halfpt, newgroup);

  return newgroup;
}

// [TODO] BuildDegreeMap: Duplicate code, please merge with NodeIRScheduler
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

Nodes findHalfSizeSplitPoints(onnx::Graph &pGraph,
                              const TargetTransformInfo &pTTI)
{
  // get total required size.
  uint64_t total = 0;
  std::unordered_map<onnx::Node *, uint64_t> nodeSize;

  for (onnx::Node *n : pGraph.nodes()) {
    // skip load and store since they are calculated by their user nodes.
    if (n->kind() == g_LoadKind || n->kind() == g_StoreKind ||
        n->kind() == g_SubGraphKind)
      continue;

    nodeSize[n] = pTTI.getOperatorMemUsage(n).size;
    total += nodeSize[n];
  }

  // Build degree map and do topological + dfs traversing.
  DegreeMap dmap = BuildDegreeMap(pGraph);
  Nodes worklist;

  // Add degree = 0 to worklist in graph order.
  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    if (dmap[n] == 0)
      worklist.push_back(n);
  }

  std::unordered_set<onnx::Node *> grpA, grpB;
  uint64_t accumulateSize = 0;
  uint64_t size_a = 0;

  // topological + dfs traversing
  while (!worklist.empty()) {
    onnx::Node *n = worklist.back();
    worklist.pop_back();
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

    if (n->kind() == g_LoadKind || n->kind() == g_StoreKind ||
        n->kind() == g_SubGraphKind)
      continue;

    if (accumulateSize < total/2) {
      grpA.insert(n);
      size_a = accumulateSize;
    } else {
      grpB.insert(n);
    }

    accumulateSize += pTTI.getOperatorMemUsage(n).size;
  }

  // put load/store to correct group.
  for (onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == g_LoadKind) {
      // Assume users of this value are in the same group.
      onnx::Node *user = n->output()->uses()[0].user;
      if (grpA.count(user))
        grpA.insert(n);
      else
        grpB.insert(n);

    } else if (n->kind() == g_StoreKind) {
      if (grpA.count(n->input()->node()))
        grpA.insert(n);
      else
        grpB.insert(n);

    } else if (n->kind() == g_SubGraphKind) {
      assert(false && "Please implement it.");
    }
  }

  // find split points from group A.
  Nodes splitPts;
  for (onnx::Node *n : grpA) {
    // If the node's user is not in grpA, add to split points.
    for (onnx::Value *outv : n->outputs())
      for (auto u : outv->uses())
        if (!grpA.count(u.user)) {
          splitPts.push_back(n);
          assert(grpB.count(u.user) && "Node is not in both split group!?");
        }
  }

  // Add all load of grpB to splitPts
  for (onnx::Node *n : grpB)
    if (n->kind() == g_LoadKind)
      splitPts.push_back(n);

  return splitPts;
}

onnx::Node *SplitGroup::findHalfSizePoints(const TargetTransformInfo *pTTI,
                                           onnx::Node *pStart) const
{
  // <accumulated size, node>
  using AccSizeNodePair = std::tuple<uint64_t, onnx::Node *>;
  std::vector<AccSizeNodePair> accSizes;
  std::vector<onnx::Node *> worklist;
  worklist.push_back(pStart);

  uint64_t total = 0;
  while (!worklist.empty()) {
    onnx::Node *n = worklist.back();
    worklist.pop_back();

    if (n->kind() == g_LoadKind)
      continue;

    total = pTTI->getOperatorMemUsage(n).size;
    if (!accSizes.empty())
      total += std::get<0>(accSizes.back());

    accSizes.emplace_back(total, n);

    // add upper layer to worklist.
    for (onnx::Value *v : n->inputs()) {
      onnx::Node *upperN = v->node();
      if (!m_SnMgr.hasSplitNode(upperN))
        continue;

      worklist.push_back(upperN);
    }
  }

  AccSizeNodePair target = std::make_tuple(total/2, nullptr);
  const auto &halfPt =
    std::upper_bound(accSizes.begin(), accSizes.end(),
                     target,
                     [] (const AccSizeNodePair &a, const AccSizeNodePair &b) {
                       return std::get<0>(a) < std::get<0>(b);
                     });
  return std::get<1>(*halfPt);
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

onnx::Graph *SplitNodeManager::splitGraph(onnx::Graph &pGraph)
{
  Nodes splitPts = findHalfSizeSplitPoints(pGraph, *m_DLATB.getTTI());
  return SplitGraph(pGraph, splitPts);
}

bool SplitNodeManager::splitNodeBySize(onnx::Node* pN,
                                       const LongInts& pNewOutSize,
                                       bool pUpdateUpper)
{
  SplitNode* ns = getSplitNode(pN);
  if (!ns->useNewOutSize(pNewOutSize))
    return false;

  // Load IR is a boundary, it is paired with Store IR and form a subgraph.
  if (!pUpdateUpper)
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

SplitGroup *SplitNodeManager::createNewGroup()
{
  SplitGroup *ngrp = new SplitGroup(*this);
  m_Groups.push_back(ngrp);
  return ngrp;
}

void SplitNodeManager::createLoadStoreAtNode(onnx::Node *pN, SplitGroup *pGroup)
{
  // Create new store and load pairs.
  for (onnx::Value *outv : pN->outputs()) {
    onnx::Node* first = nullptr;
    for(auto u : outv->uses()) {
      if (!first) {
        first = u.user;
        continue;
      }

      if (!first->isBefore(u.user))
        first = u.user;
    }

    // Create load node and insert before the first use node.
    // FIXME: the first using should be in the same group, should we need to
    //        check this?
    onnx::Node* loadN = m_Graph.create(onnx::Symbol("Load"));
    loadN->insertBefore(first);
    loadN->output()->copyMetadata(outv);
    outv->replaceAllUsesWith(loadN->output());

    // create store after creating load (since replaceAllUseWith).
    onnx::Node* storeN = m_Graph.create(onnx::Symbol("Store"), {outv});
    storeN->insertAfter(pN);

    m_SplitInfos[loadN] = SplitNodeCreator(*loadN);
    m_SplitInfos[storeN] = SplitNodeCreator(*storeN);
    pGroup->addStore(m_SplitInfos[storeN]);
  }
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

    // don't count store/load node since it's size has been added by upper node.
    if (n->kind() == g_StoreKind ||
        n->kind() == g_LoadKind)
      continue;

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
