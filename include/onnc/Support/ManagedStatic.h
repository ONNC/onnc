//===- ManagedStatic.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_MANAGED_STATIC_H
#define ONNC_SUPPORT_MANAGED_STATIC_H
#include <onnc/ADT/Uncopyable.h>
#include <onnc/Support/DataTypes.h>

namespace onnc {

template<class C>
void* object_creator() {
  return new C();
}

template<typename T> struct object_deleter {
  static void call(void * Ptr) { delete (T*)Ptr; }
};

/// ManagedStaticBase - Common base class for ManagedStatic instances.
class ManagedStaticBase
{
public:
  bool isConstructed() const { return (NULL != m_Ptr); }

  void destroy() const;

protected:
  typedef void (*DeleterFuncType)(void*);

protected:
  // This should only be used as a static variable, which guarantees that this
  // will be zero initialized.
  mutable void* m_Ptr;
  mutable DeleterFuncType m_pDeleter;
  mutable const ManagedStaticBase* m_pNext;

  void RegisterManagedStatic(void *(*creator)(), void (*deleter)(void*)) const;

};

/** \class ManagedStatic
 *  \brief ManagedStatic changes the behavior of global static variables to
 *  be lazily constructed on demand and explicitly destructed by Shutdown()
 *  function call.
 */
template<class C>
class ManagedStatic : public ManagedStaticBase
{
public:

  // Accessors.
  C* operator&() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return static_cast<C*>(m_Ptr);
  }

  const C* operator&() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return static_cast<C*>(m_Ptr);
  }

  C &operator*() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  const C &operator*() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  C *operator->() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }

  const C *operator->() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }
};

/// shutdown - Deallocate and destroy all ManagedStatic variables.
void shutdown();

/** \class Shutdown
 *  \brief Shutdown is a helper class that call shutdown() when it's destroyed.
 */
class Shutdown : private Uncopyable
{
public:
  Shutdown() { }

  ~Shutdown() { shutdown(); }
};

} // namespace of onnc

#endif
