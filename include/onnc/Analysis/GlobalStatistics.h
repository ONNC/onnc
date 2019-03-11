//===- SkyGlobal.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_GLOBALSTATISTICS_H
#define ONNC_ANALYSIS_GLOBALSTATISTICS_H
#include <onnc/Analysis/Statistics.h>

namespace onnc {

namespace global {

Statistics& stats();


} // namespace of global


/// Clear global::config()
void ClearStats();


} // namespace of skymizer

#endif

