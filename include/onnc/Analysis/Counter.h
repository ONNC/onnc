//===- Counter.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_COUNTER_H
#define ONNC_ANALYSIS_COUNTER_H
#include <onnc/ADT/StringRef.h>
#include <onnc/JSON/Group.h>

namespace onnc {

/** \class Counter
 *  \brief Counter provides an interface for statistical counting.
 *
 *  A counter object is an abstraction of json::Group object. It defines a
 *  format of json::Group to represent a counter when doing statistics.
 *  A counter has four entries:
 *  - "name" : [string],
 *  - "type" : [a magic integer],
 *  - "description" : [string],
 *  - "value" : [integer]
 *  Name should be a unique identity of a counter. Type is a fixed magic number.
 *  description is a string, and value keeps the value of a counter.
 *
 *  Users can use @ref Counter::Create function to create an empty counter. Or
 *  use @ref Counter::Get to get the counter object from a well-settle group.
 */
class Counter
{
public:
  Counter();

  Counter(const Counter& pOther);

  Counter& operator=(const Counter& pOther);

  virtual ~Counter() { /* all things in json::Object */ }

  Counter& reset(int pNumber = 0);

  Counter& increase();

  Counter& decrease();

  int value() const;

  StringRef name() const;

  StringRef getDescription() const;

  void setDescription(StringRef pDesc);

  operator int() const { return value(); }

  Counter& operator++() { return increase(); }

  /// The postfix form of ++.
  void operator++(int) { increase(); }

  Counter& operator--() { return decrease(); }

  /// The postfix form of --.
  void operator--(int) { decrease(); }

  /// If a counter wasn't given a group, it isn't active.
  bool isActive() const;

  /// follow the origin value designed by evan li.
  /// print "[name], [value], [description]\n"
  void print(std::ostream& pOS) const;

  /// Convert a well-settle json::Group object into a counter
  /// @return A inactive counter if the group is not well-settled.
  static Counter Get(json::Group pGroup);

  /// Create a counter and add corresponding group in @ref pParent.
  static Counter Create(json::Group& pParent, StringRef pName,
                        int pValue = 0, StringRef pDesc = "none");

  /// @retval true pGroup is a counter group.
  static bool IsCounter(const json::Group& pGroup);

protected:
  json::Group m_Group;
};

inline std::ostream& operator<<(std::ostream& pOS, const Counter& pCounter) {
  pCounter.print(pOS);
  return pOS;
}

} // namespace of onnc

#endif
