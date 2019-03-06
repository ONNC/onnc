//===- CustomVisitor.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_CUSTOM_VISITOR_H
#define ONNC_IR_CUSTOM_VISITOR_H

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

#include <onnc/IR/ComputeVisitor.h>

namespace onnc {

template <
  typename VisitorType,
  typename ParentType = ComputeVisitor,
  typename = typename std::enable_if<
    std::is_class<VisitorType>::value
    && std::is_base_of<ComputeVisitor, ParentType>::value
  >::type
>
class CustomVisitor : public ParentType {
public:
  using BaseType = ComputeVisitor;

  template <typename... Args>
  CustomVisitor(Args&&... args)
    noexcept(std::is_nothrow_constructible<ParentType, Args...>::value)
    : ParentType(std::forward<Args>(args)...)
  { }

  virtual ~CustomVisitor() = default;

  typename ParentType::VisitorTypeID getVisitorID() const override {
    return id(); 
  }

  static typename ParentType::VisitorTypeID id() noexcept {
    static const char var{};
    return reinterpret_cast<typename ParentType::VisitorTypeID>(&var);
  }

  static bool classof(const ComputeVisitor *visitor) {
    return visitor != nullptr
           && visitor->getVisitorID() == id();
  }
};

} // namespace onnc

#endif
