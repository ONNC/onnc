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

/** \class Config
 *  \brief Config stores all application configurations.
 */
class Config
{
public:
  Config();

  ~Config() {}

  const onnc::Path &input() const { return m_Input; }

  void setInput(const onnc::Path &pFileName) { m_Input = pFileName; }

  const onnc::Path &output() const { return m_Output; }

  void setOutput(const onnc::Path &pFileName) { m_Output = pFileName; }

  const std::string march() const { return m_Arch; }

  void setMarch(const std::string &pArch) { m_Arch = pArch; }

  bool PrintModuleBeforeISel() const { return m_PrintModuleBeforeISel; }

  void setPrintModuleBeforeISel(bool pSet) { m_PrintModuleBeforeISel = pSet; }

private:
  onnc::Path m_Input;
  onnc::Path m_Output;
  std::string m_Arch;
  bool m_PrintModuleBeforeISel;
};

#endif
