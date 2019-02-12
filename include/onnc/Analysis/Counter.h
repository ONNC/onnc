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
 *
 *  In case users want to claim a Counter object and then posphone assignment of
 *  value, we also provide default constructor and assign operator for this case.
 *  @ref Counter::isValid return true if a Counter object already has well-settled
 *  group.
 */
class Counter
{
public:
  /// Default constructor. This create an empty group object without any value.
  /// In this case, the counter object is invalid and users can not increase/
  /// decrease its value.
  Counter();

  /// Copy constructor. Copy the value of the counter from the others. Since
  /// a Counter object is just an abstract interface of a json::Group object,
  /// this copy shall not create new json::Group object; the copy operator
  /// just create a new representation of the original json::Group object.
  Counter(const Counter& pOther);

private:
  /// Conversion constructor. This create a counter with an associated group object.
  /// The @ref Counter::Create() method can create Counter objects by invoking this
  /// constructor, and the created counter objects are all valid.
  explicit Counter(json::Group group);

public:
  /// Assignment. Since a Counter object is just an abstract interface of a
  /// json::Group object, this assignment shall not create new json::Group object;
  /// the assignment operator just create a new representation of the original
  /// json::Group object.
  Counter& operator=(const Counter& pOther);

  virtual ~Counter() { /* all things in json::Object */ }

  /// reset the counter to @ref pNumber.
  Counter& reset(int pNumber = 0);

  /// increase one unit
  Counter& increase();

  /// decrease one unit
  Counter& decrease();

  /// The number of the counter
  int value() const;

  /// The identity of the counter.
  StringRef name() const;

  /// The description of the counter.
  StringRef getDescription() const;

  /// Set description.
  void setDescription(StringRef pDesc);

  /// Casting operator. Cast to the value.
  operator int() const { return value(); }

  Counter& operator++() { return increase(); }

  /// The postfix form of ++.
  void operator++(int) { increase(); }

  Counter& operator--() { return decrease(); }

  /// The postfix form of --.
  void operator--(int) { decrease(); }

  /// If a counter wasn't given a group, it isn't active.
  /// A Counter object created by default constructor is invalid.
  bool isValid() const;

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
