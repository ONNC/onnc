//===- SelfPipe.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SELF_PIPE_H
#define ONNC_SUPPORT_SELF_PIPE_H
#include <onnc/Support/Pipe.h>

namespace onnc {

/** \class SelfPipe
 *  \brief plays the self-pipe trick.
 *
 *  @see https://cr.yp.to/docs/selfpipe.html
 */
class SelfPipe : public Pipe
{
public:
  SelfPipe();

  virtual ~SelfPipe();

  /// send a notification to the reader.
  void notify();

  /// wait the notification from the reader.
  void wait();
};

} // namespace of onnc

#endif
