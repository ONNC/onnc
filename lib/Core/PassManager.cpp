//===- PassManager.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassManager.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/ADT/Bits/DigraphArc.h>

#include <algorithm>
#include <iterator>
#include <stack>
#include <set>
#include <numeric>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PassManager
//===----------------------------------------------------------------------===//
PassManager::PassManager()
  : m_pPassRegistry{onnc::GetPassRegistry()}
  , m_depGraph{}
  , m_depNodes{}
  , m_passStore{}
  , m_RunState{}
  , m_pStart{m_depGraph.addNode(StartPass::id())}
  , m_TimeStep{0u}
{ }

PassManager::PassManager(PassRegistry& pRegistry)
  : m_pPassRegistry{&pRegistry}
  , m_depGraph{}
  , m_depNodes()
  , m_passStore{}
  , m_RunState{}
  , m_pStart{m_depGraph.addNode(StartPass::id())}
  , m_TimeStep(0u)
{ }

// Use depth search first to build up a sub-graph of dependenciess.
PassManager& PassManager::add(Pass* pPass, State& pState)
{
  assert(pPass != nullptr);

  movePassToStore(pPass); // transfer ownership to *this
  addPassToDependencyGraph(pPass->getPassID(), nullptr);
  addPassToExeQueue(pPass, pState);
  return *this;
}

PassManager& PassManager::add(Pass* pPass, TargetBackend* pBackend, State& pState)
{
  assert(pPass != nullptr);

  movePassToStore(pPass); // transfer ownership to *this
  addPassToDependencyGraph(pPass->getPassID(), pBackend);
  addPassToExeQueue(pPass, pState);
  return *this;
}

// Use depth search first to build up a sub-graph of dependenciess.
PassManager& PassManager::add(Pass* pPass)
{
  return add(pPass, m_RunState);
}

PassManager& PassManager::add(Pass* pPass, TargetBackend* pBackend)
{
  return add(pPass, pBackend, m_RunState);
}

void PassManager::addPassToExeQueue(Pass* pPass, State& pState)
{
  State s;
  s.execution.emplace_back(pPass);
  UpdateExecutionOrder(s.execution);

  // Concate the two execution queue.
  pState.execution.insert(pState.execution.end(),
                          s.execution.begin(), s.execution.end());

}

/// Add a pass by DSF order
void PassManager::addPassToDependencyGraph(Pass::AnalysisID passId, TargetBackend* pBackend)
{
  // If the pass is already in the dependency graph, then we don't
  // need to add it into the graph.
  if (hasAdded(passId))
    return;

  std::stack<DepNode*> stack;
  DepNode* cur_node = addNode(passId);
  stack.push(cur_node);

  // process pass dependency.
  while (!stack.empty()) {
    cur_node = stack.top();
    stack.pop();

    AnalysisUsage usage;
    Pass * const currentPass = getPass(cur_node->passId);
    assert(currentPass != nullptr);

    currentPass->getAnalysisUsage(usage);
    if (usage.isEmpty()) {
      m_depGraph.connect(*m_pStart, *cur_node);
      continue;
    }

    // create resolver on demand.
    // The pass is might be added to other PassManager, so it's resolver
    // does exist.
    AnalysisResolver* resolver = currentPass->getResolver();
    if (!resolver) {
      resolver = new AnalysisResolver(*this);
      currentPass->setResolver(*resolver);
    }

    for (Pass::AnalysisID& use : usage) {
      if (hasAdded(use)) {
        DepNode* dep_node = findNode(use);
        assert(dep_node && "dependency node doesn't exist?!");
        // add dependency.
        m_depGraph.connect(*dep_node, *cur_node);
        resolver->add(use, *getPass(dep_node->passId));
        continue;
      }

      // Create dependent pass.
      const PassInfo* info = getPassRegistry()->getPassInfo(use);
      if (nullptr == info) {
        error(pass_not_registered) << "nullptr";
        return;
      }
      Pass* new_pass = info->makePass(pBackend);
      movePassToStore(new_pass);
      // Register the newly created pass
      DepNode* new_node = addNode(new_pass->getPassID());

      // add dependency for cur_node.
      m_depGraph.connect(*new_node, *cur_node);
      resolver->add(use, *new_pass);

      // continue traverse dependency of new node.
      stack.push(new_node);
    } // for each usage
  } // leave stacking
}

