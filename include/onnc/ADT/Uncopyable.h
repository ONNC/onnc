//===- Uncopyable.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_UNCOPYABLE_H
#define ONNC_ADT_UNCOPYABLE_H

namespace onnc {

/** \class Uncopyable
 *  \brief Uncopyable provides the base class to forbid copy operations.
 *
 */
class Uncopyable
{
protected:
  Uncopyable() { }
  ~Uncopyable() { }

private:
  Uncopyable(const Uncopyable&); /// NOT TO IMPLEMENT
  Uncopyable& operator=(const Uncopyable&); /// NOT TO IMPLEMENT
};

} // namespace of onnc

#endif

