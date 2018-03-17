//===- BinaryTree.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BINARY_TREE_H
#define ONNC_ADT_BINARY_TREE_H
#include <onnc/ADT/Bits/TreeNode.h>
#include <onnc/ADT/Bits/TreeNodeAllocator.h>
#include <onnc/ADT/Bits/TreeIterator.h>
#include <onnc/ADT/Uncopyable.h>
#include <cstddef>

namespace onnc {

/** \class BinaryTreeBase
 *  \brief BinaryTreeBase gives root node and memory management.
 *
 *  The memory management of nodes in is hidden by BinaryTreeBase.
 *  BinaryTreeBase also provides the basic functions for merging a tree and
 *  inserton of a node.
 *
 *  @see BinaryTree
 */
template<class DataType>
class BinaryTreeBase : private Uncopyable
{
public:
  typedef bintree::Node<DataType> NodeType;

protected:
  /// TreeImpl - TreeImpl records the root node and the number of nodes
  //
  //    +---> Root(end) <---+
  //    |        |left      |
  //    |      begin        |
  //    |     /     \       |
  //    |  Left     Right   |
  //    +---/         \-----+
  //
  class TreeImpl : public bintree::TreeNodeAllocator<NodeType>
  {
  private:
    typedef typename bintree::TreeNodeAllocator<NodeType>::const_iterator const_iterator;
    typedef typename bintree::TreeNodeAllocator<NodeType>::iterator       iterator;

  public:
    bintree::NodeBase node;

  public:
    TreeImpl()
      : bintree::TreeNodeAllocator<NodeType>() {
      node.left = node.right = &node;
    }

    /// summon - let leaves of pClient to connect to our root
    void summon(TreeImpl& pClient) {
      if (this == &pClient)
        return;

      iterator data;
      iterator dEnd = pClient.end();
      for (data = pClient.begin(); data!=dEnd; ++data ) {
        if ((*data).left == &pClient.node)
          (*data).left = &node;
        if ((*data).right == &pClient.node)
          (*data).right = &node;
      }
    }
  }; // TreeImpl

protected:
  /// m_Root is a special object who responses:
  //  - the pointer of root
  //  - the simple factory of nodes.
  TreeImpl m_Root;

protected:
  NodeType *createNode() {
    NodeType *result = m_Root.produce();
    result->left = result->right = &m_Root.node;
    return result;
  }

  void destroyNode(NodeType *pNode) {
    pNode->left = pNode->right = 0;
    pNode->data = 0;
    m_Root.deallocate(pNode);
  }

public:
  BinaryTreeBase() : m_Root() { }

  size_t size() const { return m_Root.size(); }

  bool empty() const { return m_Root.empty(); }

protected:
  void clear() { m_Root.clear(); }
};

/** \class BinaryTree
 *  \brief An abstract data type of binary tree.
 *
 *  @see mcld::InputTree
 */
template<class DataType>
class BinaryTree : public BinaryTreeBase<DataType>
{
public:
  typedef size_t             size_type;
  typedef ptrdiff_t          difference_type;
  typedef DataType           value_type;
  typedef value_type*        pointer;
  typedef value_type&        reference;
  typedef const value_type*  const_pointer;
  typedef const value_type&  const_reference;

  typedef BinaryTree<DataType>  Self;

  typedef bintree::TreeIterator<NonConstTraits<DataType> > iterator;
  typedef bintree::TreeIterator<ConstTraits<DataType> >    const_iterator;

  typedef bintree::PolicyIterator<bintree::DFSIterator, ConstTraits<DataType> >    const_dfs_iterator;
  typedef bintree::PolicyIterator<bintree::DFSIterator, NonConstTraits<DataType> > dfs_iterator;

  typedef bintree::PolicyIterator<bintree::BFSIterator, ConstTraits<DataType> >    const_bfs_iterator;
  typedef bintree::PolicyIterator<bintree::BFSIterator, NonConstTraits<DataType> > bfs_iterator;

protected:
  typedef bintree::Node<value_type> node_type;

public:
  BinaryTree() : BinaryTreeBase<DataType>() { }

  ~BinaryTree() { }

  bfs_iterator bfs_begin()
  { return bfs_iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  bfs_iterator bfs_end()
  { return bfs_iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  const_bfs_iterator bfs_begin() const
  { return const_bfs_iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  const_bfs_iterator bfs_end() const
  { return const_bfs_iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  dfs_iterator dfs_begin()
  { return dfs_iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  dfs_iterator dfs_end()
  { return dfs_iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  const_dfs_iterator dfs_begin() const
  { return const_dfs_iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  const_dfs_iterator dfs_end() const
  { return const_dfs_iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  iterator root()
  { return iterator(&(BinaryTreeBase<DataType>::m_Root.node)); }

  const_iterator root() const
  { return const_iterator(&(BinaryTreeBase<DataType>::m_Root.node)); }

  iterator begin()
  { return iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  iterator end()
  { return iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  const_iterator begin() const
  { return const_iterator(BinaryTreeBase<DataType>::m_Root.node.left); }

  const_iterator end() const
  { return const_iterator(BinaryTreeBase<DataType>::m_Root.node.right); }

  /// join - create a leaf node and merge it in the tree.
  //  This version of join determines the direction on compilation time.
  //  @param DIRECT the direction of the connecting edge of the parent node.
  template<size_t DIRECT>
  BinaryTree& join(bintree::TreeIteratorBase& pPosition, DataType& pValue)
  {
    node_type *node = BinaryTreeBase<DataType>::createNode();
    node->data = &pValue;

    if (pPosition.isRoot())
      pPosition.hook<bintree::TreeIteratorBase::Leftward>(node);
    else
      pPosition.hook<DIRECT>(node);

    return *this;
  }

  /// merge - merge the tree
  //  @param DIRECT the direction of the connecting edge of the parent node.
  //  @param position the parent node
  //  @param the tree being joined.
  //  @return the joined tree
  template<size_t DIRECT>
  BinaryTree& merge(bintree::TreeIteratorBase& pPosition, BinaryTree& pOther)
  {
    if (this == &pOther)
      return *this;

    if (!pOther.empty()) {
      pPosition.hook<DIRECT>(pOther.m_Root.node.left);
      BinaryTreeBase<DataType>::m_Root.summon(
                                   pOther.BinaryTreeBase<DataType>::m_Root);
      BinaryTreeBase<DataType>::m_Root.delegate(pOther.m_Root);
      pOther.m_Root.node.left = pOther.m_Root.node.right = &pOther.m_Root.node;
    }
    return *this;
  }
};

} // namespace of onnc

#endif
