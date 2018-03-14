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
  /// Add a pass by DSF order
  bool exist = false;
  const PassInfo* info = findAnalysisPassInfo(pPass->getPassID(), exist);
  if (exist) { // The pass had been added
    return;
  }

  DepNode* cur_node = m_Dependencies.addNode(pPass);
  std::stack<DepNode*> stack;
  stack.push(cur_node);

  while (!stack.empty()) {
    cur_node = stack.top();
    stack.pop();

    bool exist = false;
    const PassInfo* info = findAnalysisPassInfo(cur_node->pass->getPassID(), exist);
    if (exist) { // The pass had been added
      return;
    }

    AnalysisUsage usage;
    cur_node->pass->getAnalysisUsage(usage);
    if (usage.isEmpty()) {
      m_Dependencies.connect(*m_pStart, *cur_node);
    }
    else {
      for (AnalysisUsage::iterator use = usage.begin(); use != usage.end(); ++use) {
        exist = false;
        info = findAnalysisPassInfo(*use, exist);
        if (!exist) { // not in available analysis uet.
          // use existed node or create a new node
          DepNode* new_node = findNode(*use);
          if (nullptr == new_node) {
            Pass* new_pass = info->createPass();
            new_node = m_Dependencies.addNode(new_pass);
            stack.push(new_node);
          }
          m_Dependencies.connect(*new_node, *cur_node);
        }
      } // for each usage
      m_AvailableAnalysis[cur_node->pass->getPassID()] = cur_node;
    }
  } // leave stacking
}

bool PassManager::run(Module& pModule)
{
  PassDependencyLattice::bfs_iterator node, pEnd = m_Dependencies.bfs_end();
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    bool changed = node->pass->doInitialization(pModule);
  }

  // run the body
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    // bool changed = node->pass->run(pModule);
  }

  // finalization
  for (node = m_Dependencies.bfs_begin(); node != pEnd; node.next()) {
    bool changed = node->pass->doFinalization(pModule);
  }
}

const PassInfo*
PassManager::findAnalysisPassInfo(Pass::AnalysisID pID, bool& pExist)
{
  pExist = (m_AvailableAnalysis.end() != m_AvailableAnalysis.find(pID));
  const PassInfo* result = getPassRegistry()->getPassInfo(pID);
  if (nullptr == result)
    error(pass_not_registered) << "nullptr";
  return result;
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
