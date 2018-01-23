//===- Slab.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// SlabIteratorBase
//===----------------------------------------------------------------------===//
template<typename Slab>
onnc::SlabIteratorBase<Slab>::SlabIteratorBase()
  : m_pSlab(NULL), m_Idx(-1) {
}

template<typename Slab>
onnc::SlabIteratorBase<Slab>::SlabIteratorBase(Slab* pSlab)
  : m_pSlab(pSlab), m_Idx(0) {
}

template<typename Slab>
void onnc::SlabIteratorBase<Slab>::goNextSlab()
{
  m_pSlab = m_pSlab->getNextNode();
}

template<typename Slab>
void onnc::SlabIteratorBase<Slab>::goNextData()
{
}
