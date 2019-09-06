//===- GlobalStatistics.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/GlobalStatistics.h>
#include <onnc/Analysis/Statistics.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/Support/Memory.h>

#include <memory>

using namespace onnc;

namespace onnc {
namespace internal {

/*
static const char* g_CassettePostfix = ".cdb";
static const char* g_ConfigPostfix = ".rc";
*/

//===----------------------------------------------------------------------===//
// SkyGlobalPrivate
//===----------------------------------------------------------------------===//
class SkyGlobalPrivate
{
public:
  SkyGlobalPrivate();

  ~SkyGlobalPrivate() = default;

  Statistics& read(const Path& pConfig, Statistics::AccessMode pMode);

  Statistics& statistics() { return *m_pStat; }

  void reset();

private:
  std::unique_ptr<Statistics> m_pStat;
};

} // namespace of internal
} // namespace of skymizer

static ManagedStatic<internal::SkyGlobalPrivate> g_Stat;


//===----------------------------------------------------------------------===//
// Initialization - JSON configure
//===----------------------------------------------------------------------===//

void onnc::ClearStats()
{
  g_Stat->reset();
}


//===----------------------------------------------------------------------===//
// global
//===----------------------------------------------------------------------===//
Statistics& global::stats()
{
  auto& stats = g_Stat->statistics();
  stats.addGroup("Counter");
  stats.addGroup("Counter_Desc");
  return stats;
}


//===----------------------------------------------------------------------===//
// SkyGlobalPrivate member functions
//===----------------------------------------------------------------------===//
internal::SkyGlobalPrivate::SkyGlobalPrivate()
  : m_pStat(std::make_unique<Statistics>()) {
}

Statistics&
internal::SkyGlobalPrivate::read(const Path& pConfig, Statistics::AccessMode pMode)
{
  m_pStat->open(pConfig, pMode);
  return *m_pStat;
}

void internal::SkyGlobalPrivate::reset()
{
  m_pStat = std::make_unique<Statistics>();
}
