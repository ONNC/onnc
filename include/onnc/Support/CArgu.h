//===- CArgu.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_CARGU_H
#define ONNC_SUPPORT_CARGU_H
#include <onnc/ADT/StringRef.h>
#include <deque>
#include <string>
#include <cstring>
#include <ostream>

namespace onnc {

/** \class CArgu
 *  \brief CArgu is an abstraction of C-style arguments.
 *
 *  CArgu is a list of argument strings to represent C-style arguments.
 *
 *  CArgu owns memory spaces of all argument strings. Many member functions
 *  imply memory copy operations, such as @ref add() and @ref reset().
 */
class CArgu
{
public:
  typedef char** iterator;
  typedef const char* const* const_iterator;

  class reference
  {
  public:
    reference(char*& pTrg) : m_Trg(pTrg) { }

    // write operator
    reference& operator=(char* pAsg) {
      ::free(m_Trg);
      m_Trg = ::strdup(pAsg);
      return *this;
    }

    // write operator
    reference& operator=(const char* pAsg) {
      ::free(m_Trg);
      m_Trg = ::strdup(pAsg);
      return *this;
    }

    // read operator
    operator const char*() const { return m_Trg; }

    // read operator
    operator char*() { return m_Trg; }

  private:
    char*& m_Trg;
  };

  typedef const char* const_reference;

public:
  CArgu();

  CArgu(int pArgc, char* pArgv[]);

  CArgu(int pArgc, const char* pArgv[]);

  explicit CArgu(StringRef pCommandline);

  CArgu(const CArgu& pOther);

  CArgu(CArgu&& pOther);

  CArgu& operator=(const CArgu& pOther);

  CArgu& operator=(CArgu&& pOther);

  CArgu& assign(StringRef pCommandline);

  CArgu& assign(int pArgc, char* pArgv[]);

  CArgu& assign(int pArgc, const char* pArgv[]);

  CArgu& assign(const CArgu& pOther);

  CArgu& append(StringRef pCommandLine);

  CArgu& append(int pArgc, char* pArgv[]);

  CArgu& append(int pArgc, const char* pArgv[]);

  CArgu& append(const CArgu& pOther);

  virtual ~CArgu();

  unsigned int capacity() const { return m_Capacity; }

  unsigned int size() const { return m_Argc; }

  unsigned int count() const { return size(); }

  unsigned int argc() const { return size(); }

  char* const* argv() const { return m_Argv; }

  char** argv() { return m_Argv; }

  bool empty() const { return (0 == size()); }

  void clear();

  /// operator for writing. Check range.
  reference at(unsigned int pIdx);

  /// operator for reading. Check range
  const_reference at(unsigned int pIdx) const;

  /// operator for writing. No range checking.
  reference operator[](unsigned int pIdx);

  /// operator for reading. No range checking.
  const_reference operator[](unsigned int pIdx) const;

  /// copy pString at the end
  CArgu& push_back(const char* pString);

  CArgu& push_back(StringRef pString) {
    return this->push_back(pString.c_str());
  }

  CArgu& push_back(const std::string& pString) {
    return this->push_back(pString.c_str());
  }

  reference front() { return at(0); }

  const_reference front() const { return at(0); }

  reference back() { return at(size() - 1); }

  const_reference back() const { return at(size() - 1); }

  iterator begin() { return m_Argv; }

  iterator end() { return (m_Argv + size()); }

  const_iterator begin() const { return m_Argv; }

  const_iterator end() const { return (m_Argv + size()); }

  void print(std::ostream& pOS) const;

  void print(std::string& pStr) const;

  std::string getCommandLine() const;

  static void free(int& pArgc, char**& pArgv);

  /// Move data to @ref pArgc and @ref pArgv
  /// It doesn't allocate new memory space. Users must release storage
  /// manually. After the delegation, CArgu becomes empty.
  ///
  /// @param[out] pArgc Storage of argument count
  /// @param[out] pArgv Storage of argument vector
  void give(int& pArgc, char**& pArgv);

protected:
  CArgu& doAppend(char* pString);

protected:
  unsigned int m_Argc;
  char** m_Argv;
  unsigned int m_Capacity;
};

/// To expand C-style argument
/// @param[in]  pOrin   The argument to be expanded.
/// @param[out] pResult The result of expansion.
/// @retval true Succeed to expand
bool expand(CArgu& pResult, const CArgu& pOrin);

typedef std::deque<CArgu> CArguList;

} // namespace of onnc

namespace std {

inline std::ostream&
operator<<(std::ostream& pOS, onnc::CArgu::reference pRef)
{
  pOS << (const char*)(pRef);
  return pOS;
}

} // namespace of std

#endif
