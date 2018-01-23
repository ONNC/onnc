//===- ManagedStatic.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/ManagedStatic.h>
#include <cassert>

using namespace onnc;

static const ManagedStaticBase *StaticList = NULL;

//===----------------------------------------------------------------------===//
// ManagedStaticBase
//===----------------------------------------------------------------------===//
void ManagedStaticBase::RegisterManagedStatic(void *(*pCreator)(),
                                              void (*pDeleter)(void*)) const
{
  assert(NULL == m_Ptr && NULL == m_pDeleter && m_pNext == 0 &&
         "Partially initialized ManagedStatic!?");
  m_Ptr = pCreator ? pCreator() : NULL;
  m_pDeleter = pDeleter;

  // Add to list of managed statics.
  m_pNext = StaticList;
  StaticList = this;
}

void ManagedStaticBase::destroy() const
{
  assert(m_pDeleter && "ManagedStatic not initialized correctly!");
  assert(StaticList == this &&
         "Not destroyed in reverse order of construction?");
  // Unlink from list.
  StaticList = m_pNext;
  m_pNext = NULL;

  // Destroy memory.
  m_pDeleter(m_Ptr);

  // Cleanup.
  m_Ptr = NULL;
  m_pDeleter = NULL;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
void onnc::shutdown()
{
  while (StaticList)
    StaticList->destroy();
}
