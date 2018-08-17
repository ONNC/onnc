//===- MemoryPool.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_MEMORY_POOL_H
#define ONNC_ADT_MEMORY_POOL_H
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Support/MemoryAllocator.h>
#include <onnc/Support/SlabIterator.h>

#include <cassert>
#include <iterator>

namespace onnc {

/** \class MemoryPool
 *  \brief MemoryPool provides a factory that guarantees to remove all allocated
 *  data.
 *
 *  In particular, MemoryPool just keeps allocating memory and never deletes it
 *  until an entire block is dead. This makes allocation fast, but must be used
 *  merely when the trade-off is considered.
 *
 *  The concept of MemoryPool is a simple implementation of boost::pool.
 *
 *  This class obeys allocator requirements of C++ standard.
 *  <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3690.pdf">
 *  17.6.3.5 Allocator requirements, C++ 11 n3690</a>
 *
 *  @tparam DataType - the type of elements.
 *  @tparam Amount   - the number of elements in a slab.
 */
template<typename DataType, unsigned int Amount = 64>
class MemoryPool : public MemoryAllocator<DataType, Amount>
{
public:
  typedef MemoryAllocator<DataType, Amount> Alloc;

  typedef DataType        value_type;
  typedef const DataType* const_pointer;
  typedef DataType*       pointer;
  typedef const DataType& const_reference;
  typedef DataType&       reference;

  typedef typename Alloc::slab_type slab_type;

  typedef SlabIterator<MemoryPool> iterator;
  typedef SlabIterator<const MemoryPool> const_iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  template<typename NewDataType>
  struct rebind {
    typedef MemoryPool<NewDataType, Amount> other;
  };

public:
  MemoryPool();

  virtual ~MemoryPool();

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

  /// clear - erase all elements
  void clear();

  using Alloc::empty;
  using Alloc::size;
  using Alloc::max_size;

  using Alloc::head;
  using Alloc::tail;

protected:
  using Alloc::getSentinel;

};

//===----------------------------------------------------------------------===//
// MemoryPool
//===----------------------------------------------------------------------===//
template<typename DataType, unsigned int Amount>
MemoryPool<DataType, Amount>::MemoryPool()
  : Alloc() {
}

template<typename DataType, unsigned int Amount>
MemoryPool<DataType, Amount>::~MemoryPool()
{
  clear();
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_iterator
MemoryPool<DataType, Amount>::begin() const
{
  return const_iterator(*this, Alloc::head(), 0);
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::iterator
MemoryPool<DataType, Amount>::begin()
{
  return iterator(*this, Alloc::head(), 0);
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_iterator
MemoryPool<DataType, Amount>::end() const
{
  return const_iterator(*this, Alloc::getSentinel(), -1);
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::iterator
MemoryPool<DataType, Amount>::end()
{
  return iterator(*this, Alloc::getSentinel(), -1);
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_reverse_iterator
MemoryPool<DataType, Amount>::rbegin() const
{
  return const_reverse_iterator(end());
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::reverse_iterator
MemoryPool<DataType, Amount>::rbegin()
{
  return reverse_iterator(end());
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_reverse_iterator
MemoryPool<DataType, Amount>::rend() const
{
  return const_reverse_iterator(begin());
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::reverse_iterator
MemoryPool<DataType, Amount>::rend()
{
  return reverse_iterator(begin());
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_reference
MemoryPool<DataType, Amount>::front() const
{
  assert(!empty() && "Calling front() on empty memory pool");
  return static_cast<slab_type*>(Alloc::head())->data[0];
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::reference
MemoryPool<DataType, Amount>::front()
{
  assert(!empty() && "Calling front() on empty memory pool");
  return static_cast<slab_type*>(Alloc::head())->data[0];
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::const_reference
MemoryPool<DataType, Amount>::back() const
{
  assert(!empty() && "Calling back() on empty memory pool");
  unsigned int back = (size()-1) % Amount;
  return static_cast<slab_type*>(Alloc::getSentinel()->getPrev())->data[back];
}

template<typename DataType, unsigned int Amount>
typename MemoryPool<DataType, Amount>::reference
MemoryPool<DataType, Amount>::back()
{
  assert(!empty() && "Calling back() on empty memory pool");
  unsigned int back = (size()-1) % Amount;
  return static_cast<slab_type*>(Alloc::getSentinel()->getPrev())->data[back];
}

template<typename DataType, unsigned int Amount>
void MemoryPool<DataType, Amount>::clear()
{
  IListNodeBase* cur = head();
  IListNodeBase* sentinel = Alloc::getSentinel();
  while (cur != sentinel) {
    IListNodeBase* next = cur->getNext();
    Alloc::remove(*cur);
    delete cur;
    cur = next;
  }

  // The last node is sentinel. Do not delete it.
  Alloc::doReset();
}

} // namespace of onnc

#endif
