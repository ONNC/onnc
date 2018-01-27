//===- AboutData.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CONFIG_ABOUT_LICENSE_H
#define ONNC_CONFIG_ABOUT_LICENSE_H
#include <onnc/Support/Path.h>
#include <ostream>
#include <string>

namespace onnc {

class AboutData;

/** \class AboutLicense
 *  \brief AboutLicense is a internal class to describe License information.
 *
 *  Clients should get License information from AboutData, not operate
 *  AboutLicense directly.
 */
class AboutLicense
{
public:
  enum Key {
    kUnknown,
    kPrivate,
    kProprietary
  };

public:
  AboutLicense(const AboutLicense& pOther);

  AboutLicense& operator=(const AboutLicense& pOther);

  const std::string& text() const { return m_Text; }

  Key key() const { return m_Key; }

  void read(const Path& pLicenseFile);

  void print(std::ostream& pOS) const;

private:
  friend class AboutData;

  explicit AboutLicense(const AboutData& pData);

  AboutLicense(const Path& pLicenseFile, const AboutData& pData);

  AboutLicense(const std::string& pLicenseText, const AboutData& pData);

  void setLicense(Key pKey) { m_Key = pKey; }

  void setLicenseText(const std::string& pText) { m_Text = pText; }

private:
  Key m_Key;
  std::string m_Text;
};

} // namespace of onnc

#endif
