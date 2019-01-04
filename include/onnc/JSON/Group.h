//===- Group.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_GROUP_H
#define ONNC_JSON_GROUP_H
#include <onnc/Support/Path.h>
#include <onnc/JSON/Array.h>
#include <onnc/JSON/Object.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/StringList.h>
#include <ostream>
#include <vector>

namespace onnc {
namespace json {

class Storage;

/** \class Group
 */
class Group
{
public:
  class GroupIterator
  {
  public:
    GroupIterator();

    GroupIterator& next();

    StringRef name() const;

    Group group();

    bool operator==(const GroupIterator& pX) const { return (m_Ptr == pX.m_Ptr); }

    bool operator!=(const GroupIterator& pX) const { return (m_Ptr != pX.m_Ptr); }

  private:
    friend class Group;

    GroupIterator(const json::Object::iterator& pPtr, json::Object& pObject);

  private:
    json::Object::iterator m_Ptr;
    json::Object* m_pObject; //< privde the end of groups
  };

  class ValueIterator
  {
  public:
    ValueIterator();

    ValueIterator& next();

    Group group();

    ValueIterator& operator++() { return next(); }

    Group operator*() { return group(); }

    bool operator==(const ValueIterator& pX) const { return (m_Ptr == pX.m_Ptr); }

    bool operator!=(const ValueIterator& pX) const { return (m_Ptr != pX.m_Ptr); }

  private:
    friend class Group;

    ValueIterator(const json::Array::iterator& pPtr, json::Array& pArray);

  private:
    json::Array::iterator m_Ptr;
    json::Array* m_pArray;
  };

  typedef GroupIterator iterator;

public:
  /// default constructor is a Null group.
  Group();

  Group(Storage& pParent, StringRef pName);

  Group(const Group& pOther);

  ~Group() { }

  Group& operator=(const Group& pOther);

  /// group - Return the sub-group @ref pName.
  /// This function is unsafe. If there is no sub-group named @ref pKey, then
  /// the function behavior is undefined.
  Group group(StringRef pName);

  /// hasGroup - Check whether @ref pName sub-group exists or not.
  bool hasGroup(StringRef pName) const;

  /// deleteGroup - Deletes a sub-group @ref pName.
  bool deleteGroup(StringRef pName);

  /// Add a sub-group @ref pName. If there is a group named @ref pName, then
  /// return the group.
  ///
  /// @param[out] pExist Whether or not the group exists.
  /// @return The group named @ref pName.
  Group addGroup(StringRef pName, bool* pExist = nullptr);

  /// Returns a list of groups that are known about.
  StringList groupList() const;

  /// Returns a list of gorups
  void groupList(StringList& pGroupList) const;

  /// Return the first itertator to group list
  GroupIterator gBegin();

  /// Return the end of itertators to group list
  GroupIterator gEnd();

  /// Return the iterator to the first value (group) of group @ref pName
  ValueIterator vBegin(StringRef pName);

  /// Return the iterator to the end of group @ref pName
  ValueIterator vEnd(StringRef pName);

  /// Check whether @ref pName entry exists or not.
  bool hasEntry(StringRef pName) const;

  /// Returns a list of entry names.
  StringList entryList() const;

  /// Returns a list of entry names.
  void entryList(StringList& pEntryList) const;

  /// clear - clear all entries in the config.
  void clear();

  /// isEmpty - return true if this group contains nothing.
  bool isEmpty() const;

  /// isNull - return true if this group is a null object.
  bool isNull() const;

  /// Null - return a null object
  static Group Null();

  /// Reads the integer value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  int readEntry(StringRef pKey, int pDefault) const;

  unsigned int readEntry(StringRef pKey, unsigned int pDefault) const;

  /// Reads the integer value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  long readEntry(StringRef pKey, long pDefault) const;

  /// Reads the floating point value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  double readEntry(StringRef pKey, double pDefault) const;

  /// Reads the boolean value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  bool readEntry(StringRef pKey, bool pDefault) const;

  /// Reads the string value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  StringRef readEntry(StringRef pKey, StringRef pDefault) const;

  /// Reads the string value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  StringRef readEntry(StringRef pKey, const char* pDefault) const;

  /// Reads the string value of an entry specified by @ref pKey
  /// If the entry doesn't exist, then return pDefault.
  StringRef readEntry(StringRef pKey, const std::string& pDefault) const;

  /// Reads a Path by @ref pKey.
  /// If the entry value doesn't exist, then return pDefault.
  Path readEntry(StringRef pKey, const Path& pDefault) const;

  /// Reads a list of string values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  StringList readEntry(StringRef pKey, const StringList& pDefault) const;

  /// Reads a list of integer values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  std::vector<int>
  readEntry(StringRef pKey, const std::vector<int>& pDefault) const;

  /// Reads a list of integer values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  std::vector<long>
  readEntry(StringRef pKey, const std::vector<long>& pDefault) const;

  /// Reads a list of floating point values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  std::vector<float>
  readEntry(StringRef pKey, const std::vector<float>& pDefault) const;

  /// Reads a list of floating point values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  std::vector<double>
  readEntry(StringRef pKey, const std::vector<double>& pDefault) const;

  /// Reads a list of boolean values with a key @ref pKey.
  /// If there are no matched key, then pDefault is returned.
  std::vector<bool>
  readEntry(StringRef pKey, const std::vector<bool>& pDefault) const;

  /// Writes a value @ref pValue to the configuration object.
  void writeEntry(StringRef pKey, StringRef pValue);

  /// Writes a value @ref pValue to the configuration object.
  void writeEntry(StringRef pKey, const Path& pValue);

  /// Writes a value @ref pValue to the configuration object.
  template<typename T> void
  writeEntry(StringRef pKey, const T& pValue) { m_pObject->write(pKey, pValue); }

  /// Writes a list of value @ref pValue to the configuration object.
  template<class InputIterator> void
  writeEntry(StringRef pKey,
             InputIterator pBegin, InputIterator pEnd) {
    json::Array array;
    InputIterator value;
    for (value = pBegin; value != pEnd; ++value) 
      array.push_back(json::Value(*value));

    m_pObject->write(pKey, array);
  }

  /// Constructor - public for testing.
  explicit Group(json::Object& pObject);

private:
  friend class Storage;

private:
  json::Object* m_pObject;
};

} // namespace of json
} // namespace of skymizer

#endif