void PassManager::initRunState(Module& pModule, State& pState)
{
  m_TimeStep = 0;
  pModule.setTimeStep(1);
  for (auto& entry : m_passStore) {
    auto& passes = entry.second;
    for (auto& pass : passes) {
       pass->setTimeStep(0);
    }
  }
  m_lastExecuted.clear();
}

bool PassManager::run(Module& pModule, State& pState)
{
  initRunState(pModule, pState);

  while (!pState.execution.empty()) {
    if (!step(pModule, pState))
      return false;
  } // end of while
  return true;
}

bool PassManager::run(Module& pModule)
{
  return run(pModule, m_RunState);
}

bool PassManager::step(Module& pModule, State& pState)
{
  Pass* const pass = pState.execution.front();
  assert(pass != nullptr);

  DepNode* node = findNode(pass->getPassID());
  if (nullptr == node) {
    return Pass::kPassFailure;
  }

  pState.pass = pass;
  pState.executed = false;

  Pass::ReturnType result = Pass::kModuleNoChanged;
  if (needRun(*pState.pass, pModule)) {
    ++m_TimeStep;
    pState.pass->clear();
    pState.pass->setTimeStep(m_TimeStep);
    pState.pass->setModule(&pModule);
    pState.executed = true;
    result = doRun(*pState.pass, pModule);
  }

  if (Pass::IsFailed(result))
    return false;

  if (Pass::IsRevised(result))
    pModule.setTimeStep(m_TimeStep);

  if (Pass::IsRetry(result)) {
    UpdateExecutionOrder(pState.execution);
    pState.changed = false;
  }
  else { //< not retry
    if (Pass::IsRevised(result))
      pState.changed = true;
    pState.execution.pop_front();
  }

  return true;
}

bool PassManager::step(Module& pModule)
{
  return step(pModule, m_RunState);
}

bool PassManager::needRun(Pass& pPass, Module& pModule)
{
  if (pPass.getModule() != &pModule)
    return true;

  if (pModule.getTimeStep() > pPass.getTimeStep())
    return true;

  AnalysisUsage usage;
  pPass.getAnalysisUsage(usage);

  for (Pass::AnalysisID& use : usage) {
    DepNode* dep_node = findNode(use);
    const Pass* const depPass = getPass(dep_node->passId);
    if (depPass->getTimeStep() > pPass.getTimeStep())
      return true;
  }
  return false;
}

void PassManager::movePassToStore(Pass* pass)
{
  assert(pass != nullptr);

  const auto passId = pass->getPassID();

  // check whether the pass was already added, only add it if
  // not in the pass store; or issue an error
  using ManagedPassType = PassStore::mapped_type::value_type;
  auto& passes = m_passStore[passId];
  const auto found = std::find_if(
    begin(passes), end(passes),
    [pass](const ManagedPassType& stored) { return stored.get() == pass; }
  );
  if (found == end(passes)) {
    passes.emplace_back(pass);
  } else {
    assert(false && "cannot add a pass twice");
  }
}

Pass::ReturnType PassManager::doRun(Pass& pPass, Module& pModule)
{
  // initialize the pass
  Pass::ReturnType result = pPass.doInitialization(pModule);

  if (Pass::IsRetry(result) || Pass::IsFailed(result))
    return result;

  // run the pass
  result |= pPass.run(pModule);

  if (Pass::IsRetry(result) || Pass::IsFailed(result))
    return result;

  // finalize the pass
  result |= pPass.doFinalization(pModule);

  if (Pass::IsRetry(result) || Pass::IsFailed(result)) {
    return result;
  }

  setLastExecuted(pPass);

  return result;
}

