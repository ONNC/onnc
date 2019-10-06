//===- Tensor.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_TENSOR_H
#define ONNC_IR_COMPUTE_TENSOR_H
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/Support/Memory.h>
#include <onnc/Support/TypeTraits.h>

#include <vector>

namespace onnc {

class Tensor : public onnc::Value
{
public:
  using Dimension  = std::int64_t;
  using Dimensions = std::vector<Dimension>;
  using Size       = Dimensions::size_type;

public:
  Tensor();

  Tensor(onnc::Value::Type pKind);

  Tensor(const std::string& pName, onnc::Value::Type pKind);

  Tensor(onnc::Value::Type pKind, xTensor& pAdaptee);

  Size getNumOfDimensions() const { return m_Dimensions.size(); }

  Dimension dimension(Size pIdx) const { return m_Dimensions[pIdx]; }

  const Dimensions& getDimensions() const { return m_Dimensions; };

  void setDimensions(const Dimensions& pD) { m_Dimensions = pD; }

  void print(std::ostream& pOS) const override;

  virtual ~Tensor() = default;

  virtual Tensor* clone() const {
    auto copy = std::make_unique<Tensor>(*this);
    copy->setName(getName() + "<clone>");
    copy->clearDefine();
    copy->getUses().clear();
    return copy.release();
  }

  virtual Tensor* create() const {
    auto tensor = std::make_unique<Tensor>(getName() + "<clone>", kind());
    return tensor.release();
  }

protected:
  Dimensions m_Dimensions;
};

/** \class TensorT
 *  \brief TensorT is a placeholder of tensor in a network
 */
template<typename ValueTypeT, Value::Type Kind>
class TensorT : public onnc::Tensor
{
public:
  using ValueList  = std::vector<ValueTypeT>;
  using ValueType  = typename ValueList::value_type;
  using Size       = typename ValueList::size_type;

public:
  TensorT()
    : onnc::Tensor(Kind)
    , m_Values()
  {}

  TensorT(const std::string& pName)
    : onnc::Tensor(pName, Kind)
    , m_Values()
  {}

  TensorT(xTensor& pAdaptee)
    : onnc::Tensor(Kind, pAdaptee)
    , m_Values()
  {}

  TensorT(const TensorT&) = default;
  TensorT(TensorT&&)      = delete;

  virtual ~TensorT() = default;

  TensorT& operator=(const TensorT&) = default;
  TensorT& operator=(TensorT&&) = delete;

  ValueList& getValues() { return m_Values; }

  const ValueList& getValues() const { return m_Values; }

  Tensor* clone() const override {
    auto copy = std::make_unique<TensorT>(*this);
    copy->setName(getName() + "<clone>");
    copy->clearDefine();
    copy->getUses().clear();
    return copy.release();
  }

  Tensor* create() const override {
    auto tensor = std::make_unique<TensorT>(getName() + "<clone>");
    return tensor.release();
  }

  void print(std::ostream& pOS) const override;

private:
  ValueList m_Values;
};

template<typename ValueTypeT, Value::Type Kind>
void TensorT<ValueTypeT, Kind>::print(std::ostream& pOS) const {
  Tensor::print(pOS);
}

// clang-format off
typedef TensorT<float,       onnc::Value::kFloat>   FloatTensor;
typedef TensorT<float,       onnc::Value::kFloat16> Float16Tensor;
typedef TensorT<bool,        onnc::Value::kBoolean> BooleanTensor;
typedef TensorT<int8_t,      onnc::Value::kInt8>    Int8Tensor;
typedef TensorT<int16_t,     onnc::Value::kInt16>   Int16Tensor;
typedef TensorT<int32_t,     onnc::Value::kInt32>   Int32Tensor;
typedef TensorT<uint8_t,     onnc::Value::kUint8>   Uint8Tensor;
typedef TensorT<uint16_t,    onnc::Value::kUint16>  Uint16Tensor;
typedef TensorT<int64_t,     onnc::Value::kInt64>   Int64Tensor;
typedef TensorT<uint32_t,    onnc::Value::kUint32>  Uint32Tensor;
typedef TensorT<uint64_t,    onnc::Value::kUint64>  Uint64Tensor;
typedef TensorT<double,      onnc::Value::kDouble>  DoubleTensor;
typedef TensorT<std::string, onnc::Value::kString>  StringTensor;
// clang-format on

namespace internal {
template <typename T>
struct remove_ref_const
{
  using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
};

template <typename T>
using remove_ref_const_t = typename remove_ref_const<T>::type;

template <typename Iterator, typename Value, typename = void_t<>>
struct is_writable : std::false_type
{};

template <typename Iterator, typename Value>
struct is_writable<
  Iterator, Value,
  void_t<decltype(*std::declval<remove_ref_const_t<Iterator>>() = std::declval<remove_ref_const_t<Value>>())>>
  : std::true_type
{};

template <typename TensorType, typename OutputIterator>
void doCopyTensor(const TensorType& tensor, OutputIterator&& out, std::true_type)
{
  using std::begin;
  using std::copy;
  using std::end;

  copy(begin(tensor.getValues()), end(tensor.getValues()), std::forward<OutputIterator>(out));
}

template <typename TensorType, typename OutputIterator>
void doCopyTensor(const TensorType&, OutputIterator&&, std::false_type)
{
  assert(false && "should not reach here, this is dummy function for preventing compile error");
}

template <typename TensorType, typename OutputIterator>
void copyTensorImpl(const TensorType& tensor, OutputIterator&& out)
{
  using selector_type = typename std::conditional<is_writable<OutputIterator, typename TensorType::ValueType>::value,
                                                  std::true_type, std::false_type>::type;
  doCopyTensor(tensor, std::forward<OutputIterator>(out), selector_type{});
}

template <typename OutputIterator>
void copyTensor(const Tensor& tensor, OutputIterator&& out)
{
  assert(false && "should not reach here, no matched tensor type");
}

template <typename FirstTensor, typename... RestTensors, typename OutputIterator>
void copyTensor(const Tensor& tensor, OutputIterator&& out)
{
  if (const auto* targetTensor = dynamic_cast<const FirstTensor*>(&tensor)) {
    copyTensorImpl(*targetTensor, std::forward<OutputIterator>(out));
  } else {
    copyTensor<RestTensors...>(tensor, std::forward<OutputIterator>(out));
  }
}
} // namespace internal

template <typename OutputIterator>
void copy(const Tensor& tensor, OutputIterator&& out)
{
  internal::copyTensor<BooleanTensor, FloatTensor, Float16Tensor, Int8Tensor, Int16Tensor, Int32Tensor, Int64Tensor,
                       Uint8Tensor, Uint16Tensor, Uint32Tensor, Uint64Tensor, DoubleTensor, StringTensor>(
    tensor, std::forward<OutputIterator>(out));
}

Tensor::Size size(const Tensor& tensor);

// Convert Tensor to Container, copy all values into that container
template <
  typename Container,
  typename = void_t<decltype(Container(size(std::declval<Tensor>()))), decltype(begin(std::declval<Container>())),
                    typename std::enable_if<std::is_move_constructible<Container>::value>::type>>
Container to_(const Tensor& tensor)
{
  using std::begin;

  Container c(size(tensor));
  copy(tensor, begin(c));
  return c;
}

} // namespace onnc

#endif
