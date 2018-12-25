//===- Statistics.h --------------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013 - 2016, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_STATISTICS_H
#define ONNC_ANALYSIS_STATISTICS_H
#include <onnc/Analysis/StatisticsGroup.h>
#include <onnc/Support/Path.h>
#include <onnc/ADT/StringList.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/JSON/Value.h>
#include <ostream>

namespace onnc {

/** \class Statistics
 *  \brief Statistics implements Skymizer's default configuration system.
 *
 *  \code
 *  Statistics config("~/configrc");
 *  StringRef image = config.group("General").readEntry("image", "look.jpg");
 *  \endcode
 */
class Statistics : private Uncopyable
{
public:
  enum AccessMode {
    kReadOnly, ///< Opened, and the JSON file is read-only
    kReadWrite ///< Opened, and will write back to JSON file in destructor.
  };

public:
  /// Default constructor
  /// Default constructor is invalid because we don't read anything.
  Statistics();

  /// Read the configuration from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Statistics(StringRef pContent);

  /// Read the configuration from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Statistics(const std::string& pContent);

  /// Read the configuration from string @ref pContent
  /// 
  /// @param[in] pContent The content of a configuration file
  explicit Statistics(const char* pContent);

  /// Read the configuration from file @ref pFile
  /// If @ref pFile can not be parsed, then a fatal error is thrown.
  /// 
  /// @param[in] pFile The configuration file
  /// @param[in] pMode If the @ref pFile file is writable, the final result will
  ///                  be writen back when destruction.
  Statistics(const Path& pFile, AccessMode pMode = kReadOnly);

  /// Destructor. If @ref accessMode() is writable, then write back the value.
  virtual ~Statistics();

  /// Opens the configuration object from @ref pFile
  ///
  /// @param[in] pFile The configuration file
  /// @param[in] pMode If the @ref pFile file is writable, the final result will
  ///                  be writen back when destruction.
  Statistics& open(const Path& pFile, AccessMode pMode = kReadWrite);

  /// Reads the content of @pref pContent as a configuration object.
  ///
  /// @param[in] pContent The content of a configuration file
  Statistics& read(StringRef pContent);

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
  const StatisticsGroup group(StringRef pGroup) const;

  /// Returns the group named @ref pGroup
  StatisticsGroup group(StringRef pGroup);

  /// Add a sub-group @ref pName and return the group.
  StatisticsGroup addGroup(StringRef pName);

  /// Update the group object @ref pName by group @ref pGroup.
  Statistics& update(StringRef pName, const StatisticsGroup& pGroup);

  /// Merge the group object @ref pGroup as a sub-group @ref pName.
  /// This function \b copies @ref pGroup into the configuration object.
  Statistics& merge(StringRef pName, const StatisticsGroup& pGroup);

  /// Print the content of the configuration object
  void print(std::ostream& pOS) const;

  /// @retval The filename of the configuration object.
  const Path& filename() const { return m_FilePath; }

  /// reset the configuration object.
  Statistics& reset();

  /// The top-level group
  StatisticsGroup top();

  /// Synchronize the configuration file.
  /// @retval true Success
  bool sync();

  inline Statistics& clear() { return reset(); };

  /// Add a counter with int type. The default description is "no description"
  /// @retval true Success
  bool addCounter(StringRef pName);
  bool addCounter(StringRef pName, StringRef pDesc);

  /// increase counter, default by 1
  /// @retval true Success
  bool increaseCounter(StringRef pName, unsigned int incNumber=1);
  /// decrease counter, default by 1
  /// @retval true Success
  bool decreaseCounter(StringRef pName, unsigned int decNumber=1);

  /// print counter name, counter value, and its description
  void printCounter(StringRef pName, OStream& pOS);

  /// print all key in Counter group
  /// @retval StringList
  StringList counterList() const;

  /// reset counter as initNum, default is 0
  /// @retval true Success
  bool resetCounter(StringRef pName, int initNum=0);  

protected:
  StatisticsGroup* m_pGroup;
  AccessMode m_AccessMode;
  Path m_FilePath;
  json::Value m_Value;
};

} // namespace of skymizer

namespace std {

inline ostream& operator<<(ostream& pOS, const onnc::Statistics& pConfig)
{
  pConfig.print(pOS);
  return pOS;
}

} // namespace of std

#endif
