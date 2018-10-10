//===- AboutData.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CONFIG_ABOUT_DATA_H
#define ONNC_CONFIG_ABOUT_DATA_H
#include <onnc/Config/AboutLicense.h>
#include <onnc/Support/Path.h>
#include <onnc/Option/OptDefs.h>
#include <ostream>
#include <string>
#include <vector>

namespace onnc {

/** \class AboutData
 *  \brief AboutData is used to store information about the Skymizer app.
 */
class AboutData
{
public:
  AboutData(const std::string& pAppName,
               const std::string& pProgramName,
               const std::string& pVersion,
               AboutLicense::Key pLicenseType = AboutLicense::kPrivate,
               const std::string& pShortDescription = std::string(),
               const std::string& pCopyrightStatement = "Copyright (C), The ONNC Team. All rights reserved.",
               const std::string& pHomePageAddress = "https://onnc.ai",
               const std::string& pBugsEmailAddress = "bugs@onnc.ai");

  ~AboutData() { }

  const std::string& appName() const { return m_AppName; }

  const std::string& programName() const { return m_ProgName; }

  const std::string& version() const { return m_Version; }

  AboutData& setLicenseType(AboutLicense::Key pType);

  AboutData& setLicenseText(const std::string& pText);

  const std::string& licenseText() const { return m_License.text(); }

  const std::string& copyright() const { return m_CopyRight; }

  const std::string& homepage() const { return m_HomePageAddress; }

  const std::string& bugAddress() const { return m_BugsEmailAddress; }

  AboutData& addBasicOption(const cl::OptDefs& pOption);

  AboutData& addAdvOption(const cl::OptDefs& pOption);

  void print(std::ostream& pOS, bool pAdvanced = false) const;

private:
  typedef std::vector<const cl::OptDefs*> OptDefinitionList;

private:
  std::string m_AppName;
  std::string m_ProgName;
  std::string m_Version;
  std::string m_ShortDesc;
  std::string m_CopyRight;
  std::string m_HomePageAddress;
  std::string m_BugsEmailAddress;
  AboutLicense m_License;
  OptDefinitionList m_BasicOptions;
  OptDefinitionList m_AdvOptions;
};

} // namespace of onnc

namespace std {

inline ostream& operator<<(ostream& pOS, const onnc::AboutData& pAbout)
{
  pAbout.print(pOS);
  return pOS;
}

inline ostream& operator<<(ostream& pOS, const onnc::AboutLicense& pAbout)
{
  pAbout.print(pOS);
  return pOS;
}

} // namespace of std

#endif
