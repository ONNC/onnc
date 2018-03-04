//===- ONNXReader.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_READER_ONNX_READER_H
#define ONNC_IR_READER_ONNX_READER_H
#include <onnc/Support/ErrorCode.h>
#include <onnc/Support/Path.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/IR/Module.h>

namespace onnc {
namespace onnx {

/** \class Reader
 *  \brief onnx::Reader reads ONNX protocol buffer files and generate Module
 *  object.
 */
class Reader
{
public:
  /// constructor.
  Reader();

  virtual ~Reader();

  /// @return The parsing result.
  SystemError parse(const Path& pFileName, Module& pModule);

  SystemError parse(StringRef pContent, Module& pModule);
};

} // namespace of onnx
} // namespace of onnc

#endif
