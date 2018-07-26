//===- OptStorage.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_STORAGE_H
#define ONNC_OPTION_OPTION_STORAGE_H
#include <vector>
#include <ostream>

namespace onnc {
namespace cl {

//===----------------------------------------------------------------------===//
// OptStorage
//===----------------------------------------------------------------------===//

/** \class OptStorage
 *  \brief This version of OptStorage provides the implicit casting to DataType.
 */
template<class DataType, bool IsClass>
class OptStorage { };

/** \class OptStorage
 *  \brief OptStorage with internal storage and inheritability.
 *
 * Define how to hold a class type object, such as a string.  Since we can
 * inherit from a class, we do so.  This makes us exactly compatible with the
 * object in all cases that it is used.
 */
template<class DataType>
class OptStorage<DataType, true> : public DataType
{
public:
  OptStorage()
    : DataType(), m_ValueStr() {
  }

  virtual ~OptStorage() { }

  const std::string& getValueStr() const { return m_ValueStr; }

  void setValueStr(const std::string& pS) { m_ValueStr = pS; }

  void setValueDesc(const std::string& pS) { m_ValueDesc = pS; }

  template<class T>
  void setValue(const T& pValue) { DataType::operator=(pValue); }

  const DataType& getValue() const { return *this; }

  DataType& getValue() { return *this; }

  const std::string& getValueDesc() const { return m_ValueDesc; }

  void print(std::ostream& pOS) const { pOS << getValue(); }

protected:
  std::string m_ValueStr;
  std::string m_ValueDesc;
};

/** \class OptStorage
 *  \brief OptStorage with internal storage but no inheritability.
 *
 * Define a partial specialization to handle things we cannot inherit from.  In
 * this case, we store an instance through containment, and overload operators
 * to get at the value.
 */
template<class DataType>
class OptStorage<DataType, false>
{
public:
  OptStorage()
    : m_Data(DataType()), m_ValueStr() {
  }

  virtual ~OptStorage() { }

  const std::string& getValueStr() const { return m_ValueStr; }

  void setValueStr(const std::string& pS) { m_ValueStr = pS; }

  void setValueDesc(const std::string& pS) { m_ValueDesc = pS; }

  template<class T>
  void setValue(const T& pValue) { m_Data = pValue; }

  DataType& getValue() { return m_Data; }

  const DataType& getValue() const { return m_Data; }

  // type casting.
  operator DataType() const { return getValue(); }

  // If the datatype is a pointer, support -> on it.
  DataType operator->() const { return m_Data; }

  const std::string& getValueDesc() const { return m_ValueDesc; }

  void print(std::ostream& pOS) const { pOS << m_Data; }

protected:
  DataType m_Data;
  std::string m_ValueStr;
  std::string m_ValueDesc;
};

//===----------------------------------------------------------------------===//
// ListStorage
//===----------------------------------------------------------------------===//
/** \class ListStorage
 *  \brief ListStorage provides the implicit casting to std::vector<DataType>.
 */
template<class DataType, bool IsClass>
class ListStorage : public std::vector< OptStorage<DataType, IsClass> >
{
protected:
  typedef OptStorage<DataType, IsClass> option_type;

  typedef std::vector<option_type> InternalList;

public:
  void addValue(const option_type& pOption) { this->push_back(pOption); }
};

} // namespace of cl
} // namespace of onnc

#endif
