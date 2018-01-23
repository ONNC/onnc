//===- Alias.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_ALIAS_H
#define ONNC_OPTION_ALIAS_H
#include <onnc/Option/OptDefs.h>

namespace onnc {
namespace cl {

/** class cl::alias
 *  \brief alias set an alias for the other option.
 *
 *  We can use cl::alias to make --help option is a alias for the -h option.
 *  @code{.cpp}
 *  cl::opt<bool> Help("h", cl::kShort);
 *  cl::alias help("help", cl::kLong, cl::trueopt(Help));
 *  @endcode
 *
 *  @note decoration design pattern.
 */
class alias : public OptDefs
{
public:
  // The parameters have arbitrary position. Their order is no matter.
  template<class M0>
  explicit alias(const M0& pM0) : OptDefs(cl::kOptional), m_pTruth(NULL) {
    apply(pM0, this);
    done();
  }

  template<class M0, class M1>
  alias(const M0& pM0, const M1& pM1)
    : OptDefs(cl::kOptional), m_pTruth(NULL) {
    apply(pM0, this); apply(pM1, this);
    done();
  }

  template<class M0, class M1, class M2>
  alias(const M0& pM0, const M1& pM1, const M2& pM2)
    : OptDefs(cl::kOptional), m_pTruth(NULL) {
    apply(pM0, this); apply(pM1, this); apply(pM2, this);
    done();
  }

  template<class M0, class M1, class M2, class M3>
  alias(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3)
    : OptDefs(cl::kOptional), m_pTruth(NULL) {
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    done();
  }

  /// Set the true option and copy all attributes to the alias.
  void setAliasFor(OptDefs& pOption);

  /// add occurrence in the true option.
  /// @note decoration design pattern.
  bool addOccurrence(StringRef pA, StringRef pV)
  { return truth()->addOccurrence(pA, pV); }

  /// Get the number of occurrence on the command line
  /// @note decoration design pattern.
  unsigned int getNumOccurrence() const { return truth()->getNumOccurrence(); }

  /// @retval true If the option definition occurs on the command line
  /// @note decoration design pattern.
  bool hasOccurrence() const { return truth()->hasOccurrence(); }

private:
  bool handleOccurrence(StringRef pN, StringRef pV) {
    // use the true option's argument string, but its own value.
    return truth()->handleOccurrence(truth()->getArgStr(), pV);
  }

  void done();

  OptDefs* truth() { return m_pTruth; }

  const OptDefs* truth() const { return m_pTruth; }

private:
  OptDefs* m_pTruth;
};

/** \class trueopt
 *  \brief trueopt used to set the option an alias.
 */
class trueopt
{
public:
  explicit trueopt(OptDefs& pOption) : m_Option(pOption) {}

  template<class ALIAS>
  void apply(ALIAS& pAlias) const { pAlias.setAliasFor(m_Option); }

private:
  OptDefs& m_Option;
};

} // namespace of cl
} // namespace of onnc

#endif
