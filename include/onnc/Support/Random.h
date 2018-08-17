//===- Random.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_RANDOM_H
#define ONNC_SUPPORT_RANDOM_H
#include <cstdio>

namespace onnc {

/** \class onnc::Random
 *  \brief provides random generator.
 *
 *  class Random leverages system's random device to generate random number.
 */
class Random
{
public:
  typedef unsigned int result_type;

public:
  Random();

  ~Random();

  result_type operator()() const;

  static unsigned int maximun();

private:
  unsigned int GetRandomNumberSeed() const;
};

} // namespace of onnc

#endif
