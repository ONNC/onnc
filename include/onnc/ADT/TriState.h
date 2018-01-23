//===- TriState.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_TRISTATE_H
#define ONNC_ADT_TRISTATE_H

namespace onnc {

// Other safe-to-copy-by-value common option types.
enum TriState {
  TriTrue,
  TriFalse,
  TriUnset 
};

} // namespace of onnc

#endif
