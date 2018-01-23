//===- AsyncPipe.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ASYNCHRONOUS_PIPE_H
#define ONNC_SUPPORT_ASYNCHRONOUS_PIPE_H
#include <onnc/Support/Pipe.h>

namespace onnc {

/** \class AsyncPipe
 *  \brief AsyncPipe creates a pipe whose both sides are asynchronous.
 *
 *  Read and write operators on AsyncPipe is non-blocking.
 */
class AsyncPipe : public Pipe
{
public:
  AsyncPipe();

  virtual ~AsyncPipe() { }
};

} // namespace of onnc

#endif
