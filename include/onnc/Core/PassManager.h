//===- PassManager.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_MANAGER_H
#define ONNC_CORE_PASS_MANAGER_H

#include <deque>
#include <map>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <onnc/Core/Pass.h>
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassInfo.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/IR/Module.h>
#include <onnc/ADT/Digraph.h>

namespace onnc {

class TargetBackend;

/** \class onnc::PassManager
 *  \brief stores a set of passes and run them.
 */
class PassManager
{
private:
  using PassStore = std::unordered_map<
    Pass::AnalysisID, std::vector<std::unique_ptr<Pass>>
  >;
  using LastExecuted = std::unordered_map<
    Pass::AnalysisID, Pass*
  >;

public:
  using ExecutionOrder = std::deque<Pass*>;

  /// run state
  struct State {
    ExecutionOrder execution;
    bool changed; // module changed or not
    bool executed; // current pass is executed (true) or skipped (false)
    Pass* pass; // current pass

    State() : execution(), changed(false), executed(false), pass(nullptr) { }
  };

public:
  /// Use global-wise GetPassRegistry().
  /// @see PassRegistry
  PassManager();

  /// Use external PassRegistry
  PassManager(PassRegistry& pRegistry);

  /// PassManager is responsible to release all passes being added and created
  /// during adding passes.
  ~PassManager() = default;

  /// add pPass to:
  /// 1. dependency graph (ignore if pPass.getPassID() has been added before)
  /// 2. execution queue.
  template <typename PassType, typename... Args>
  PassManager& add(Args&&... args) {
    static_assert(std::is_base_of<Pass, PassType>::value, "added type should be derived class of Pass");
    return add(new PassType(std::forward<Args>(args)...));
  }

  PassManager& add(Pass* pPass);

  PassManager& add(Pass* pPass, TargetBackend* pBackend);

  PassManager& add(Pass* pPass, State& pState);

  PassManager& add(Pass* pPass, TargetBackend* pBackend, State& pState);

  /// PassManager::run behaviour:
  /// 1. If a pass dependents on other passes (cyclic dependency is disallowed),
  ///    PassManager guarantees all dependencies are executed before that pass.
  ///
  /// 2. PassManager doesn't re-execute a pass if that pass's dependencies are
  ///    unchanged.
  ///
  /// 3. If a pass return retry, PassManager re-executes that pass, but whether
  ///    re-executes it's dependencies or not follows rule 2.

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

  bool needRun(Pass& pPass, Module& pModule);

  void initRunState(Module& pModule, State& pState);

  /// @return The number of registered passes.
  unsigned int size() const;

  const State& state() const { return m_RunState; }

  Pass* getPass(Pass::AnalysisID passId) const;

  PassRegistry* getPassRegistry() { return m_pPassRegistry; }

  void printState(const State& pState, OStream& pOS) const;

  void dumpState(const State& pState) const;

private:
  struct DepNode : public DigraphNode<DepNode>
  {
  public:
    explicit DepNode(Pass::AnalysisID passId) noexcept
      : passId{passId}
    { }

  public:
    const Pass::AnalysisID passId;
  };

  /// The start pass for lattice.
  struct StartPass : public CustomPass<StartPass>
  {
  public:
    StartPass() = default;

    Pass::ReturnType runOnModule(Module &pModule) override
    {
      return kModuleNoChanged;
    }

    StringRef getPassName() const override { return "start"; }
  };

  using DepGraph = Digraph<DepNode>;
  using DepNodes = std::unordered_map<Pass::AnalysisID, DepNode*>;

private:
  /// Dependency graph operator: find a node
  DepNode* findNode(Pass::AnalysisID pID) const;

  /// Dependency graph operator: add a node
  DepNode* addNode(Pass::AnalysisID passId);

  /// Dependency graph operator
  /// @retval true If the pass @ref pID has been added.
  bool hasAdded(Pass::AnalysisID pID) const;

  /// Dependency graph operator
  /// Add a pass to internal dependency graph. Pass is added in DSF order.
  ///
  /// @note The function can be called multiple times with the same pPass
  ///       without side effect.
  void addPassToDependencyGraph(Pass::AnalysisID passId, TargetBackend* pBackend);

  /// Move ownership of the parameter pass into this
  void movePassToStore(Pass* pass);

  /// Run the pass
  Pass::ReturnType doRun(Pass& pPass, Module& pModule);

  void UpdateExecutionOrder(ExecutionOrder& pOrder);

  /// Add a pass to execution queue. If a pass depends on other passes, the
  /// dependent passes are added to exe queue unconditionally.
  void addPassToExeQueue(Pass* pPass, State& pState);

  bool hasLastExecuted(Pass::AnalysisID passId) const;

  Pass* getLastExecuted(Pass::AnalysisID passId) const;

  void setLastExecuted(Pass& pass);

  Pass* getLastExecutedOrFromStore(Pass::AnalysisID passId) const;

private:
  PassRegistry* m_pPassRegistry;

  // a graph describes the dependencies among passes.
  DepGraph m_depGraph;

  // A map from ID to node in dependency graph.
  DepNodes m_depNodes;

  LastExecuted m_lastExecuted;

  PassStore m_passStore;

  State m_RunState;

  DepNode *m_pStart;

  // Executing time step, it is reset on initRunState. PassManager uses time
  // step to decide whether to execute a pass or not. If PassManager execute a
  // pass, it also updates Pass' time step.
  unsigned m_TimeStep;
};

} // namespace of onnc

#endif
