//===- Statistics.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_STATISTICS_H
#define ONNC_INTERPRETER_STATISTICS_H
#include <onnc/Core/ModulePass.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace onnc {
// TODO: Use Data Pass.
/** \class [Experimental] Statistics Data Pass.
 *  \brief Statistics API. It is experimental and stay in ONNI because we are
 *         tring to find out what's the best Statistics API design.
 */
template<typename Data>
class Statistics : public ModulePass
{
public:
  ~Statistics() override {}

  operator Data&() { return m_Data; }
  operator const Data&() const { return m_Data; }
  void print(OStream &pOS, const Module *pModule) const override {
    printStatisticData(pOS);
  }

protected:
  Statistics(char& pPassID)
    : ModulePass(pPassID) { }

private:
  Data m_Data;

  virtual void printStatisticData(OStream &pOS) const = 0;

  /// Virtual method overridden by subclasses to process the module
  /// being operated on.
  Pass::ReturnType runOnModule(Module &pModule) override {
    return kModuleNoChanged;
  };
};

template<typename Key, typename Data>
class OneDStatistic : public Statistics<std::unordered_map<Key, Data>>
{
public:
  ~OneDStatistic() override {}

protected:
  explicit OneDStatistic(char& pPassID, std::string pPrefix)
    : Statistics<std::unordered_map<Key, Data>>(pPassID), m_Prefix(pPrefix) { }

  const char *SEP = " |";
  std::string m_Prefix;

  void printSeparator(OStream &pOS, std::pair<int, int> pWidth) const {
    pOS << m_Prefix
        << std::setfill('-')
        << std::setw(pWidth.first) << '-'
        << "-+"
        << std::setw(pWidth.second) << '-'
        << std::setfill(' ')
        << std::endl;
  }

private:
  void printStatisticData(OStream &pOS) const override {
    std::pair<int, int> width = printHeader(pOS);
    const std::unordered_map<Key, Data> &data = *this;
    for (auto &c : data) {
      pOS << m_Prefix << std::setw(width.first) << c.first << SEP
          << std::setw(width.second) << c.second << std::endl;
    }
    printFooter(pOS);
  }
  virtual std::pair<int, int> printHeader(OStream &pOS) const {
    return {0, 0};
  };
  virtual void printFooter(OStream &pOS) const {};
};

// TODO: Define some macros for easy using.

} // namespace of onnc

#endif
