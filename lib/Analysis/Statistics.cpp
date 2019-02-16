//===- Statistics.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/Counter.h>
#include <onnc/Analysis/Statistics.h>
#include <onnc/Analysis/GlobalStatistics.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IndentOStream.h>
#include <onnc/JSON/Reader.h>

#include <fstream>

using namespace onnc;

constexpr const char gCounterGroupName[] = "Counter";

//===----------------------------------------------------------------------===//
// Statistics
//===----------------------------------------------------------------------===//
// Statistics hides the implementation details to keep flexibility for change.
// It is reduced to an adapter of StatisticsPrivate.
//===----------------------------------------------------------------------===//
namespace onnc {
namespace internal {
  std::ostream& print(std::ostream& stream, const Counter& counter, const std::string& separator)
  {
    return stream << "Counter{ "
                  << "name=\"" << counter.name() << "\""
                  << ", desc=\"" << counter.desc() << "\""
                  << ", value=" << counter
                  << " }" << separator;
  }
} // namespace internal
} // namespace onnc

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

json::Group Statistics::getCounterGroup() const
{
  auto& gStat = global::stats();
  assert(gStat.hasGroup(gCounterGroupName));

  return gStat.group(gCounterGroupName);
}

std::ostream& Statistics::print(std::ostream& stream, const std::string& separator) const
{
  for (const auto counter : (*this) | view::counter) {
    if (counter.isAllowPrint()) {
      internal::print(stream, counter, separator);
    }
  }

  return stream;
}
