//===- Statistics.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_STATISTICS_H
#define ONNC_ANALYSIS_STATISTICS_H
#include <onnc/JSON/Storage.h>
#include <onnc/Support/Path.h>
#include <onnc/ADT/StringList.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/JSON/Value.h>

#include <iostream>

namespace onnc {

/** \class Statistics
 *  \brief Statistics implements Skymizer's default configuration system.
 *
 *  \code
 *  Statistics config("~/configrc");
 *  StringRef image = config.group("General").readEntry("image", "look.jpg");
 *  \endcode
 */
class Statistics final : public json::Storage
{
public:
  /// Default constructor
  /// Default constructor is invalid because we don't read anything.
  Statistics() = default;

  Statistics(const Statistics&) = delete;
  Statistics(Statistics&&) = delete;

  Statistics& operator=(const Statistics&) = delete;
  Statistics& operator=(Statistics&&) = delete;

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
  Statistics(const Path& pFile, json::Storage::AccessMode pMode = kReadOnly);

  /// Destructor. If @ref accessMode() is writable, then write back the value.
  ~Statistics() = default;

  json::Group getCounterGroup() const;

  /// Print all statistics data to the given stream, entries are separated by
  /// parameter separator
  std::ostream& print(std::ostream& stream = std::cout, const std::string& separator = "\n") const;
};


} // namespace onnc

#endif
