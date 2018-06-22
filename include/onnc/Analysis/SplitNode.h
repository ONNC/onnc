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

typedef std::unordered_map<const onnx::Value *, MemSize> ValMemSizeMap;

/** \class SplitNode
 *  Represent a node, encapsulate the knowledge of inferring new input sizes
 *  based on new output sizes.
 */
class SplitNode
{
public:
  SplitNode(onnx::Node& pN, bool pSizeDecideByUser = false);

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

  /// Should SplitGraph skip this node when calculating memory usage?
  /// For example:
  //  If this is Load IR, SplitGraph should skip it since loading size of Load
  /// IR is decided by user IR (e.g. Gemm).
  ///
  /// If this is Store IR, SplitGraph should skip it since store size has
  /// calculated by parent node.
  bool skipWhenCalMemSize() const { return m_SizeCalByOtherNode; }

protected:
  LongInts m_NewOutSizes;
  const LongInts m_OutSizes;

  /// The input and output size is calculated by other nodes.
  bool m_SizeCalByOtherNode;
  onnx::Node& m_Node;
};

class SplitGraphManager;
class SplitGraph
{
public:
  typedef std::unordered_map<onnx::Node*, SplitNode*> SplitNodeHash;

  SplitGraph(SplitGraphManager &pSgMgr, onnx::Graph &pGraph);

  ~SplitGraph();

  void resetToOrigSize();

  void getMemUsage(ValMemSizeMap &pVMSMap) const;

  /// Reduce size of all values in a group to meet memory size constraint.
  void shrinkSize();

  onnx::Graph & getGraph() { return m_Graph; }

  SplitNode* getSplitNode(onnx::Node* pN);

  const SplitNode* getSplitNode(onnx::Node* pN) const;

  bool hasSplitNode(onnx::Node *pN) const;

  void rebuildSplitNodes();

  void setAllocStatus(bool success, uint64_t size);

  void print(OStream &pOS) const;

private:
  void clear();

  /// @param pN Split from node pN.
  /// @param pUpdateUpper Propagate new size to upper levels.
  void splitNodeByFactor(onnx::Node* pN, unsigned pAxis, unsigned pFactor,
                         bool pUpdateUpper = true);

  bool splitNodeBySize(onnx::Node* pN, const LongInts& pNewOutSize,
                       bool pUpdateUpper = true);

private:
  SplitGraphManager &m_SgMgr;

  onnx::Graph &m_Graph;

  SplitNodeHash m_SplitNodes;

  /// split parameters for each output value.
  std::vector<onnx::Node *> m_Stores;
  std::vector<unsigned> m_CurSplitAxis;
  std::vector<unsigned> m_CurSplitFactor;

  /// Allocation status
  bool m_AllocSuccess;
  uint64_t m_AllocSize;
};

/** \class SplitGraphManager
 *  Do splitting on SplitNode in backward direction, and provide splitting
 *  result which can be used in memory allocation.
 */
class SplitGraphManager
{
public:
  typedef std::vector<SplitGraph*> SplitGraphs;

  SplitGraphManager(onnx::Graph& pGraph, DLATargetBackend& pDLATB);
  ~SplitGraphManager();

  SplitGraphs &getSplitGraphs() { return m_SubGraphs; }

  SplitGraph *splitNewSubGraph(SplitGraph &pSpGraph);

  /// Dump splitting result. Callable in GDB.
  void dump() const;

  void print(OStream &pOS) const;

  const TargetTransformInfo &getTTI() const { return *m_DLATB.getTTI(); }

private:
  void clear();

  DLATargetBackend& m_DLATB;

  /// Communication (data exchange) between subgraphs is through load/store.
  SplitGraphs m_SubGraphs;
};

} // namespace of onnc

#endif
