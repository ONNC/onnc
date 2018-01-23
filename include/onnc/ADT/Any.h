//===- Any.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ANY_H
#define ONNC_ADT_ANY_H
#include <onnc/Support/OStrStream.h>
#include <onnc/ADT/TypeTraits.h>
#include <onnc/ADT/TypeTag.h>
#include <ostream>

namespace onnc {

/** \class Any
 *  \brief Any is a storage for multiple types.
 */
class Any
{
public:
  Any();

  template<typename ValueType> Any(const ValueType& pValue);

  Any(const Any& pCopy);

  virtual ~Any();

  Any& swap(Any& pRHS);

  template<typename ValueType> Any& operator=(const ValueType& pRHS);

  Any& operator=(const Any& pRHS);

  bool empty() const;

  void clear();

  void print(std::ostream& pOS) const;

  virtual Any* clone() const;

  TypeTag type() const {
    return (NULL == m_pContent)? NULL : m_pContent->type();
  } 

protected:
  template<typename ValueType>
  friend ValueType* unsafe_cast(Any*);

  struct PlaceHolder
  {
  public:
    virtual ~PlaceHolder() { }
    virtual PlaceHolder* clone() const = 0;
    virtual void print(std::ostream& pOS) const = 0;
    virtual TypeTag type() const = 0;
    virtual bool classof(const PlaceHolder& pHolder) const = 0;
  };

  template<typename ValueType>
  struct Holder : public PlaceHolder
  {
  public:
    Holder(const ValueType& pValue) : held(pValue) { }

    Holder* clone() const { return new Holder(held); }

    void print(std::ostream& pOS) const { pOS << held; }

    TypeTag type() const { return type_tag<ValueType>(); }

    bool classof(const PlaceHolder& pHolder) const {
      return (pHolder.type() == this->type());
    }

  private:
    Holder& operator=(const Holder& ); // NOT IMPLEMENT

  public:
    ValueType held;
  };

protected:
  PlaceHolder* m_pContent;
};

//===----------------------------------------------------------------------===//
// Series of casting
//===----------------------------------------------------------------------===//
template<typename ValueType> ValueType*
unsafe_cast(Any* pOperand);

template<typename ValueType> const ValueType*
unsafe_cast(const Any* pOperand);

template<typename ValueType> ValueType
unsafe_cast(Any& pOperand);

template<typename ValueType> ValueType
unsafe_cast(const Any& pOperand);

template<typename ValueType> ValueType&&
unsafe_cast(Any&& pOperand);

} // namespace of onnc

namespace std {

inline ostream&
operator<<(ostream& pOS, const onnc::Any& pValue)
{
  pValue.print(pOS);
  return pOS;
}

inline std::string to_string(const onnc::Any& pAny)
{
  std::string result;
  onnc::OStrStream oss(result);
  pAny.print(oss);
  return result;
}

} // namespace of std

#include "Bits/Any.tcc"

#endif
