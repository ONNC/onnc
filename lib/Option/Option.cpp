//===- Option.cpp ----------------------------------------------------------==//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/Option.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Option
//===----------------------------------------------------------------------===//
cl::Option::Option(const OptDefs& pCategory)
  : m_Category(pCategory), m_NumOccurrence(0) {
}

//===----------------------------------------------------------------------===//
// OptDefs
//===----------------------------------------------------------------------===//
bool cl::Option::addOccurrence(StringRef pName, StringRef pValue)
{
  ++m_NumOccurrence;

  // check number of occurrences.
  // We keep the checking here rather than legalization phase because the
  // errors of occurrence it better to be recognized as early as possible.
  switch (category().getOccurrence()) {
    case cl::kOptional:
      if (m_NumOccurrence > 1) {
        error(category(), "may only occur zero or one times!");
        return false;
      }
      break;
    case cl::kRequired:
      if (m_NumOccurrence > 1) {
        error(category(), "may occur exactly once!");
        return false;
      }
      break;
    case cl::kZeroOrMore:
    case cl::kOneOrMore:
      break;
  }

  return this->handleOccurrence(pName, pValue);
}
