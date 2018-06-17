#ifndef ONNC_OPTIONS_H
#define ONNC_OPTIONS_H

#include "onnc/Option/CommandLine.h"

namespace onnc {

#define ENABLE_ONNC_OPTIONS()                                                  \
  do {                                                                         \
    ONNCOptions options;                                                       \
  } while (false)

class ONNCOptions
{
public:
  ONNCOptions();
  static int &getBatchSize();
};
}

#endif
