//===- FileDescriptor.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_DESCRIPTOR_H
#define ONNC_SUPPORT_FILE_DESCRIPTOR_H
#include <onnc/Config/Config.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <cstdio>

namespace onnc {

typedef void * HANDLE;

typedef int FD;

/* Standard file descriptors. */
#ifndef STDIN_FILENO
#define STDIN_FILENO 0 /* Standard input. */
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1 /* Standard output. */
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2 /* Standard error output. */
#endif

enum StdFD {
  kStdIn  = STDIN_FILENO,
  kStdOut = STDOUT_FILENO,
  kStdErr = STDERR_FILENO
};

} // namespace of onnc

#endif
