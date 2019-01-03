//===- Storage.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_STORAGE_H
#define ONNC_JSON_STORAGE_H
#include <onnc/JSON/Group.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/StringList.h>
#include <onnc/Support/Path.h>
#include <string>
#include <ostream>

namespace onnc {
namespace json {

/** \class Storage
 *  \brief Storage provides an interface to maintain JSON object in a file.
 *
 *  Storage encapsulates JSON object and gives a systematic use of JSON object.
 *  A JSON object is separated into two levels of views: groups and entries.
 *  A group contains multiple entries, and an entry forms a key-value pair.
 *  Groups forms a nested structure. That means, a group can contains another
 *  groups.
 *
 *  Here is an example to add a group in storage and then get it back.
 *  \code
 *  json::Storage storage;
 *  storage.addGroup("a group");
 *  // add en entry { key, value } is { "e1", 0 }
 *  storage.group("a group").writeEntry("e1", 0);
 *
 *  // handle other things
 *  json::StorageGroup group = storage.group("a group");
 *
 *  // -1 is a default value if "e1" dones't exist
 *  int v = group.readEntry("e1", -1);
 *  \endcode
 *
 *  @ref Storage::addGroup and Group::addGroup add a group into a storage or
 *  a parent group. @ref Storage::group and @ref Group::group returns child
 *  group.
 *
 *  In case that you uses storage as a member variable, you might want the
 *  storage object is invalid and can postpone open/read calls. You can add
 *  a dummy integer in the constructor parameter. Here is the example:
 *  \code
 *  json::Storage storage(0);
 *  storage.read("{ }");
 *  \endcode
 *
 *  An entry is a key-value pair.
 *  A key must be a string, used to identify an entry. A value can be multiple
 *  types:
 *  - an integer,
 *  - a floating point number,
 *  - a boolean value,
 *  - a path,
 *  - a string, or
 *  - an array of abovementioned types.
 *
 *  You can read or write a value into a group by series of readEntry and
 *  writeEntry functions.
 */
class Storage : private Uncopyable
{
public:
  enum AccessMode {
    kReadOnly, ///< Opened, and the JSON file is read-only
    kReadWrite ///< Opened, and will write back to JSON file in destructor.
  };

  enum {
    Empty
  };

public:
  /// Default constructor. It is equivalent to Storage("{ }")
  Storage();

  /// Constructor with dummy integer. This is a invalid storage (not read
  /// anything).
  /// This constructor is used to postpone open/read calls.
  explicit Storage(int);

  /// Read the JSON object from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Storage(StringRef pContent);

  /// Read the JSON object from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Storage(const std::string& pContent);

  /// Read the JSON object from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Storage(const char* pContent);

  /// Read the JSON object from file @ref pFile
  /// If @ref pFile can not be parsed, then a fatal error is thrown.
  /// 
  /// @param[in] pFile The file
  /// @param[in] pMode If the @ref pFile file is writable, the final result will
  ///                  be writen back when destruction.
  Storage(const Path& pFile, AccessMode pMode = kReadOnly);

  /// Destructor. If @ref getAccessMode() is writable, then write back the value.
  virtual ~Storage();

  /// Opens the configuration object from @ref pFile
  ///
  /// @param[in] pFile The configuration file
  /// @param[in] pMode If the @ref pFile file is writable, the final result will
  ///                  be writen back when destruction.
  Storage& open(const Path& pFile, AccessMode pMode = kReadWrite);

  /// Reads the content of @pref pContent as a configuration object.
  ///
  /// @param[in] pContent The content of a configuration file
  Storage& read(StringRef pContent);

  /// @retval true Only if we read configuration object successfully.
  bool isValid() const;

  /// Returns the access mode of this configuration object
  AccessMode accessMode() const;

  /// Returns a list of groups that are known about.
  StringList groupList() const;

  /// Returns a list of group names.
  void groupList(StringList& pGroupList) const;

  /// @retval true If there is existing a group named @ref pGroup.
  bool hasGroup(StringRef pGroup) const;

  /// Deletes a configuration entry group.
  bool deleteGroup(StringRef pGroup);

  /// Returns the group named @ref pGroup
  const Group group(StringRef pGroup) const;

  /// Returns the group named @ref pGroup
  Group group(StringRef pGroup);

  /// Add a sub-group @ref pName and return the group.
  Group addGroup(StringRef pName);

  /// Update the group object @ref pName by group @ref pGroup.
  Storage& update(StringRef pName, const Group& pGroup);

  /// Merge the group object @ref pGroup as a sub-group @ref pName.
  /// This function \b copies @ref pGroup into the configuration object.
  Storage& merge(StringRef pName, const Group& pGroup);

  /// Print the content of the configuration object
  void print(std::ostream& pOS) const;

  /// @retval The filename of the configuration object.
  const Path& filename() const { return m_FilePath; }

  /// reset the configuration object.
  Storage& reset();

  /// The top-level group
  Group top();

  /// Synchronize the configuration file.
  /// @retval true Success
  bool sync();

  inline Storage& clear() { return reset(); };

protected:
  json::Group* m_pGroup;
  AccessMode m_AccessMode;
  Path m_FilePath;
  json::Value m_Value;
};

inline std::ostream& operator<<(std::ostream& pOS, const Storage& pStorage)
{
  pStorage.print(pOS);
  return pOS;
}

} // namespace of json
} // namespace of onnc

#endif
