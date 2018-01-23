//===- CArguRef.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_CARGU_REFERENCE_H
#define ONNC_SUPPORT_CARGU_REFERENCE_H
#include <onnc/ADT/StringList.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/Support/CArgu.h>

namespace onnc {

/** \class CArguRef
 *  \brief CArguRef is an abstraction of a reference to C-style arguments.
 *
 *  CArguRef is a list of argument strings to represent C-style arguments.
 *
 *  Unlike @ref CArgu, CArguRef doesn't own the memory space of argument
 *  strings.
 */
class CArguRef : private Uncopyable
{
public:
  typedef StringList::const_iterator const_iterator;
  typedef StringList::const_reference const_reference;
  typedef StringList::size_type size_type;

public:
  /// Construct CArguRef by given C-style arguments.
  CArguRef(int pArgc, const char* const* pArgv);

  /// Casting from CArgu.
  CArguRef(const CArgu& pArgu);

  /// Destructor
  ~CArguRef() { }

  /// Return the number of elements.
  unsigned int size() const { return m_Argus.size(); }

  /// @retval true There are no elements
  bool empty() const { return m_Argus.empty(); }

  /// Constant iterator begin
  const_iterator begin() const { return m_Argus.begin(); }

  /// Constant iterator end
  const_iterator end() const { return m_Argus.end(); }

  /// The first argument string
  const_reference front() const { return m_Argus.front(); }

  /// The last argument string
  const_reference back() const { return m_Argus.back(); }

  /// Return the reference to the argument at position @ref pIdx
  const_reference operator[](size_type pIdx) const { return m_Argus[pIdx]; }

protected:
  StringList m_Argus;
};

} // namespace of onnc

#endif
