//===- Environ.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ENVIRON_H
#define ONNC_SUPPORT_ENVIRON_H
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/CEnv.h>

namespace onnc {

/** \class Environ
 *  \brief provides abstraction of host environment variables.
 *
 *  Environ provides abstraction of host environment variables. Handle Environ
 *  object is equivalent to handle with \code extern char** environ; \endcode.
 *
 *  Although Environ may have multiple objects in the system, it is by means
 *  a kind of global variable.
 */
class Environ
{
public:
  typedef size_t size_type;

  typedef StringRef const_reference;

  class iterator
  {
  public:
    iterator() : m_Idx(-1) { }

    iterator(int pIdx) : m_Idx(pIdx) { }

    iterator(const iterator& pCopy) : m_Idx(pCopy.m_Idx) { }

    iterator& operator=(const iterator& pCopy);

    iterator& operator++();

    iterator& operator--();

    iterator operator++(int);

    iterator operator--(int);

    const_reference operator*() { return Environ::at(m_Idx); }

    const_reference key() const;

    const_reference value() const;

    bool operator==(const iterator& pOther) const { return (m_Idx == pOther.m_Idx); }

    bool operator!=(const iterator& pOther) const { return !(*this == pOther); }

  private:
    int m_Idx;
  };

public:
  /// Return the number of environment variables.
  /// @note Time complexity is O(n).
  static unsigned int size();

  static const_reference at(size_type pIdx);

  const_reference operator[](size_type pIdx) const { return at(pIdx); }

  static iterator begin() { return iterator(0); }

  static iterator end() { return iterator(-1); }

  /// Inserts or resets the environment variable @ref pName in the \b current
  /// environment list.  If the variable @ref pName does not exist in the list,
  /// it is inserted with the given value @ref pValue. If the variable does exist,
  /// the @ref pOverwrite is tested; if @ref pOverwrite is false, the variable is
  /// not reset, otherwise, it is reset to given @ref pValue.
  ///
  /// @retval true Set well
  static bool set(StringRef pName, StringRef pValue, bool pOverwrite = true);

  /// Deletes all instances of the variable name pointed to by @ref pname
  /// from the list.
  static bool unset(StringRef pName);

  /// Take an argument of the form \code pName=pValue \codeend and is equivalent
  /// to:
  /// \code
  ///      set(pName, pValue, true);
  /// \codeend
  static bool insert(StringRef pName, StringRef pValue) { return set(pName, pValue, true); }

  /// Insert all variables of @pCEnv into the global variables. If all insertion successes, then
  /// the returned iterator is the end of @ref pCEnv.
  ///
  /// @param[in] pOverwrite Overwrite the variable if it exists.
  /// @return The end of inserted variable, i.e., the first variable not been inserted.
  static CEnv::const_iterator insert(const CEnv& pCEnv, bool pOverwrite = true);

  /// Obtains the current value of the environment variable @ref pName.
  static const_reference get(StringRef pName);

  /// @retval true Variable @ref pName exists
  static bool has(StringRef pName);

  /// Return the underline data structure - extern char** environ.
  static char** raw();
};

} // namespace of onnc

#endif
