//===- InterpreterPass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_PASS_H
#define ONNC_INTERPRETER_PASS_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

class TargetBackend;

// XXX: Experimental

/** \class InterpreterPass
 *  \brief Run interpreter.
 */
class InterpreterPass : public ModulePass
{
public:
  static char ID;

public:
  InterpreterPass(TargetBackend *pBackend,
                  char *pInputMem,
                  unsigned int pVerbose);

  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend *m_pBackend;
  char *m_pInputMem;
  unsigned int m_Verbose;
};

// XXX: Experimental
InterpreterPass *CreateInterpreterPass(TargetBackend *pBackend,
                                       char *pInputMem,
                                       unsigned int pVerbose);

} // namespace of onnc

#endif
