//===- SigAction.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SIGNAL_ACTION_H
#define ONNC_SUPPORT_SIGNAL_ACTION_H

namespace onnc {

/** \class SigAction
 *  \brief provides software signal facilities.
 */
class SigAction
{
public:
  void install();
};

} // namespace of onnc

#endif
