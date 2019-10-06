//===- TypeTraits.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_TYPE_TRAITS_H_INCLUDED
#define ONNC_SUPPORT_TYPE_TRAITS_H_INCLUDED

#include <iterator>
#include <type_traits>
#include <utility>

namespace onnc {

template <typename...>
using void_t = void;

} // namespace onnc

#endif
