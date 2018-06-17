#include "onnc/IR/ONNCOptions.h"
#include "onnc/Option/CommandLine.h"
#include "onnc/Support/ManagedStatic.h"

using namespace onnc;

namespace onnc {

struct BatchSize {
  void operator=(const int &Val) const
  {
    if (Val > 0)
      ONNCOptions::getBatchSize() = Val;
  }
};

ONNCOptions::ONNCOptions()
{
  // used by UpdateGraphOutputSize
  static cl::opt<BatchSize, cl::OptParser<int> > batch_size(
      "batch-size", cl::kShort, cl::kOptional, cl::kValueRequired, cl::init(-1),
      cl::desc("specific input batch size"));
}

int &ONNCOptions::getBatchSize()
{
  static int batch_size = -1;
  return batch_size;
}

} // namespace onnc
