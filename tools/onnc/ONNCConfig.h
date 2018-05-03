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

  ONNCConfig& addInput(const onnc::Path& pFilePath);

  std::vector<onnc::Path> inputs() const;

  const onnc::Path& output() const { return m_Output; }

  void setOutput(const onnc::Path& pFileName) { m_Output = pFileName; }

  const onnc::Quadruple& quadruple() const { return m_Quadruple; }

  /// set up Quadruple
  void setQuadruple(const std::string& pValue);

  const std::string& getArchName() const { return m_Arch; }

  void setArchName(const std::string& pName) { m_Arch = pName; }

private:
  typedef std::vector<onnc::Path> FilePathList;

private:
  FilePathList m_Inputs;
  onnc::Path m_Output;
  onnc::Quadruple m_Quadruple;
  std::string m_Arch;
};

#endif
