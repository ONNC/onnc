//===- Statistics.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/Statistics.h>
#include <onnc/Analysis/GlobalStatistics.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IndentOStream.h>
#include <onnc/JSON/Reader.h>
#include <fstream>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Statistics
//===----------------------------------------------------------------------===//
// Statistics hides the implementation details to keep flexibility for change.
// It is reduced to an adapter of StatisticsPrivate.
//===----------------------------------------------------------------------===//
Statistics::Statistics()
  : json::Storage(json::Storage::Empty) {
}

Statistics::Statistics(StringRef pContent)
  : json::Storage(pContent) {
}

Statistics::Statistics(const std::string& pContent)
  : json::Storage(pContent) {
}

Statistics::Statistics(const char* pContent)
  : json::Storage(pContent) {
}

Statistics::Statistics(const Path& pFilePath, json::Storage::AccessMode pMode)
  : json::Storage(pFilePath, pMode) {
}

bool Statistics::addCounter(StringRef pName, StringRef pDesc)
{
  auto& gStat = global::stats();
  if (gStat.group("Counter").hasEntry(pName))
    return false;
  gStat.group("Counter").writeEntry(pName, 0);
  gStat.group("Counter_Desc").writeEntry(pName, pDesc);
  return true;
}

bool Statistics::increaseCounter(StringRef pName, unsigned int incNumber)
{
  auto& gStat = global::stats();
  if (! gStat.group("Counter").hasEntry(pName))
    return false;
  int entry_value = gStat.group("Counter").readEntry(pName, 0) + incNumber;
  gStat.group("Counter").writeEntry(pName, entry_value);
  return true;
}

bool Statistics::decreaseCounter(StringRef pName, unsigned int decNumber)
{
  auto& gStat = global::stats();
  if (! gStat.group("Counter").hasEntry(pName))
    return false;
  int entry_value = gStat.group("Counter").readEntry(pName, 0) - decNumber;
  gStat.group("Counter").writeEntry(pName, entry_value);
  return true;
}

void Statistics::printCounter(StringRef pName, OStream &pOS)
{
  auto& gStat = global::stats();
  if (! gStat.group("Counter").hasEntry(pName))
    return;
  pOS << pName << "," 
      << gStat.group("Counter").readEntry(pName, 0) << ","
      << gStat.group("Counter_Desc").readEntry(pName, "no value") 
      // please note that this magic string comes from StatisticsTest.cpp.
      // I guess it's becuase readEntry is implemented by template.
      << std:: endl;
}

StringList Statistics::counterList() const
{
  return global::stats().group("Counter").entryList();
}

bool Statistics::resetCounter(StringRef pName, int initNum)
{
  auto& gStat = global::stats();
  if (! gStat.group("Counter").hasEntry(pName))
    return false;
  gStat.group("Counter").writeEntry(pName, initNum);
  return true;
}
