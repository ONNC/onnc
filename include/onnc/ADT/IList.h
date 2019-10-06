//===- IList.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ILIST_H
#define ONNC_ADT_ILIST_H
#include <onnc/ADT/IListIterator.h>
#include <onnc/ADT/IListNode.h>
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Support/DataTypes.h>
#include <iterator>
#include <algorithm>
#include <cassert>

namespace onnc {

/** \class IListBase
 *  \brief IListBase provides type-independent APIs for IList.
 *
 *  A new IListBase object contains a single IListNodeBase object - sentinel.
 *  Sentinel defines the end of the list. Its @ref getNext() refers to itself.
 *  @ref head() is the first node of the list, and @ref tail() is the last node
 *  of the list. The previous node of @ref head() should be the sentinel.
 *  And the next node of @ref tail() should be the sentinel as well.
 */
class IListBase
{
protected:
  class Sentinel : public IListNodeBase
  {
  public:
    Sentinel(IListNodeBase*& pHead) : m_pHead(pHead) { }

    void setHead(IListNodeBase& pNode) { m_pHead = &pNode; }

  private:
    IListNodeBase*& m_pHead;
  };

public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

public:
  IListBase();

  virtual ~IListBase();

  /// @return The number of nodes. O(n).
  size_type size() const;

  /// @retval true There are no nodes in the list.
  bool empty() const;

  size_type max_size() const { return size_type(-1); }

  void swap(IListBase& pOther);

  /// Remove a single node from the list.
  static void remove(IListNodeBase& pWhere);

protected:
  static bool isSentinel(const IListNodeBase& pNode);

  const IListNodeBase* getSentinel() const;
  IListNodeBase*       getSentinel();

  const IListNodeBase* head() const { return m_pHead; }
  IListNodeBase* head()             { return m_pHead; }

  const IListNodeBase* tail() const { return getSentinel()->getPrev(); }
  IListNodeBase* tail()             { return getSentinel()->getPrev(); }

  void setHead(IListNodeBase* pHead) { m_pHead = pHead; }

  /// doInsert inserts @ref pNew at the front of @ref pWhere
  void doInsert(IListNodeBase& pWhere, IListNodeBase& pNew);

  /// Give up all elements in the list. Reset list to empty.
  void doReset();

  /// doConcatenate - concatenate the other list
  void doConcatenate(IListBase& pOther);

  /// No fundamental reason why IListBase can't be copyable, but the
  /// default copy/copy-assign won't do.
  IListBase(const IListBase& pCopy) = delete;
  IListBase& operator=(const IListBase& pCopy) = delete;

protected:
  mutable IListNodeBase* m_pHead;
};

/** \class IList
 *  \brief IList is an intrusive linked list.
 *
 *  Note that IList is unlike LLVM iplist:
 *  1) IList doesn't destroy elements.
 *  2) IList is not a factory. It doesn't produce any elements.
 *     That is, insert(), push() and erase() don't create new nodes.
 *  3) Fixed end(). (end() == end()) is always true, no matter the list has
 *  been modified.
 *
 *  IList doesn't have some functions, such as assign and resize, which
 *  requires the ability to produce NodeType. Its interface is not compatible
 *  to STL list and LLVM iplist.
 *
 *  Advancing end() iterator goes nowhere. That's the iterator still stays
 *  at end(). However, advancing rend() iterator goes to rbegin() and missing
 *  conditions may causes reverse_iterator loop infinitely.
 */
template<typename NodeType>
class IList : public IListBase 
{
public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef IListIterator<const NodeType> const_iterator;
  typedef IListIterator<NodeType>       iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  typedef const NodeType* const_pointer;
  typedef NodeType*       pointer;
  typedef const NodeType& const_reference;
  typedef NodeType&       reference;

public:
  IList() : IListBase() { }

  virtual ~IList() { }

  const_iterator begin() const;
  iterator       begin();
  const_iterator end()   const;
  iterator       end();

  const_reverse_iterator rbegin() const;
  reverse_iterator       rbegin();
  const_reverse_iterator rend()   const;
  reverse_iterator       rend();

  const_reference front() const;
  reference       front();
  const_reference back() const;
  reference       back();

  /// Insert element at beginning
  void push_front(NodeType* pValue);

  /// Delete first element
  void pop_front();

  /// Add element at the end
  void push_back(NodeType* pValue);

  /// Delete last element
  void pop_back();

  /// Concatenate @ref pOther after the current list, and reset @ref pOther.
  void concat(IList& pOther);

  /// Insert new elements before the element at the specified position
  iterator insert(iterator pWhere, NodeType* pValue);

  iterator erase(iterator pWhere);
  iterator erase(iterator pFirst, iterator pLast);

  /// give up the control of all elements.
  void reset();

  void clear();
};

#include "Bits/IList.tcc"

} // namespace of onnc

#endif
