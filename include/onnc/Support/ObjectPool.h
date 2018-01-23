//===- ObjectPool.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OBJECT_POOL_H
#define ONNC_SUPPORT_OBJECT_POOL_H
#include <onnc/ADT/IList.h>
#include <onnc/Support/Slab.h>

namespace onnc {

/** \class ObjectPool
 *  \brief The ObjecPool class provides a bump allocator that has the ability
 *  to deallocate objects.
 *
 *  The ObjectPool uses first-match algorithm to allocate/deallocate objects.
 *
 *  @note If you concern about the memory size, use MemoryPool instead.
 */
template<typename DataType, unsigned int Amount = 64>
class ObjectPool : public IListBase
{
private:
  struct Entry
  {
  public:
    bool used;
    DataType value;

  public:
    Entry()
      : used(false), value() {
    }
  };

public:
  typedef const DataType* const_pointer;
  typedef const DataType& const_reference;
  typedef DataType* pointer;
  typedef DataType& reference;

  typedef Slab<Entry, Amount> slab_type;
  typedef unsigned int size_type;

public:
  ObjectPool();

  ~ObjectPool();

  /// Allocate uninitialized storage. Time complexity is O(n) by linear searching.
  /// @retval nullptr Fail to allocate.
  pointer allocate();

  /// Deallocate storage, so that space could be reused by later allocation.
  void deallocate(pointer pPtr);

  /// Call the (default) constructor of @ref DataType.
  void construct(pointer pPtr) const;

  /// Call the destructor of @ref DataType.
  void destroy(pointer pPtr) const;

  /// @return The number of allocated objects.
  unsigned int size() const { return m_Size; }

  /// @retval true The pool is empty
  bool empty() const { return (0 == m_Size); }

protected:
  /// Delete all slabs.
  void clear();

protected:
  unsigned int m_Size;

private:
  /// @retval true pPtr is not in pSlab.
  static bool InSlab(slab_type* pSlab, pointer pPtr);

  using IListBase::size;

  using IListBase::empty;
};

} // namespace of onnc

#include <onnc/Support/Bits/ObjectPool.tcc>

#endif
