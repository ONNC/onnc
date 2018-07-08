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

char PassManager::StartPass::ID = 0;
//===----------------------------------------------------------------------===//
// PassManager
//===----------------------------------------------------------------------===//
PassManager::PassManager()
  : m_pPassRegistry(onnc::GetPassRegistry()),
    m_Dependencies(), m_AvailableAnalysis(),
    m_RunState(),
    m_pStart(m_Dependencies.addNode(new StartPass())) {
}

PassManager::PassManager(PassRegistry& pRegistry)
  : m_pPassRegistry(&pRegistry),
    m_Dependencies(), m_AvailableAnalysis(),
    m_RunState(),
    m_pStart(m_Dependencies.addNode(new StartPass())) {
}

PassManager::~PassManager()
{
  m_Dependencies.clear();
}

// Use depth search first to build up a sub-graph of dependenciess.
void PassManager::add(Pass* pPass, State& pState)
{
  doAdd(pPass, nullptr, pState);
}

void PassManager::add(Pass* pPass, TargetBackend* pBackend, State& pState)
{
  doAdd(pPass, pBackend, pState);
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

/// Add a pass by DSF order
void PassManager::doAdd(Pass* pPass, TargetBackend* pBackend, State& pState)
{
  pState.execution.push_back(pPass->getPassID());

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

bool PassManager::run(Module& pModule, State& pState)
{
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

  Pass::ReturnType result = doRun(*pState.pass, pModule);
  if (Pass::IsFailed(result))
    return false;

  if (Pass::IsRetry(result)) {
    if (Pass::IsRevised(result)) {
      UpdateExecutionOrder(pState.execution);
      pState.changed = false;
    }
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

PassManager::DepNode* PassManager::findNode(Pass::AnalysisID pID)
{
  AvailableAnalysisMap::iterator entry = m_AvailableAnalysis.find(pID);
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
