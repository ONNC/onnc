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
  typedef std::deque<Pass::AnalysisID> ExecutionOrder;

  /// run state
  struct State {
    ExecutionOrder execution;
    bool changed; // module changed or not
    Pass* pass; // current pass

    State() : execution(), changed(false), pass(nullptr) { }
  };

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

  void add(Pass* pPass, State& pState);

  void add(Pass* pPass, TargetBackend* pBackend, State& pState);

  /// run all passes
  /// @retval false A pass return failure.
  bool run(Module& pModule);

  /// run one pass
  /// @return The pass run
  bool step(Module& pModule);

  /// continue to run all passes
  bool run(Module& pModule, State& pState);

  /// run one pass
  /// @return The pass run
  bool step(Module& pModule, State& pState);

  /// @return The number of registered passes.
  unsigned int size() const;

  const State& state() const { return m_RunState; }

  Pass* lookup(Pass::AnalysisID pID);

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

    Pass::ReturnType runOnModule(Module &pModule) { return kModuleNoChanged; }

    StringRef getPassName() const { return "start"; }
  };

  typedef Digraph<DepNode> PassDependencyLattice;

  typedef std::map<Pass::AnalysisID, DepNode*> AvailableAnalysisMap;

private:
  PassRegistry* getPassRegistry() { return m_pPassRegistry; }

  /// Dependency graph operator: find a node
  DepNode* findNode(Pass::AnalysisID pID);

  /// Dependency graph operator: add a node
  DepNode* addNode(Pass& pPass);

  /// Dependency graph operator
  /// @retval true If the pass @ref pID has been added.
  bool hasAdded(Pass::AnalysisID pID) const;

  void doAdd(Pass* pPass, TargetBackend* pBackend, State& pState);

  /// Run the pass
  Pass::ReturnType doRun(Pass& pPass, Module& pModule);

  void UpdateExecutionOrder(ExecutionOrder& pOrder);

private:
  PassRegistry* m_pPassRegistry;

  // a graph describes the dependencies among passes.
  PassDependencyLattice m_Dependencies;

  // A map from ID to node in dependency graph.
  AvailableAnalysisMap m_AvailableAnalysis;

  State m_RunState;

  DepNode *m_pStart;
};

} // namespace of onnc

#endif
