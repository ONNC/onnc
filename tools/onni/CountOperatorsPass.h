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

#include <string>

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

  ReturnType runOnModule(Module& pModule) override;

private:
  std::string m_Prefix;
};

// XXX: Experimental
CountOperatorsPass *CreateCountOperatorsPass(const std::string &pPrefix);

} // namespace of onnc

#endif
