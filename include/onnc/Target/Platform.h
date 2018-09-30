//===- Platform.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_PLATFORM_H
#define ONNC_TARGET_PLATFORM_H
#include <onnc/Target/TargetBackend.h>

namespace onnc {

/** \class Platform
 *  \brief Platform collects information of a target platform.
 *
 *  One platform is made of multiple target backends.
 */
class Platform
{
public:
  /// Constructor.
  Platform(const std::string& pName);

  const std::string& name() const { return m_Name; }

  void setShortDescription(const std::string& pDesc) { m_ShortDesc = pDesc; }

  const std::string& getShortDescription() const { return m_ShortDesc; }

private:
  typedef std::vector<TargetBackend*> TargetBackendList;

private:
  std::string m_Name;
  std::string m_ShortDesc;
  TargetBackendList m_TargetBackends;
};

} // namespace of onnc

#endif
