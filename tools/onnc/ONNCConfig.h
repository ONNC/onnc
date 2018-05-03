//===- ONNCConfig.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COMPILER_ONNC_CONFIG_H
#define ONNC_COMPILER_ONNC_CONFIG_H
#include <onnc/Core/Application.h>
#include <onnc/Support/Path.h>
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetOptions.h>
#include <vector>

/** \class ONNCConfig
 *  \brief ONNCConfig collects all options on the command line.
 */
class ONNCConfig
{
public:
  static constexpr const char* DefaultOutputName = "a.out";

public:
  ONNCConfig();

  ~ONNCConfig();

  const onnc::Path& input() const { return m_Input; }

  void setInput(const onnc::Path& pFilePath) { m_Input = pFilePath; }

  const onnc::Path& output() const { return m_Output; }

  void setOutput(const onnc::Path& pFileName) { m_Output = pFileName; }

  const onnc::Quadruple& quadruple() const { return m_Quadruple; }

  /// set up Quadruple
  void setQuadruple(const std::string& pValue);

  const std::string& getArchName() const { return m_Arch; }

  void setArchName(const std::string& pName) { m_Arch = pName; }

  onnc::TargetOptions& target() { return m_TargetOptions; }

  const onnc::TargetOptions& target() const { return m_TargetOptions; }

private:
  onnc::Path m_Input;
  onnc::Path m_Output;
  onnc::Quadruple m_Quadruple;
  std::string m_Arch;
  onnc::TargetOptions m_TargetOptions;
};

#endif
