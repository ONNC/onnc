//===- SkyGlobal.h --------------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_GLOBALSTATISTICS_H
#define ONNC_ANALYSIS_GLOBALSTATISTICS_H
#include <onnc/Analysis/Statistics.h>
#include <onnc/Analysis/StatisticsGroup.h>

namespace onnc {

namespace global {

Statistics* stat();


} // namespace of global


/// Clear global::config()
void ClearStats();


} // namespace of skymizer

#endif

