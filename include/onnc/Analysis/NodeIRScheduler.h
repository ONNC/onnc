//===- NodeIRScheduler.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements IR scheduler, especially focus on reorder load/store to
// appropriate position for max performance.
//
//===----------------------------------------------------------------------===//
#ifndef NODE_IR_SCHEDULER_H
#define NODE_IR_SCHEDULER_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace onnc {

using Nodes = std::vector<onnx::Node *>;
using Ints = std::vector<int>;

/** \class NodeIRScheduler
 *  \brief onnx Graph IR scheduler. Reorder IR, especially load/store for
 *         better, even maximum performance.
 */
class NodeIRScheduler : public ModulePass
{
public:
  struct ExeResUser
  {
    onnx::Node *user;
    unsigned remainCycles;  /// Remaining cycles to finish executing the user.

    ExeResUser(onnx::Node *user, unsigned rcycles)
      : user(user), remainCycles(rcycles) {}
  };

  // <execution resource, list of <user, remaining cycles>>
  using ExeResUserMap = std::unordered_map<const ExeResource*,
                                           std::vector<ExeResUser>>;
public:
  static char ID;

  virtual ~NodeIRScheduler();

public:
  NodeIRScheduler(DLATargetBackend* pDLATB = nullptr);

  ReturnType runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  void print(std::ostream& pOS) const;

private:
  Nodes greedyPickNextNodes(Nodes &pCands);

  bool isExeResAvailable(const ExeResource *pExeRes) const;

  void addExeResUser(const ExeResource *pExeRes, onnx::Node *pUser);

  Nodes issue();

  void clear() { m_NewIROrder.clear(); }

private:
  DLATargetBackend* m_DLATB;
  Nodes m_NewIROrder;
  ExeResUserMap m_ExeResUsers;
};

NodeIRScheduler *CreateNodeIRSchedulerPass(DLATargetBackend *pDLATB);

}  // namespace of onnc

#endif  // NODE_IR_SCHEDULER_H
