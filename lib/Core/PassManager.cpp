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
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IOStream.h>
#include <stack>
#include <set>

using namespace onnc;

char PassManager::StartPass::ID = 0;
//===----------------------------------------------------------------------===//
// PassManager
//===----------------------------------------------------------------------===//
PassManager::PassManager()
  : m_pPassRegistry(onnc::GetPassRegistry()),
    m_AvailableAnalysis(), m_Dependencies(),
    m_pStart(m_Dependencies.addNode(new StartPass())) {
}

PassManager::PassManager(PassRegistry& pRegistry)
  : m_pPassRegistry(&pRegistry),
    m_AvailableAnalysis(), m_Dependencies(),
    m_pStart(m_Dependencies.addNode(new StartPass())) {
}

PassManager::~PassManager()
{
  m_Dependencies.clear();
}

// Use depth search first to build up a sub-graph of dependenciess.
void PassManager::add(Pass* pPass)
{
  doAdd(pPass, nullptr);
}

void PassManager::add(Pass* pPass, TargetBackend* pBackend)
{
  doAdd(pPass, pBackend);
}

/// Add a pass by DSF order
void PassManager::doAdd(Pass* pPass, TargetBackend* pBackend)
{
  if (hasAdded(pPass->getPassID()))
    return;

  DepNode* cur_node = m_Dependencies.addNode(pPass);
  std::stack<DepNode*> stack;
  stack.push(cur_node);

  while (!stack.empty()) {
    cur_node = stack.top();
    if (hasAdded(cur_node->pass->getPassID()))
      return;

    stack.pop();

    AnalysisUsage usage;
    cur_node->pass->getAnalysisUsage(usage);
    if (usage.isEmpty()) {
      m_Dependencies.connect(*m_pStart, *cur_node);
    }
    else {
      for (AnalysisUsage::iterator use = usage.begin(); use != usage.end(); ++use) {
        if (!hasAdded(*use)) {
          // use existed node or create a new node
          DepNode* new_node = findNode(*use);
          if (nullptr == new_node) {
            // make the pass by ourself
            const PassInfo* info = getPassRegistry()->getPassInfo(*use);
            if (nullptr == info) {
              error(pass_not_registered) << "nullptr";
              return;
            }
            Pass* new_pass = info->makePass(pBackend);
            new_node = m_Dependencies.addNode(new_pass);
            stack.push(new_node);
          }
          m_Dependencies.connect(*new_node, *cur_node);
        }
      } // for each usage
    }
    m_AvailableAnalysis[cur_node->pass->getPassID()] = cur_node;
  } // leave stacking
}

bool PassManager::run(Module& pModule)
{
  bool changed = false;
  PassDependencyLattice::bfs_iterator node, pEnd = m_Dependencies.bfs_end();
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    changed |= node->pass->doInitialization(pModule);
  }

  // run the body
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    changed |= node->pass->run(pModule);
  }

  // finalization
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    changed |= node->pass->doFinalization(pModule);
  }
  return changed;
}

PassManager::DepNode* PassManager::findNode(Pass::AnalysisID pID)
{
  PassDependencyLattice::iterator node, nEnd = m_Dependencies.end();
  for (node = m_Dependencies.begin(); node != nEnd; ++node) {
    if (pID == node->pass->getPassID()) { // existed
      return &*node;
    }
  }
  return nullptr;
}

unsigned int PassManager::size() const
{
  return m_AvailableAnalysis.size();
}

bool PassManager::hasAdded(Pass::AnalysisID pID) const
{
  return (m_AvailableAnalysis.end() != m_AvailableAnalysis.find(pID));
}
