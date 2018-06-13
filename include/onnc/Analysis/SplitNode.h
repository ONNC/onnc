//===- SplitNode.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SPLIT_NODE_H
#define ONNC_SPLIT_NODE_H
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <unordered_map>

namespace onnc {

using ValMemSizeMap = std::unordered_map<const onnx::Value *, MemSize>;

/** \class SplitNode
 *  Represent a node, encapsulate the knowledge of inferring new input sizes
 *  based on new output sizes.
 */
class SplitNode
{
friend class SplitNodeManager;

public:
  SplitNode(onnx::Node& pN);

  virtual ~SplitNode() {}

  virtual bool useNewOutSize(const LongInts& pNewOutSize);

  /// Calculate required input size based on new output size.
  /// Many operators have input size equal to output size.
  ///
  /// @param pIdx Which input.
  virtual LongInts calNewInputSize(unsigned pIdx) const;

  virtual LongInts getNewOutputSize(unsigned pIdx) const;

  onnx::NodeKind kind() const { return m_Node.kind(); }

  void resetSize() { m_NewOutSizes = m_OutSizes; }

  const onnx::Node &getNode() const { return m_Node; }

  onnx::Node &getNode() { return m_Node; }

protected:
  LongInts m_NewOutSizes;
  const LongInts m_OutSizes;

  onnx::Node& m_Node;
};

class SplitNodeManager;
class SplitGroup
{
friend class SplitNodeManager;
public:
  SplitGroup(SplitNodeManager &pSnMgr)
    : m_SnMgr(pSnMgr) {}

  void resetToOrigSize();

  void getMemUsage(const TargetTransformInfo *pTTI, ValMemSizeMap &pVMSMap);

  /// Reduce size of all values in a group to meet memory size constraint.
  void shrinkSize();

private:
  SplitNodeManager &m_SnMgr;

  // split parameters.
  unsigned m_CurSplitAxis;
  unsigned m_CurSplitFactor;
};

/** \class SplitNodeManager
 *  Do splitting on SplitNode in backward direction, and provide splitting
 *  result which can be used in memory allocation.
 */
class SplitNodeManager
{
public:
  typedef std::vector<SplitGroup*> SplitGroups;
  typedef std::unordered_map<onnx::Node*, SplitNode*> SplitInfoHash;

  SplitNodeManager(onnx::Graph& pGraph, DLATargetBackend& pDLATB);
  ~SplitNodeManager();

  /// @param pN Split from node pN.
  /// @param pUpdateUpper Propagate new size to upper levels.
  void splitNodeByFactor(onnx::Node* pN, unsigned pAxis, unsigned pFactor,
                         bool pUpdateUpper = true);

  bool splitNodeBySize(onnx::Node* pN, const LongInts& pNewOutSize,
                       bool pUpdateUpper = true);

  SplitGroup *createNewGroup();

  SplitGroups &getGroups() { return m_Groups; }

  SplitNode* getSplitNode(onnx::Node* pN);

  const SplitNode* getSplitNode(onnx::Node* pN) const;

  bool hasSplitNode(onnx::Node *pN) const;

  onnx::Graph *splitGraph(onnx::Graph &pGraph);

  /// Dump splitting result. Callable in GDB.
  void dump() const;

  void print(OStream &pOS) const;

private:
  void clear();

  DLATargetBackend& m_DLATB;
  onnx::Graph& m_Graph;
  SplitInfoHash m_SplitInfos;

  /// Store means local memory spilling to global memory. Each group can have
  /// n stores, and each group forms a subgraph. Communication (data exchange)
  /// between groups (subgraph) is through load/store.
  ///
  /// Graph splitting and get memory usages are operating on a group.
  SplitGroups m_Groups;
};

} // namespace of onnc

#endif
