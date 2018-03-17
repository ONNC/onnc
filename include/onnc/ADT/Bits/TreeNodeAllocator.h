//===- TreeNodeAllocator.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BITS_TREE_NODE_ALLOCATOR_H
#define ONNC_ADT_BITS_TREE_NODE_ALLOCATOR_H
#include <onnc/Support/MemoryPool.h>
#include <onnc/ADT/Bits/TreeNode.h>

namespace onnc {
namespace bintree {

/** \class TreeNodeAllocator
 *  \brief TreeNodeAllocator manages onnc::Node.
 *
 *  TreeNodeAllocator guarantees all allocated memory are released finally. When
 *  the destructor of TreeNodeAllocator is called, all allocated memory are freed.
 *
 *  TreeNodeAllocator provides delegation of memory for merge two trees.
 *  We have to merge two TreeNodeAllocatos while merging trees, and
 *  TreeNodeAllocator::delegate() can move the memory from one to another.
 *
 *  @see MemoryAllocator
 */
template<typename NodeType>
class TreeNodeAllocator : public MemoryPool<NodeType>
{
protected:
  typedef MemoryPool<NodeType> Alloc;

public:
  typedef typename Alloc::iterator       iterator;
  typedef typename Alloc::const_iterator const_iterator;

public:
  /// produce - produce a node, add it under control
  NodeType* produce() {
    NodeType* result = Alloc::allocate();
    new (result) NodeType();
    return result;
  }

  /// delegate - get the control of chunks owned by the client
  //  after calling delegate(), client will renouce its control
  //  of memory space.
  void delegate(TreeNodeAllocator& pClient) { Alloc::doConcatenate(pClient); }
};

} // namespace of bintree
} // namespace of onnc

#endif
