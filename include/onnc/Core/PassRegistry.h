//===- PassRegistry.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_REGISTRY_H
#define ONNC_CORE_PASS_REGISTRY_H
#include <onnc/Core/Pass.h>
#include <onnc/Core/PassInfo.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/ADT/Uncopyable.h>
#include <map>

namespace onnc {

/** \class onnc::PassRegistry
 *  \brief stores a set of pass ID and its information.
 */
class PassRegistry : private Uncopyable
{
public:
  PassRegistry() { }

  /// Delete all registered passes. All passes are delegated from
  /// INITIALIZE_PASS macro, PassRegistry is required to delete all PassInfo
  /// objects.
  ~PassRegistry();

  /// Look up a pass' corresponding PassInfo
  /// @retval nullptr Not found
  const PassInfo* getPassInfo(Pass::AnalysisID pID) const;

  /// register a pass
  /// If @ref pInfo had been registered, then an error occurs.
  void registerPass(const PassInfo& pInfo);

  /// clear all registered passes. This function releases memory space from
  /// registered PassInfo objects.
  void clear();

  bool isEmpty() const;

  unsigned int numOfPasses() const;

private:
  typedef std::map<Pass::AnalysisID, const PassInfo*> MapType;

private:
  MapType m_Map;
};

/// Get the singleton of the PassRegistry
PassRegistry* GetPassRegistry();

} // namespace of onnc

#endif