PassManager::DepNode* PassManager::findNode(Pass::AnalysisID pID) const
{
  const auto found = m_depNodes.find(pID);
  if (found == end(m_depNodes)) {
    return nullptr;
  }

  return found->second;
}

PassManager::DepNode* PassManager::addNode(Pass::AnalysisID passId)
{
  if (hasAdded(passId)) {
    return m_depNodes.find(passId)->second;
  }

  DepNode* const cur_node = m_depGraph.addNode(passId);
  m_depNodes.emplace(passId, cur_node);
  return cur_node;
}

unsigned int PassManager::size() const
{
  return std::accumulate(
    begin(m_passStore), end(m_passStore),
    0u,
    [](unsigned init, const PassStore::value_type& entry) {
       return init + static_cast<unsigned>(entry.second.size());
    }
  );
}

void PassManager::printState(const State& pState, OStream& pOS) const
{
  pOS << "Execution queue: ";
  if (pState.execution.empty()) {
    pOS << "empty." << std::endl;
    return;
  }

  pOS << "Start";
  for (Pass* pass : pState.execution) {
    pOS << " -> " << pass->getPassName();
  }
  pOS << std::endl;
}

void PassManager::dumpState(const State& pState) const
{
  printState(pState, errs());
}

bool PassManager::hasAdded(Pass::AnalysisID pID) const
{
  return m_depNodes.find(pID) != end(m_depNodes);
}

void PassManager::UpdateExecutionOrder(ExecutionOrder& pOrder)
{
  std::unordered_set<DepNode*> visited;
  std::deque<std::pair<bool, DepNode*> > stack;
  std::vector<DepNode*> post_order;

  stack.emplace_back(false, findNode(pOrder.front()->getPassID()));

  while (!stack.empty()) {
    std::pair<bool, DepNode*> node = stack.back();
    stack.pop_back();

    if (node.first) { // get a parent
      if (post_order.end() == std::find(post_order.begin(), post_order.end(), node.second)) {
        post_order.push_back(node.second);
      }
    }
    else { // get a child

      // turn the child to parent
      visited.insert(node.second);
      stack.push_back(std::make_pair(true, node.second));

      // push all children
      digraph::ArcBase* iter = node.second->first_in;
      while (nullptr != iter) {
        // if not visited
        DepNode* target = static_cast<DepNode*>(iter->source);
        if (visited.end() == visited.find(target)) {
          stack.push_back(std::make_pair(false, target));
        }
        iter = iter->next_in;
      }
    }
  } // end of while

  std::vector<DepNode*>::reverse_iterator ele = post_order.rbegin();
  std::vector<DepNode*>::reverse_iterator eEnd = post_order.rend();
  ++ele;
  while (eEnd != ele) {
    if (eEnd != (ele + 1)) {
      const auto passId = (*ele)->passId;
      pOrder.push_front(getLastExecutedOrFromStore(passId));
    }
    ++ele;
  }
}

bool PassManager::hasLastExecuted(Pass::AnalysisID passId) const
{
   return m_lastExecuted.find(passId) != end(m_lastExecuted);
}

Pass* PassManager::getLastExecuted(Pass::AnalysisID passId) const
{
  const auto found = m_lastExecuted.find(passId);
  if (found == end(m_lastExecuted)) {
    return nullptr;
  }

  return found->second;
}

void PassManager::setLastExecuted(Pass& pass)
{
  const auto passId = pass.getPassID();
  const auto found = m_lastExecuted.find(passId);
  if (found != end(m_lastExecuted)) {
    found->second = &pass;
  } else {
    m_lastExecuted.emplace(passId, &pass);
  }
}

Pass* PassManager::getLastExecutedOrFromStore(Pass::AnalysisID passId) const
{
  if (hasLastExecuted(passId)) {
    return getLastExecuted(passId);
  }

  return getPass(passId);
}

Pass* PassManager::getPass(Pass::AnalysisID passId) const
{
  const auto found = m_passStore.find(passId);
  if (found == end(m_passStore)) {
    return nullptr;
  }

  const auto& passes = found->second;
  assert(!passes.empty());

  return passes.back().get();
}
