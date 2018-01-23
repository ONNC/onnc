//===- ArrayRef.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ARRAYREF_H
#define ONNC_ADT_ARRAYREF_H
#include <vector>
#include <cassert>

namespace onnc {

/** \class ArrayRef
 *  \brief represents a constant reference to an array.
 *
 *  ArrayRef represents a constant reference to an array (0 or more elements
 *  consecutively in memory), i.e. a start pointer and a length.  It allows
 *  various APIs to take consecutive elements easily and conveniently.
 * 
 *  This class does not own the underlying data, it is expected to be used in
 *  situations where the data resides in some other buffer, whose lifetime
 *  extends past that of the ArrayRef. For this reason, it is not in general
 *  safe to store an ArrayRef.
 * 
 *  This is intended to be trivially copyable, so it should be passed by
 *  value.
 */
template<typename T>
class ArrayRef
{
public:
  typedef const T *iterator;
  typedef const T *const_iterator;
  typedef size_t size_type;

  typedef std::reverse_iterator<iterator> reverse_iterator;

public:
  /// @name Constructors
  /// @{

  /// Construct an empty ArrayRef.
  ArrayRef() : m_Data(nullptr), m_Length(0) {}

  /// Construct an ArrayRef from a single element.
  ArrayRef(const T &OneElt)
    : m_Data(&OneElt), m_Length(1) {}

  /// Construct an ArrayRef from a pointer and length.
  ArrayRef(const T *data, size_t length)
    : m_Data(data), m_Length(length) {}

  /// Construct an ArrayRef from a range.
  ArrayRef(const T *begin, const T *end)
    : m_Data(begin), m_Length(end - begin) {}

  /// Construct an ArrayRef from a std::vector.
  template<typename A>
  ArrayRef(const std::vector<T, A> &Vec)
    : m_Data(Vec.data()), m_Length(Vec.size()) {}

  /// Construct an ArrayRef from a C array.
  template <size_t N>
  constexpr ArrayRef(const T (&Arr)[N])
    : m_Data(Arr), m_Length(N) {}

  /// Construct an ArrayRef from a std::initializer_list.
  ArrayRef(const std::initializer_list<T> &Vec)
  : m_Data(Vec.begin() == Vec.end() ? (T*)nullptr : Vec.begin()),
    m_Length(Vec.size()) {}

  /// Construct an ArrayRef<const T*> from ArrayRef<T*>. This uses SFINAE to
  /// ensure that only ArrayRefs of pointers can be converted.
  template <typename U>
  ArrayRef(const ArrayRef<U *> &A,
           typename std::enable_if<
               std::is_convertible<U *const *, T const *>::value>::type* = 0)
    : m_Data(A.data()), m_Length(A.size()) {}

  /// Construct an ArrayRef<const T*> from std::vector<T*>. This uses SFINAE
  /// to ensure that only vectors of pointers can be converted.
  template<typename U, typename A>
  ArrayRef(const std::vector<U *, A> &Vec,
           typename std::enable_if<
               std::is_convertible<U *const *, T const *>::value>::type* = 0)
    : m_Data(Vec.data()), m_Length(Vec.size()) {}

  /// @}
  /// @name Simple Operations
  /// @{

  iterator begin() const { return m_Data; }
  iterator end() const { return m_Data + m_Length; }

  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() const { return reverse_iterator(begin()); }

  /// empty - Check if the array is empty.
  bool empty() const { return m_Length == 0; }

  const T *data() const { return m_Data; }

  /// size - Get the array size.
  size_t size() const { return m_Length; }

  /// front - Get the first element.
  const T &front() const {
    assert(!empty());
    return m_Data[0];
  }

  /// back - Get the last element.
  const T &back() const {
    assert(!empty());
    return m_Data[m_Length-1];
  }

  // copy - Allocate copy in Allocator and return ArrayRef<T> to it.
  template <typename Allocator> ArrayRef<T> copy(Allocator &A) {
    T *Buff = A.template Allocate<T>(m_Length);
    std::uninitialized_copy(begin(), end(), Buff);
    return ArrayRef<T>(Buff, m_Length);
  }

  /// equals - Check for element-wise equality.
  bool equals(ArrayRef RHS) const {
    if (m_Length != RHS.m_Length)
      return false;
    return std::equal(begin(), end(), RHS.begin());
  }

  /// slice(n) - Chop off the first N elements of the array.
  ArrayRef<T> slice(unsigned N) const {
    assert(N <= size() && "Invalid specifier");
    return ArrayRef<T>(data()+N, size()-N);
  }

  /// slice(n, m) - Chop off the first N elements of the array, and keep M
  /// elements in the array.
  ArrayRef<T> slice(unsigned N, unsigned M) const {
    assert(N+M <= size() && "Invalid specifier");
    return ArrayRef<T>(data()+N, M);
  }

  // \brief Drop the last \p N elements of the array.
  ArrayRef<T> drop_back(unsigned N = 1) const {
    assert(size() >= N && "Dropping more elements than exist");
    return slice(0, size() - N);
  }

  /// @}
  /// @name Operator Overloads
  /// @{
  const T &operator[](size_t Index) const {
    assert(Index < m_Length && "Invalid index!");
    return m_Data[Index];
  }

  /// @}
  /// @name Expensive Operations
  /// @{
  std::vector<T> vec() const {
    return std::vector<T>(m_Data, m_Data+m_Length);
  }

