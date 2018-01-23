//===- IOStream.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_IOSTREAM_H
#define ONNC_SUPPORT_IOSTREAM_H
#include <onnc/Support/OStream.h>

namespace onnc {

/// outs() - This returns a reference to a OStream for standard output.
/// Use it like: outs() << "foo" << "bar";
OStream& outs();

/// errs() - This returns a reference to a OStream for standard error.
/// Use it like: errs() << "foo" << "bar";
OStream& errs();

/// ins() - This returns a reference to an IStream for standard in.
/// Use it like: ins() >> x;
IStream& ins();

} // namespace of onnc

#endif
