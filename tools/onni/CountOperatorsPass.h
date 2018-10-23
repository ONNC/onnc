//===- CountOperatorsPass.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COUNT_OPERATORS_PASS_H
#define ONNC_COUNT_OPERATORS_PASS_H
#include "Statistics.h"

#include <string>

namespace onnc {

class TargetBackend;

// XXX: Experimental

/** \class CountOperatorsPass
 *  \brief Count & print Operators count statistics
 */
class CountOperatorsPass : public OneDStatistic<std::string, int>
{
public:
  static char ID;

public:
  CountOperatorsPass(const std::string &pPrefix)
      : OneDStatistic(ID, pPrefix) {}

  ReturnType runOnModule(Module& pModule) override;

private:
  ~CountOperatorsPass() override {}

  std::pair<int, int> printHeader(OStream &pOS) const override;
  void printFooter(OStream &pOS) const override;

  std::pair<int, int> m_Width;
  uint64_t m_Total{0};
};

// XXX: Experimental
CountOperatorsPass *CreateCountOperatorsPass(const std::string &pPrefix);

} // namespace of onnc

#endif
