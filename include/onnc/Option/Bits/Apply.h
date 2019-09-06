//===- Apply.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Apply methods transfer the operation to the correct methods.
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_BITS_APPLY_H
#define ONNC_OPTION_BITS_APPLY_H
#include <onnc/Option/OptDefs.h>

namespace onnc {
namespace cl {

//===----------------------------------------------------------------------===//
// Applicator
//===----------------------------------------------------------------------===//
template<class Mod>
struct Applicator
{
  template<class Opt>
  static void opt(const Mod& M, Opt& O) { M.apply(O); }
};

//===----------------------------------------------------------------------===//
// Partial specifications
//===----------------------------------------------------------------------===//
template<unsigned n>
struct Applicator<char[n]>
{
  template<class Opt>
  static void opt(const char* Str, Opt& O) { O.setArgStr(Str); }
};

template<unsigned n>
struct Applicator<const char[n]>
{
  template<class Opt>
  static void opt(const char* Str, Opt& O) { O.setArgStr(Str); }
};

template<>
struct Applicator<const char*>
{
  template<class Opt>
  static void opt(const char* Str, Opt& O) { O.setArgStr(Str); }
};

template<>
struct Applicator<std::string>
{
  template<class Opt>
  static void opt(const std::string& Str, Opt& O) { O.setArgStr(Str); }
};

template<>
struct Applicator<Occurrence>
{
  static void opt(Occurrence NO, OptDefs& pO) {
    pO.setOccurrence(NO);
  }
};

template<>
struct Applicator<enum Prefix>
{
  static void opt(enum Prefix pOF, OptDefs& pO) {
    pO.setPrefix(pOF);
  }
};

template<>
struct Applicator<enum ValueOccurrence>
{
  static void opt(enum ValueOccurrence pVF, OptDefs& pO) {
    pO.setValueOccurrence(pVF);
  }
};

template<>
struct Applicator<enum Delimiter>
{
  static void opt(enum Delimiter pOP, OptDefs& pO) {
    pO.setDelimiter(pOP);
  }
};

//===----------------------------------------------------------------------===//
// apply method - Apply a modifier to an option in a type safe way.
//===----------------------------------------------------------------------===//
template<class M, class O>
void apply(const M& pMethod, O* pOption)
{
  Applicator<M>::opt(pMethod, *pOption);
}

} // namespace of cl
} // namespace of onnc

#endif
