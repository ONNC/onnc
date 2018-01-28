//===- AboutData.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/AboutData.h>
#include <onnc/Config/AboutLicense.h>
#include <onnc/Support/IFStream.h>
#include <streambuf>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AboutData
//===----------------------------------------------------------------------===//
AboutData::AboutData(const std::string& pAppName,
                     const std::string& pProgName,
                     const std::string& pVersion,
                     enum AboutLicense::Key pLicenseType,
                     const std::string& pShortDescription,
                     const std::string& pCopyrightStatement,
                     const std::string& pHomePageAddress,
                     const std::string& pBugsEmailAddress)
  : m_AppName(pAppName),
    m_ProgName(pProgName),
    m_Version(pVersion),
    m_ShortDesc(pShortDescription),
    m_CopyRight(pCopyrightStatement),
    m_HomePageAddress(pHomePageAddress),
    m_BugsEmailAddress(pBugsEmailAddress),
    m_License(*this) {
}

void AboutData::print(std::ostream& pOS, bool pAdvanced) const
{
  pOS << "NAME\n"
      << "\t" << m_ProgName << " -- " << m_ShortDesc << "\n" << std::endl;
  pOS << "SYNOPSIS\n"
      << "\t" << m_ProgName << " [options ...]\n" << std::endl;
  pOS << "DESCRIPTION" << std::endl;
  OptDefinitionList::const_iterator option, oEnd = m_BasicOptions.end();
  for (option = m_BasicOptions.begin(); option != oEnd; ++option) {
    pOS << "\t";
    (*option)->print(pOS);
  }

  if (pAdvanced) {
    pOS << std::endl;
    pOS << "ADVANCED DESCRIPTION" << std::endl;
    OptDefinitionList::const_iterator option, oEnd = m_AdvOptions.end();
    for (option = m_AdvOptions.begin(); option != oEnd; ++option) {
      pOS << "\t";
      (*option)->print(pOS);
    }
  }
  pOS << std::endl;

  pOS << "COPYRIGHT" << std::endl;
  pOS << "\t" << m_AppName << " version " << m_Version << std::endl;
  pOS << "\t" << m_HomePageAddress << std::endl;

  if (pAdvanced) {
    pOS << "\tPlease report bugs to " << m_BugsEmailAddress << std::endl;
  }
}

AboutData& AboutData::setLicenseType(AboutLicense::Key pType)
{
  m_License.setLicense(pType);
  return *this;
}

AboutData& AboutData::setLicenseText(const std::string& pText)
{
  m_License.setLicenseText(pText);
  return *this;
}

AboutData& AboutData::addBasicOption(const cl::OptDefs& pOption)
{
  m_BasicOptions.push_back(&pOption);
  return *this;
}

AboutData& AboutData::addAdvOption(const cl::OptDefs& pOption)
{
  m_AdvOptions.push_back(&pOption);
  return *this;
}
