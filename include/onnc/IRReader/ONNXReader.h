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
#include <onnc/ADT/ConstBuffer.h>
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

  /// parse ONNX file
  /// @return error occurred in the parsing.
  SystemError parse(const Path& pFileName, Module& pModule);

  SystemError parse(ConstBuffer pContent, Module& pModule);

  /// Set the total bytes limit.
  /// @param[in] pTotalBytesLimit
  /// @param[in] pWarningThreshold
  void setTotalBytesLimit(int pTotalBytesLimit, int pWarningThreshold);

private:
  int m_TotalBytesLimit;
  int pWarningThreshold;
};

} // namespace of onnx
} // namespace of onnc

#endif
