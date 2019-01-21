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
#include <stack>
#include <set>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PassManager
//===----------------------------------------------------------------------===//
PassManager::PassManager()
  : m_pPassRegistry(onnc::GetPassRegistry()),
    m_Dependencies(), m_AvailableAnalysis(),
    m_RunState(),
    m_pStart(m_Dependencies.addNode(new StartPass())),
    m_TimeStep(0) {
}

PassManager::PassManager(PassRegistry& pRegistry)
  : m_pPassRegistry(&pRegistry),
    m_Dependencies(), m_AvailableAnalysis(),
    m_RunState(),
    m_pStart(m_Dependencies.addNode(new StartPass())),
    m_TimeStep(0) {
}

PassManager::~PassManager()
{
  m_Dependencies.clear();
}

// Use depth search first to build up a sub-graph of dependenciess.
void PassManager::add(Pass* pPass, State& pState)
{
  addPassToDependencyGraph(pPass, nullptr);
  addPassToExeQueue(pPass, pState);
}

void PassManager::add(Pass* pPass, TargetBackend* pBackend, State& pState)
{
  addPassToDependencyGraph(pPass, pBackend);
  addPassToExeQueue(pPass, pState);
}

// Use depth search first to build up a sub-graph of dependenciess.
void PassManager::add(Pass* pPass)
{
  add(pPass, m_RunState);
}

void PassManager::add(Pass* pPass, TargetBackend* pBackend)
{
  add(pPass, pBackend, m_RunState);
}

void PassManager::addPassToExeQueue(Pass* pPass, State& pState)
{
  State s;
  s.execution.push_back(pPass->getPassID());
  UpdateExecutionOrder(s.execution);

  // Concate the two execution queue.
  pState.execution.insert(pState.execution.end(),
                          s.execution.begin(), s.execution.end());

}

/// Add a pass by DSF order
void PassManager::addPassToDependencyGraph(Pass* pPass, TargetBackend* pBackend)
{
  // If the pass is already in the dependency graph, then we don't
  // need to add it into the graph.
  if (hasAdded(pPass->getPassID()))
    return;

  std::stack<DepNode*> stack;
  DepNode* cur_node = addNode(*pPass);
  stack.push(cur_node);

  // process pass dependency.
  while (!stack.empty()) {
    cur_node = stack.top();
    stack.pop();

    AnalysisUsage usage;
    cur_node->pass->getAnalysisUsage(usage);
    if (usage.isEmpty()) {
      m_Dependencies.connect(*m_pStart, *cur_node);
      continue;
    }

    // create resolver on demand.
    // The pass is might be added to other PassManager, so it's resolver
    // does exist.
    AnalysisResolver* resolver = cur_node->pass->getResolver();
    if (!resolver) {
      resolver = new AnalysisResolver(*this);
      cur_node->pass->setResolver(*resolver);
    }

    for (Pass::AnalysisID& use : usage) {
      if (hasAdded(use)) {
        DepNode* dep_node = findNode(use);
        assert(dep_node && "dependency node doesn't exist?!");
        // add dependency.
        m_Dependencies.connect(*dep_node, *cur_node);
        resolver->add(use, *dep_node->pass);
        continue;
      }

      // Create dependent pass.
      const PassInfo* info = getPassRegistry()->getPassInfo(use);
      if (nullptr == info) {
        error(pass_not_registered) << "nullptr";
        return;
      }
      Pass* new_pass = info->makePass(pBackend);

      // Register the newly created pass
      DepNode* new_node = addNode(*new_pass);

      // add dependency for cur_node.
      m_Dependencies.connect(*new_node, *cur_node);
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
  for (Pass::AnalysisID id : pState.execution)
    lookup(id)->setTimeStep(0);
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
  DepNode* node = findNode(pState.execution.front());
  if (nullptr == node)
    return Pass::kPassFailure;
  pState.pass = node->pass;
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
    Pass* depPass = dep_node->pass;
    if (depPass->getTimeStep() > pPass.getTimeStep())
      return true;
  }
  return false;
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
  return result;
}

PassManager::DepNode* PassManager::findNode(Pass::AnalysisID pID) const
{
  AvailableAnalysisMap::const_iterator entry = m_AvailableAnalysis.find(pID);
  if (m_AvailableAnalysis.end() == entry)
    return nullptr;
  return entry->second;
}

PassManager::DepNode* PassManager::addNode(Pass& pPass)
{
  DepNode* cur_node = m_Dependencies.addNode(&pPass);
  m_AvailableAnalysis[cur_node->pass->getPassID()] = cur_node;
  return cur_node;
}

unsigned int PassManager::size() const
{
  return m_AvailableAnalysis.size();
}

Pass* PassManager::lookup(Pass::AnalysisID pID)
{
  DepNode* node = findNode(pID);
  if (nullptr == node)
    return nullptr;

  return node->pass;
}

void PassManager::printState(const State& pState, OStream& pOS) const
{
  pOS << "Execution queue: ";
  if (pState.execution.empty()) {
    pOS << "empty." << std::endl;
    return;
  }

  pOS << "Start";
  for (Pass::AnalysisID id : pState.execution) {
    DepNode* node = findNode(id);
    pOS << " -> " << node->pass->getPassName();
  }
  pOS << std::endl;
}

void PassManager::dumpState(const State& pState) const
{
  printState(pState, errs());
}

bool PassManager::hasAdded(Pass::AnalysisID pID) const
{
  return (m_AvailableAnalysis.end() != m_AvailableAnalysis.find(pID));
}

void PassManager::UpdateExecutionOrder(ExecutionOrder& pOrder)
{
  std::unordered_set<DepNode*> visited;
  std::deque<std::pair<bool, DepNode*> > stack;
  std::vector<DepNode*> post_order;

  stack.push_back(std::make_pair(false, findNode(pOrder.front())));

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
      pOrder.push_front((*ele)->pass->getPassID());
    }
    ++ele;
  }
}
