//===- PassManager.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_MANAGER_H
#define ONNC_CORE_PASS_MANAGER_H
#include <onnc/Core/Pass.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassInfo.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/IR/Module.h>
#include <onnc/ADT/Digraph.h>
#include <map>

namespace onnc {

class TargetBackend;

/** \class onnc::PassManager
 *  \brief stores a set of passes and run them.
 */
class PassManager
{
public:
  /// Use global-wise GetPassRegistry().
  /// @see PassRegistry
  PassManager();

  /// Use external PassRegistry
  PassManager(PassRegistry& pRegistry);

  /// PassManager is responsed to release all passes being added and created
  /// during adding passes.
  ~PassManager();

  void add(Pass* pPass);

  void add(Pass* pPass, TargetBackend* pBackend);

  bool run(Module& pModule);

  /// @return The number of registered passes.
  unsigned int size() const;

private:
  struct DepNode : public DigraphNode<DepNode>
  {
  public:
    DepNode(Pass* pPass) : pass(pPass) { }

    // since passes are delegated, we release memory in Digraph destruction.
    ~DepNode() { delete pass; }

  public:
    Pass *pass;
  };

  /// The start pass for lattice.
  struct StartPass : public ModulePass
  {
  public:
    static char ID;

  public:
    StartPass() : ModulePass(ID) { }

    bool runOnModule(Module &pModule) { return false; }

    StringRef getPassName() const { return "start"; }
  };

  typedef Digraph<DepNode> PassDependencyLattice;

  typedef std::map<Pass::AnalysisID, DepNode*> AvailableAnalysisMap;

private:
  PassRegistry* getPassRegistry() { return m_pPassRegistry; }

  DepNode* findNode(Pass::AnalysisID pID);

  /// @retval true If the pass @ref pID has been added.
  bool hasAdded(Pass::AnalysisID pID) const;

  void doAdd(Pass* pPass, TargetBackend* pBackend);

private:
  PassRegistry* m_pPassRegistry;
  AvailableAnalysisMap m_AvailableAnalysis;
  PassDependencyLattice m_Dependencies;
  DepNode *m_pStart;
};

} // namespace of onnc

#endif
