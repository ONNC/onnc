//===- CountOperatorsPass.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COUNT_OPERATORS_PASS_H
#define ONNC_COUNT_OPERATORS_PASS_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Analysis/Statistics.h>
#include <iomanip>
#include <iostream>


namespace onnc {

class TargetBackend;

// XXX: Experimental

/** \class CountOperatorsPass
 *  \brief Count & print Operators count statistics
 */
class CountOperatorsPass : public ModulePass
{
public:
  static char ID;

public:
  CountOperatorsPass(const std::string &pPrefix)
      : ModulePass(ID), m_Prefix(pPrefix) {}

  Pass::ReturnType runOnModule(Module& pModule) override;
  StringRef getPassName() const override { return "CountOperatorsPass"; }
  void print(OStream& pOS, const Module* pModule) const override;

private:
  ~CountOperatorsPass() override {}

  std::pair<int, int> printHeader(OStream &pOS) const;
  void printFooter(OStream &pOS) const;

  std::pair<int, int> m_Width;
  uint64_t m_Total{0};
  const char *SEP = " |";
  std::string m_Prefix;

  void printSeparator(OStream &pOS, std::pair<int, int> pWidth) const {
    pOS << m_Prefix
        << std::setfill('-')
        << std::setw(pWidth.first) << '-'
        << "-+"
        << std::setw(pWidth.second) << '-'
        << std::setfill(' ')
        << std::endl;
  }
};

// XXX: Experimental
CountOperatorsPass *CreateCountOperatorsPass(const std::string &pPrefix);

} // namespace of onnc

#endif
