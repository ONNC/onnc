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
    m_Dependencies(), m_AvailableAnalysis(),
    m_pStart(m_Dependencies.addNode(new StartPass())) {
}

PassManager::PassManager(PassRegistry& pRegistry)
  : m_pPassRegistry(&pRegistry),
    m_Dependencies(), m_AvailableAnalysis(),
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

bool PassManager::run(Module& pModule)
{
  /// TODO: If a pass return kPassRetry, run all dependent passes again until success.
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
