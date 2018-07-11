//===- ONNCJITConfig.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JUST_IN_TIME_INTERPRETER_ONNC_CONFIG_H
#define ONNC_JUST_IN_TIME_INTERPRETER_ONNC_CONFIG_H
#include <onnc/Core/Application.h>
#include <onnc/Support/Path.h>
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetOptions.h>
#include <vector>

/** \class ONNCJITConfig
 *  \brief ONNCJITConfig collects all options on the command line.
 */
class ONNCJITConfig
{
public:
  static constexpr const char* DefaultOutputName = "a.out";

  enum VerboseLevel : int {
    kQuiet = 0,
    kNotice = 1,
    kNormal = 2,
    kNoisy  = 3,
    kDebugging = 7
  };

public:
  ONNCJITConfig();

  ~ONNCJITConfig();

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

private:
  onnc::Path m_Input;
  onnc::Path m_Output;
  onnc::Quadruple m_Quadruple;
  std::string m_Arch;
  onnc::TargetOptions m_TargetOptions;
  unsigned int m_Verbose;
};

#endif
