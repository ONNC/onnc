//===- GCFactory.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_GCFACTORY_H
#define ONNC_SUPPORT_GCFACTORY_H
#include <onnc/Support/LinearAllocator.h>
#include <onnc/ADT/TypeTraits.h>

#include <assert.h>
#include <cstddef>
#include <iterator>

namespace onnc {

/** \class DataIteratorBase
 *  \brief DataIteratorBase provides the basic functions of DataIterator
 *  @see DataIterator
 */
template <typename ChunkType>
struct DataIteratorBase {
 public:
  ChunkType* m_pChunk;
  unsigned int m_Pos;

 public:
  DataIteratorBase(ChunkType* X, unsigned int pPos)
      : m_pChunk(X), m_Pos(pPos) {}

  inline void advance() {
    ++m_Pos;
    if ((m_Pos == m_pChunk->bound) && (0 == m_pChunk->next))
      return;
    if (m_Pos == m_pChunk->bound) {
      m_pChunk = m_pChunk->next;
      m_Pos = 0;
    }
  }

  bool operator==(const DataIteratorBase& y) const {
    return ((this->m_pChunk == y.m_pChunk) && (this->m_Pos == y.m_Pos));
  }

  bool operator!=(const DataIteratorBase& y) const {
    return ((this->m_pChunk != y.m_pChunk) || (this->m_Pos != y.m_Pos));
  }
};

/** \class DataIterator
 *  \brief DataIterator provides STL compatible iterator for allocators
 */
template <typename ChunkType, class Traits>
class DataIterator : public DataIteratorBase<ChunkType> {
 public:
  typedef typename ChunkType::value_type value_type;
  typedef Traits traits;
  typedef typename traits::pointer pointer;
  typedef typename traits::reference reference;
  typedef DataIterator<ChunkType, Traits> Self;
  typedef DataIteratorBase<ChunkType> Base;

  typedef typename traits::nonconst_traits nonconst_traits;
  typedef DataIterator<ChunkType, nonconst_traits> iterator;
  typedef typename traits::const_traits const_traits;
  typedef DataIterator<ChunkType, const_traits> const_iterator;
  typedef std::forward_iterator_tag iterator_category;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 public:
  DataIterator() : Base(NULL, 0) {}

  DataIterator(ChunkType* pChunk, unsigned int pPos) : Base(pChunk, pPos) {}

  DataIterator(const DataIterator& pCopy) : Base(pCopy.m_pChunk, pCopy.m_Pos) {}

  ~DataIterator() {}

  // -----  operators  ----- //
  reference operator*() {
    assert(this->m_pChunk != NULL &&
           "data iterator goes to a invalid position");
    return this->m_pChunk->data[Base::m_Pos];
  }

  Self& operator++() {
    this->Base::advance();
    return *this;
  }

  Self operator++(int) {
    Self tmp = *this;
    this->Base::advance();
    return tmp;
  }
};

template <typename Alloc>
class GCFactoryBase : public Alloc {
 public:
  typedef DataIterator<typename Alloc::chunk_type,
                       NonConstTraits<typename Alloc::value_type> > iterator;
  typedef DataIterator<typename Alloc::chunk_type,
                       ConstTraits<typename Alloc::value_type> > const_iterator;

  typedef typename Alloc::value_type value_type;
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::reference reference;
  typedef typename Alloc::size_type size_type;

 protected:
  GCFactoryBase() : Alloc(), m_NumAllocData(0) {}

  explicit GCFactoryBase(size_t pNum) : Alloc(pNum), m_NumAllocData(0) {}

 public:
  virtual ~GCFactoryBase() { Alloc::clear(); }

  // -----  modifiers  ----- //
  value_type* allocate(size_t N) {
    value_type* result = Alloc::allocate(N);
    if (result != NULL)
      m_NumAllocData += N;
    return result;
  }

  value_type* allocate() {
    ++m_NumAllocData;
    return Alloc::allocate();
  }

  void deallocate(pointer& pPtr, size_type N) {
    Alloc::deallocate(pPtr, N);
    if (pPtr == NULL)
      m_NumAllocData -= N;
  }

  void deallocate(pointer& pPtr) {
    Alloc::deallocate(pPtr);
    if (pPtr == NULL)
      --m_NumAllocData;
  }

  void reset() {
    Alloc::reset();
    m_NumAllocData = 0;
  }

  // -----  iterators  ----- //
  iterator begin() { return iterator(Alloc::m_pRoot, 0); }

  const_iterator begin() const { return const_iterator(Alloc::m_pRoot, 0); }

  iterator end() {
    return (Alloc::m_pCurrent) == 0
               ? begin()
               : iterator(Alloc::m_pCurrent, Alloc::m_pCurrent->bound);
  }

  const_iterator end() const {
    return (Alloc::m_pCurrent) == 0
               ? begin()
               : const_iterator(Alloc::m_pCurrent, Alloc::m_pCurrent->bound);
  }

  // -----  observers  ----- //
  bool empty() const { return Alloc::empty(); }

  unsigned int capacity() const { return Alloc::max_size(); }

  unsigned int size() const { return m_NumAllocData; }

 protected:
  unsigned int m_NumAllocData;
};

/** \class GCFactory
 *  \brief GCFactory provides a factory that guarantees to remove all allocated
 *  data.
 */
template <typename DataType, size_t ChunkSize>
class GCFactory : public GCFactoryBase<LinearAllocator<DataType, ChunkSize> > {
 public:
  GCFactory() : GCFactoryBase<LinearAllocator<DataType, ChunkSize> >() {}
};

template <typename DataType>
class GCFactory<DataType, 0>
    : public GCFactoryBase<LinearAllocator<DataType, 0> > {
 public:
  explicit GCFactory(size_t pNum)
     : GCFactoryBase<LinearAllocator<DataType, 0> >(pNum) {}
};

}  // namespace onnc

#endif  // ONNC_SUPPORT_GCFACTORY_H