  /// @}
  /// @name Conversion operators
  /// @{
  operator std::vector<T>() const {
    return std::vector<T>(m_Data, m_Data+m_Length);
  }

  /// @}
private:
  /// The start of the array, in an external buffer.
  const T *m_Data;

  /// The number of elements.
  size_type m_Length;
};

/** \class MutableArrayRef
 *  \brief represents a mutable reference to an array.
 *
 *  MutableArrayRef represents a mutable reference to an array (0 or more
 *  elements consecutively in memory), i.e. a start pointer and a length.  It
 *  allows various APIs to take and modify consecutive elements easily and
 *  conveniently.
 * 
 *  This class does not own the underlying data, it is expected to be used in
 *  situations where the data resides in some other buffer, whose lifetime
 *  extends past that of the MutableArrayRef. For this reason, it is not in
 *  general safe to store a MutableArrayRef.
 * 
 *  This is intended to be trivially copyable, so it should be passed by
 *  value.
 */
template<typename T>
class MutableArrayRef : public ArrayRef<T>
{
public:
  typedef T *iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;

public:
  /// Construct an empty MutableArrayRef.
  MutableArrayRef() : ArrayRef<T>() {}

  /// Construct an MutableArrayRef from a single element.
  MutableArrayRef(T &OneElt) : ArrayRef<T>(OneElt) {}

  /// Construct an MutableArrayRef from a pointer and length.
  MutableArrayRef(T *data, size_t length)
    : ArrayRef<T>(data, length) {}

  /// Construct an MutableArrayRef from a range.
  MutableArrayRef(T *begin, T *end) : ArrayRef<T>(begin, end) {}

  /// Construct a MutableArrayRef from a std::vector.
  MutableArrayRef(std::vector<T> &Vec)
  : ArrayRef<T>(Vec) {}

  /// Construct an MutableArrayRef from a C array.
  template <size_t N>
  constexpr MutableArrayRef(T (&Arr)[N])
    : ArrayRef<T>(Arr) {}

  T *data() const { return const_cast<T*>(ArrayRef<T>::data()); }

  iterator begin() const { return data(); }
  iterator end() const { return data() + this->size(); }

  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() const { return reverse_iterator(begin()); }

  /// front - Get the first element.
  T &front() const {
    assert(!this->empty());
    return data()[0];
  }

  /// back - Get the last element.
  T &back() const {
    assert(!this->empty());
    return data()[this->size()-1];
  }

  /// slice(n) - Chop off the first N elements of the array.
  MutableArrayRef<T> slice(unsigned N) const {
    assert(N <= this->size() && "Invalid specifier");
    return MutableArrayRef<T>(data()+N, this->size()-N);
  }

  /// slice(n, m) - Chop off the first N elements of the array, and keep M
  /// elements in the array.
  MutableArrayRef<T> slice(unsigned N, unsigned M) const {
    assert(N+M <= this->size() && "Invalid specifier");
    return MutableArrayRef<T>(data()+N, M);
  }

  MutableArrayRef<T> drop_back(unsigned N) const {
    assert(this->size() >= N && "Dropping more elements than exist");
    return slice(0, this->size() - N);
  }

  /// @}
  /// @name Operator Overloads
  /// @{
  T &operator[](size_t Index) const {
    assert(Index < this->size() && "Invalid index!");
    return data()[Index];
  }
};

/// @name ArrayRef Convenience constructors
/// @{

/// Construct an ArrayRef from a single element.
template<typename T>
ArrayRef<T> makeArrayRef(const T &OneElt) {
  return OneElt;
}

/// Construct an ArrayRef from a pointer and length.
template<typename T>
ArrayRef<T> makeArrayRef(const T *data, size_t length) {
  return ArrayRef<T>(data, length);
}

/// Construct an ArrayRef from a range.
template<typename T>
ArrayRef<T> makeArrayRef(const T *begin, const T *end) {
  return ArrayRef<T>(begin, end);
}

/// Construct an ArrayRef from a std::vector.
template<typename T>
ArrayRef<T> makeArrayRef(const std::vector<T> &Vec) {
  return Vec;
}

/// Construct an ArrayRef from an ArrayRef (no-op) (const)
template <typename T> ArrayRef<T> makeArrayRef(const ArrayRef<T> &Vec) {
  return Vec;
}

/// Construct an ArrayRef from an ArrayRef (no-op)
template <typename T> ArrayRef<T> &makeArrayRef(ArrayRef<T> &Vec) {
  return Vec;
}

/// Construct an ArrayRef from a C array.
template<typename T, size_t N>
ArrayRef<T> makeArrayRef(const T (&Arr)[N]) {
  return ArrayRef<T>(Arr);
}

/// @}
/// @name ArrayRef Comparison Operators
/// @{

template<typename T>
inline bool operator==(ArrayRef<T> LHS, ArrayRef<T> RHS) {
  return LHS.equals(RHS);
}

template<typename T>
inline bool operator!=(ArrayRef<T> LHS, ArrayRef<T> RHS) {
  return !(LHS == RHS);
}

/// @}

// ArrayRefs can be treated like a POD type.
template <typename T> struct isPodLike;
template <typename T> struct isPodLike<ArrayRef<T> > {
  static const bool value = true;
};

} // end of namespace onnc

#endif
