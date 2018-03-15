//===- Config.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COMPILER_ONNX_TO_TG_CONFIG_H
#define ONNC_COMPILER_ONNX_TO_TG_CONFIG_H
#include <onnc/Support/Path.h>
#include <onnc/Core/CompilerConfig.h>

using namespace onnc;

/** \class Config
 *  \brief Config stores all application configurations.
 */
class Config : public CompilerConfig
{
public:
  Config();

  ~Config() {}

  const Path& input() const { return m_Input; }

  void setInput(const Path& pFileName) { m_Input = pFileName; }

  const Path& output() const { return m_Output; }

  void setOutput(const Path& pFileName) { m_Output = pFileName; }

private:
  Path m_Input;
  Path m_Output;
};

#endif
