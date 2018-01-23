//===- CEnv.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_CENV_H
#define ONNC_SUPPORT_CENV_H
#include <onnc/ADT/StringMap.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/Support/ErrorCode.h>
#include <ostream>

namespace onnc {

class Path;
class Environ;

/** \class CEnv
 *  \brief an abstraction of C-style environment variables.
 *
 *  C-style environment variable is a list of C strings. CEnv copies
 *  the C strings and provide setting functions. @ref CEnv::set creates or sets
 *  value to an environment variable. @ref CEnv::size is the number of environment
 *  variables.
 */
class CEnv
{
public:
  typedef StringMap<StringRef> EnvMap;
  typedef EnvMap::iterator iterator;
  typedef EnvMap::const_iterator const_iterator;

public:
  /// Default constructor.
  CEnv();

  /// Copy constructor.
  CEnv(const CEnv& pCopy);

  /// Copy variables from host environment.
  explicit CEnv(const Environ& pEnviron);

  /// Assignment operator.
  CEnv& operator=(const CEnv& pCopy);

  /// Destructor - releases all allocated strings.
  ~CEnv();

  /// Set environment variable @ref pName to @ref pValue.
  CEnv& set(const std::string& pName, const std::string& pValue);

  /// Obtain the value environment of the variable @ref pName
  /// If the variable doesn't exit, return empty StringRef object.
  StringRef get(const std::string& pName) const;

  /// Remove environment variable @ref pName
  /// If the variable doesn't exit, then do nothing.
  CEnv& unset(const std::string& pName);

  /// Return the number of entries.
  unsigned int size() const { return m_EnvMap.numOfEntries(); }

  /// @retval true Variable @ref pName exists
  bool has(const std::string& pName) const;

  iterator begin() { return m_EnvMap.begin(); }

  iterator end() { return m_EnvMap.end(); }

  const_iterator begin() const { return m_EnvMap.begin(); }

  const_iterator end() const { return m_EnvMap.end(); }

  /// Produce C-style environment variable list.
  void make(char**& pEnvp) const;

  /// Release produced @ref pEnvp memory.
  static void free(char**& pEnvp);

  void print(std::ostream& pOS) const;

protected:
  EnvMap m_EnvMap;
};

inline std::ostream& operator<<(std::ostream& pOS, const CEnv& pEnv)
{
  pEnv.print(pOS);
  return pOS;
}

} // namespace of onnc

#endif
