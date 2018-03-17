//===- TreeNode.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BITS_TREE_NODE_H
#define ONNC_ADT_BITS_TREE_NODE_H
#include <onnc/ADT/TypeTraits.h>
#include <cassert>
#include <iterator>
#include <cstddef>

namespace onnc {
namespace bintree {

/** \class NodeBase
 *  \brief As the based class, NodeBase provides linkages for a node.
 */
struct NodeBase
{
public:
  NodeBase *left;
  NodeBase *right;

public:
  NodeBase() : left(NULL), right(NULL) { }
};

/** \class Node
 *  \brief A node of a binary tree.
 *  The owner of data. Inherit from NodeBase, thus, has linkages.
 */
template<typename DataType>
struct Node : public NodeBase
{
public:
  typedef DataType        value_type;

public:
  DataType* data;

public:
  Node() : NodeBase(), data(NULL) { }

  Node(const DataType& pValue) : NodeBase(), data(&pValue) { }
};

} // namespace of bintree
} // namespace of onnc 

#endif
