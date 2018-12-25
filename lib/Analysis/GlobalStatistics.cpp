//===- GlobalStatistics.cpp ------------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, 2014, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/GlobalStatistics.h>
#include <onnc/Analysis/Statistics.h>
#include <onnc/Support/ManagedStatic.h>
/*
#include <onnc/ADT/Rope.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/Expansion.h>
#include <onnc/Support/FileStatus.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Environ.h>
#include <cstdlib>
#include <cassert>
*/

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

  ~SkyGlobalPrivate();

  Statistics* read(const Path& pConfig, Statistics::AccessMode pMode);

  Statistics* statistics() { return m_pStat; }

  void reset();

private:
  Statistics* m_pStat;
};

} // namespace of internal
} // namespace of skymizer

/*
static ManagedStatic<internal::SkyGlobalPrivate> g_SkyGlobal;
static ManagedStatic<Cassette> g_AppCassette;
static ManagedStatic<Cassette> g_SysCassette;
*/
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
Statistics* global::stats()
{
  Statistics* stats = g_Stat->statistics();
  stats->addGroup("Counter");
  stats->addGroup("Counter_Desc");
  return stats;
}


//===----------------------------------------------------------------------===//
// SkyGlobalPrivate member functions
//===----------------------------------------------------------------------===//
internal::SkyGlobalPrivate::SkyGlobalPrivate()
  : m_pStat(new Statistics()) {
}

internal::SkyGlobalPrivate::~SkyGlobalPrivate()
{
  delete m_pStat;
}

Statistics*
internal::SkyGlobalPrivate::read(const Path& pConfig, Statistics::AccessMode pMode)
{
  m_pStat->open(pConfig, pMode);
  return m_pStat;
}

void internal::SkyGlobalPrivate::reset()
{
  delete m_pStat;
  m_pStat = new Statistics();
}
