//===- Reader.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_READER_H
#define ONNC_JSON_READER_H
#include <onnc/JSON/Value.h>
#include <onnc/JSON/Object.h>
#include <onnc/ADT/StringRef.h>
#include <cstdio>

namespace onnc {

class Path;

/** \namespace onnc::json
 *  \brief Libraries for JSON parsing and writing
 */
namespace json {

/** \class Reader
 *  \brief JSON reader.
 */
class Reader
{
public:
  enum Result {
    kSuccess,
    kCantOpen,
    kIllegal
  };

public:
  /// Constructor. Initialize scanner.
  Reader();

  /// Destructor. Destroy scanner.
  virtual ~Reader();

  /// parse a JSON file.
  /// @param[out] pRoot The root json value.
  /// @retval true If it succeed to parse file @ref pPath
  /// @retval false If it fails to check the grammar.
  Result parse(const Path& pPath, Value& pRoot);

  /// @retval true If succeed to read content @ref pContent
  bool read(StringRef pContent, Value& pRoot);

private:
  void open(FILE* pFile);

  void load(StringRef pContent);

private:
  void* m_Scanner;  
};

} // namespace of json
} // namespace of onnc

#endif
