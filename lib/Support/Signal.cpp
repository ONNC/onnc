//===- Signal.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Signal.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Signal
//===----------------------------------------------------------------------===//
Signal::Signal(Kind pKind)
  : m_Kind(pKind){
}

void Signal::ResetAll()
{
  // Block all signals temporarily
  sigset_t allsignals;
  sigfillset(&allsignals);
  sigprocmask(SIG_BLOCK, &allsignals, NULL);

  // Discard all pending signals
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  for (int i = 1; i < NSIG; ++i)
    sigaction(i, &sa, NULL);

  // Reset all signals to default handler
  sa.sa_handler = SIG_DFL;
  for (int i = 1; i < NSIG; ++i)
    sigaction(i, &sa, NULL);

  // Ignore the user signals that may be sent so that we don't terminate
  // execed program by mistake
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);

  // Unblock all signals
  sigprocmask(SIG_UNBLOCK, &allsignals, NULL);
}
