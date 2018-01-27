//===- AboutData.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/AboutLicense.h>
#include <onnc/Config/AboutData.h>
#include <onnc/Support/IFStream.h>
#include <streambuf>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AboutLicense
//===----------------------------------------------------------------------===//
AboutLicense::AboutLicense(const AboutData& pData)
  : m_Key(kUnknown), m_Text() {
}

AboutLicense::AboutLicense(const Path& pLicenseFile, const AboutData& pData)
  : m_Key(kUnknown) {
  read(pLicenseFile);
}

AboutLicense::AboutLicense(const std::string& pLicenseText, const AboutData& pData)
  : m_Key(kPrivate), m_Text(pLicenseText) {
}

AboutLicense::AboutLicense(const AboutLicense& pOther)
  : m_Key(pOther.m_Key), m_Text(pOther.m_Text) {
}

AboutLicense& AboutLicense::operator=(const AboutLicense& pOther)
{
  m_Key  = pOther.m_Key;
  m_Text = pOther.m_Text;
  return *this;
}

void AboutLicense::read(const Path& pLicenseFile)
{
  IFStream ifs(pLicenseFile);
  if (!ifs.good() || !ifs.is_open())
    return;

  ifs.seekg(0, std::ios::end);
  m_Text.reserve(ifs.tellg());
  ifs.seekg(0, std::ios::beg);
  m_Text.assign((std::istreambuf_iterator<char>(ifs)),
                 std::istreambuf_iterator<char>());
}

void AboutLicense::print(std::ostream& pOS) const
{
  switch (this->key()) {
    case kUnknown:
      pOS << "Skymizer Confidential Class A+.";
      break;
    case kPrivate:
      pOS << "Customized License, SPECIAL COSTUMER ONLY.";
      break;
    case kProprietary:
      pOS << "See LICENSE.TXT for details.";
      break;
  }
}
