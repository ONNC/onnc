//===- SlabIterator.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SLAB_ITERATOR_H
#define ONNC_SUPPORT_SLAB_ITERATOR_H
#include <onnc/Support/DataTypes.h>
#include <iterator>

namespace onnc {

/** \class SlabIterator
 *
 *  SlabIterator is a bidirectional iterator.
 */
template<typename Container>
class SlabIterator
  : public std::iterator<std::bidirectional_iterator_tag,
                         typename Container::value_type>
{
public:
  typedef std::iterator<std::bidirectional_iterator_tag,
                         typename Container::value_type> super;
  typedef typename super::reference reference;
  typedef typename super::pointer   pointer;
  typedef typename super::difference_type difference_type;

  typedef typename Container::slab_type slab_type;

public:
  SlabIterator();

  SlabIterator(Container& pContainer, IListNodeBase* pNode, unsigned int pIdx);

  reference operator* () const;
  pointer   operator->() const;

  SlabIterator& operator++();
  SlabIterator& operator--();
  SlabIterator  operator++(int);
  SlabIterator  operator--(int);

  bool operator==(const SlabIterator& pOther) const;
  bool operator!=(const SlabIterator& pOther) const;

private:
  void advance() { m_pSlab = m_pSlab->getNext(); }

  void retreat() { m_pSlab = m_pSlab->getPrev(); }

  void forward();

  void backward();

  // SlabIterator is a bidirectional iterator, but it has an implicit
  // conversion to pointer-type. Therefore, declare these functions
  // (but don't implement them) as private can help users catch
  // accidental mis-use.
  void operator+(difference_type) const;
  void operator-(difference_type) const;
  void operator+=(difference_type) const;
  void operator-=(difference_type) const;
  template<class T> void operator<(T) const;
  template<class T> void operator<=(T) const;
  template<class T> void operator>(T) const;
  template<class T> void operator>=(T) const;
  template<class T> void operator-(T) const;

private:
  Container* m_pContainer;
  IListNodeBase* m_pSlab;
  unsigned int m_Idx;
};

//===----------------------------------------------------------------------===//
// SlabIterator Member Functions
//===----------------------------------------------------------------------===//
template<typename Container>
SlabIterator<Container>::SlabIterator()
  : m_pContainer(NULL), m_pSlab(NULL), m_Idx(-1) {
}

template<typename Container>
SlabIterator<Container>::SlabIterator(Container& pContainer,
                                      IListNodeBase* pSlab,
                                      unsigned int pIdx)
  : m_pContainer(&pContainer), m_pSlab(pSlab), m_Idx(pIdx) {
}

template<typename Container>
typename SlabIterator<Container>::reference
SlabIterator<Container>::operator* () const
{
  assert(NULL != m_pContainer && "Cannot derefer a NULL pointer");
  slab_type* slab = static_cast<slab_type*>(m_pSlab);
  return slab->data[m_Idx];
}

template<typename Container>
typename SlabIterator<Container>::pointer
SlabIterator<Container>::operator->() const
{
  assert(NULL != m_pContainer && "Cannot derefer a NULL pointer");
  slab_type* slab = static_cast<slab_type*>(m_pSlab);
  return (slab->data + m_Idx);
}

template<typename Container>
void SlabIterator<Container>::forward()
{
  assert(NULL != m_pContainer && "trivial iterator can not forward");

  // move one step.
  m_Idx = (m_Idx + 1) % slab_type::max_size;

  // if I'm at the rear of the container, then go to the end
  if (m_pSlab == m_pContainer->tail() &&
      m_Idx == m_pContainer->size() % slab_type::max_size) {
    advance();
    m_Idx = -1;
    return;
  }

  // otherwise, if I'm at the rear of a slab, then go to the next slab
  if (0 == m_Idx) {
    advance();
  }
}

template<typename Container>
void SlabIterator<Container>::backward()
{
  assert(NULL != m_pContainer && "trivial iterator can not backward");

  // if I'm at the front of the container, then go to the rend()
  if (m_pSlab == m_pContainer->head() && 0 == m_Idx) {
    retreat();
    m_Idx = -1;
    return;
  }

  // otherwise, if I'm at the front of a slab_type, then go to the previous slab
  if (0 == m_Idx) {
    retreat();
    m_Idx = slab_type::max_size - 1;
    return;
  }

  // the rest, move back one step.
  --m_Idx;
}

template<typename Container>
SlabIterator<Container>& SlabIterator<Container>::operator++()
{
  forward();
  return *this;
}

template<typename Container>
SlabIterator<Container>& SlabIterator<Container>::operator--()
{
  backward();
  return *this;
}

template<typename Container>
SlabIterator<Container> SlabIterator<Container>::operator++(int)
{
  SlabIterator result(*this);
  forward();
  return result;
}

template<typename Container>
SlabIterator<Container> SlabIterator<Container>::operator--(int)
{
  SlabIterator result(*this);
  backward();
  return result;
}

template<typename Container> bool
SlabIterator<Container>::operator==(const SlabIterator& pOther) const
{
  return (m_pContainer == pOther.m_pContainer &&
          m_pSlab == pOther.m_pSlab &&
          m_Idx == pOther.m_Idx);
}

template<typename Container> bool
SlabIterator<Container>::operator!=(const SlabIterator& pOther) const
{
  return !(*this == pOther);
}

} // namespace onnc

#endif
