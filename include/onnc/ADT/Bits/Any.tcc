//===- Any.tcc ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/If.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <type_traits>

//===----------------------------------------------------------------------===//
// Templata Implementation of Any
//===----------------------------------------------------------------------===//
template<typename ValueType>
onnc::Any::Any(const ValueType& pValue)
  : m_pContent(new Holder<typename std::decay<const ValueType>::type>(pValue)) {
}

template<typename ValueType> onnc::Any&
onnc::Any::operator=(const ValueType& pRHS)
{
  Any(pRHS).swap(*this);
  return *this;
}

//===----------------------------------------------------------------------===//
// Casting
//===----------------------------------------------------------------------===//
/// pointer to pointer
template<typename ValueType> ValueType*
onnc::unsafe_cast(onnc::Any* pOperand)
{
  if (NULL == pOperand || pOperand->empty())
    return NULL;

  return &static_cast<Any::Holder<ValueType>*>(pOperand->m_pContent)->held;
}

/// constant pointer to constant pointer
template<typename ValueType> const ValueType*
onnc::unsafe_cast(const Any* pOperand)
{
  return unsafe_cast<ValueType>(const_cast<Any*>(pOperand));
}

/// reference to copy
template<typename ValueType> ValueType
onnc::unsafe_cast(onnc::Any& pOperand)
{
  typedef typename std::remove_reference<ValueType>::type nonref;
  nonref* result = unsafe_cast<nonref>(&pOperand);

  if (NULL == result)
    fatal(fatal_bad_unsafe_cast);

  typedef typename if_c<std::is_reference<ValueType>::value,
               ValueType,
               typename std::add_lvalue_reference<ValueType>::type>::type ref_type;

  return static_cast<ref_type>(*result);
}

/// constant reference to copy
template<typename ValueType> ValueType
onnc::unsafe_cast(const Any& pOperand)
{
  typedef typename std::remove_reference<ValueType>::type nonref;
  return unsafe_cast<const nonref &>(const_cast<Any&>(pOperand));
}

template<typename ValueType> ValueType&&
onnc::unsafe_cast(Any&& pOperand)
{
  return unsafe_cast<ValueType&&>(pOperand);
}
