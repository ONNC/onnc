//===- Signal.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SIGNAL_H
#define ONNC_SUPPORT_SIGNAL_H
#include <onnc/Support/ErrorCode.h>
#include <signal.h>
#include <assert.h>

namespace onnc {

/** \class Signal
 *  \brief Signal provides an abstract interface to handle system signal.
 */
class Signal
{
public:
  enum Kind : int {
    // ID     Name       Default Action  Description
    kHUP    = SIGHUP,    ///< terminate  terminal line hangup
    kINT    = SIGINT,    ///< terminate  interrupt program
    kQUIT   = SIGQUIT,   ///< core dump  quit program
    kILL    = SIGILL,    ///< core dump  illegal instruction
    kTRAP   = SIGTRAP,   ///< core dump  trace trap
    kABRT   = SIGABRT,   ///< core dump  abort program (formerly SIGIOT)
#ifdef SIGEMT
    kEMT    = SIGEMT,    ///< core dump  emulate instruction executed
#endif
    kFPE    = SIGFPE,    ///< core dump  floating-point exception
    kKILL   = SIGKILL,   ///< terminate  kill program
    kBUS    = SIGBUS,    ///< core dump  bus error
    kSEGV   = SIGSEGV,   ///< core dump  segmentation violation
    kSYS    = SIGSYS,    ///< core dump  non-existent system call invoked
    kPIPE   = SIGPIPE,   ///< terminate  write on a pipe with no reader
    kALRM   = SIGALRM,   ///< terminate  real-time timer expired
    kTERM   = SIGTERM,   ///< terminate  software termination signal
    kURG    = SIGURG,    ///< discare    urgent condition present on socket
    kSTOP   = SIGSTOP,   ///< stop       stop (cannot be caught or ignored)
    kTSTP   = SIGTSTP,   ///< stop       stop signal generated from keyboard
    kCONT   = SIGCONT,   ///< discare    continue after stop
    kCHLD   = SIGCHLD,   ///< discare    child status has changed
    kTTIN   = SIGTTIN,   ///< stop       background read attempted from control terminal
    kTTOU   = SIGTTOU,   ///< stop       background write attempted to control terminal
    kIO     = SIGIO,     ///< discare    I/O is possible on a descriptor
    kXCPU   = SIGXCPU,   ///< terminate  cpu time limit exceeded
    kXFSZ   = SIGXFSZ,   ///< terminate  file size limit exceeded
    kVTALRM = SIGVTALRM, ///< terminate  virtual time alarm
    kPROF   = SIGPROF,   ///< terminate  profiling timer alarm
    kWINCH  = SIGWINCH,  ///< discare    Window size change
#ifdef SIGINFO
    kINFO   = SIGINFO,   ///< discare    status request from keyboard
#endif
    kUSR1   = SIGUSR1,   ///< terminate  User defined signal 1
    kUSR2   = SIGUSR2,   ///< terminate  User defined signal 2
    kNull                ///< unsupported signal
  };

  typedef void (*HandleFunc)(int pSig);

  enum Flag {
    kNoChildStop = SA_NOCLDSTOP, ///< SIGCHLD signal generate only when child
                                 ///< process stops
    kNoChildWait = SA_NOCLDWAIT, ///< Doesn't create zombie process after
                                 ///< SIGCHLD signal is registered
    kOnStack     = SA_ONSTACK,   ///< deliver the signal to process on a signal
                                 ///< stack
    kNoDefer     = SA_NODEFER,   ///< Doesn't mask the signal when the handler
                                 ///< is running
    kResetHand   = SA_RESETHAND, ///< Reset signal handler to SIG_DFL when the
                                 ///< signal is delivered
    kRestart     = SA_RESTART,   ///< Automatically restart system calls
                                 ///< interrupted by the signal handler.
    kSigInfo     = SA_SIGINFO    ///< Use richer signal handler interface
  };

public:
  explicit Signal(Kind pKind);

  ~Signal() { }

  /// The kind of Signal.
  Kind kind() const { return m_Kind; }

  /// Reset all signals to its default handler
  static void ResetAll();

private:
  Signal() = delete;

private:
  Kind m_Kind;
};

} // namespace of onnc

#endif
