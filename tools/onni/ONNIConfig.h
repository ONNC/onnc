//===- ONNIConfig.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_ONNI_CONFIG_H
#define ONNC_INTERPRETER_ONNI_CONFIG_H
#include <onnc/Core/Application.h>
#include <onnc/Support/Path.h>
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetOptions.h>
#include <vector>

/** \class ONNIConfig
 *  \brief ONNIConfig collects all options on the command line.
 */
class ONNIConfig
{
public:
  static constexpr const char* DefaultOutputName = "out.tsr";

  enum VerboseLevel : int {
    kQuiet = 0,
    kNotice = 1,
    kNormal = 2,
    kNoisy  = 3,
    kDebugging = 7
  };

public:
  ONNIConfig();

  ~ONNIConfig();

  const onnc::Path& model() const { return m_Model; }

  void setModel(const onnc::Path& pFilePath) { m_Model = pFilePath; }

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

  void setVerbose(unsigned int pLevel) { m_Verbose = pLevel; }

  unsigned int verbose() const { return m_Verbose; }

  void setDryRun(bool pIsDryRun) { m_DryRun = pIsDryRun; }

  bool dryRun() const { return m_DryRun; }

private:
  onnc::Path m_Model;
  onnc::Path m_Input;
  onnc::Path m_Output;
  onnc::Quadruple m_Quadruple;
  std::string m_Arch;
  onnc::TargetOptions m_TargetOptions;
  unsigned int m_Verbose;
  bool m_DryRun;
};

#endif
