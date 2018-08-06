//===- Tensor.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_TENSOR_H
#define ONNC_IR_COMPUTE_TENSOR_H
#include <onnc/IR/Compute/Value.h>
#include <onnc/Config/ONNX.h>
#include <vector>

namespace onnc {

class Tensor : public onnc::Value
{
public:
  typedef std::vector<int64_t> Dimensions;

public:
  Tensor();

  Tensor(onnc::Value::Type pKind);

  Tensor(const std::string& pName, onnc::Value::Type pKind);

  Tensor(onnc::Value::Type pKind, xTensor& pAdaptee);

  unsigned int getNumOfDimensions() const { return m_Dimensions.size(); }

  unsigned int dimension(unsigned int pIdx) const { return m_Dimensions[pIdx]; }

  void setDimensions(const Dimensions& pD) { m_Dimensions = pD; }

  void print(std::ostream& pOS) const {
    pOS << getName() << "[]";
  }

protected:
  Dimensions m_Dimensions;
};

/** \class TensorT
 *  \brief TensorT is a placeholder of tensor in a network
 */
template<typename ValueType, Value::Type Kind>
class TensorT : public onnc::Tensor
{
public:
  typedef std::vector<ValueType> ValueList;

public:
  TensorT()
    : onnc::Tensor(Kind), m_Values() {
  }

  TensorT(const std::string& pName)
    : onnc::Tensor(pName, Kind), m_Values() {
  }

  TensorT(xTensor& pAdaptee)
    : onnc::Tensor(Kind, pAdaptee), m_Values() {
  }

  virtual ~TensorT() { }

  ValueList& getValues() { return m_Values; }

  const ValueList& getValues() const { return m_Values; }

private:
  ValueList m_Values;
};

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

} // namespace of onnc

#endif
