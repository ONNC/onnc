//===- If.h ---------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_IF_H
#define ONNC_ADT_IF_H

namespace onnc {

template<bool C, typename T1, typename T2>
struct if_c {
  typedef T1 type;
};

template<typename T1, typename T2>
struct if_c<false, T1, T2>
{
  typedef T2 type;
};

} // namespace of onnc

#endif
