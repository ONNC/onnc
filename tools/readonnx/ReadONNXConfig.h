//===- ReadONNXConfig.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_READ_ONNX_CONFIG_H
#define ONNC_READ_ONNX_CONFIG_H
#include <onnc/Core/Application.h>
#include <onnc/Support/Path.h>
#include <onnc/IR/Quadruple.h>
#include <onnc/Support/OFStream.h>
#include <vector>

/** \class ReadONNXConfig
 *  \brief ReadONNXConfig collects all options on the command line.
 */
class ReadONNXConfig
{
public:
  ReadONNXConfig();

  ~ReadONNXConfig();

  const onnc::Path& input() const { return m_Input; }

  void setInput(const onnc::Path& pFilePath) { m_Input = pFilePath; }

  void setOutput(const onnc::Path& pFileName);

  std::ostream& output() { return m_OStream; }

private:
  onnc::Path m_Input;
  onnc::OFStream m_OStream;
};

#endif
