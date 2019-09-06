//===- OptimizationsUtils.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTIMIZATIONS_UTILS
#define ONNC_OPTIMIZATIONS_UTILS

#include <onnc/IR/Compute/Tensor.h>

#ifndef PP_UNWRAP
#  define PP_UNWRAP(list) PP_UNWRAP_(list)
#  define PP_UNWRAP_(list) PP_UNWRAP__ list
#  define PP_UNWRAP__(...) __VA_ARGS__
#endif

#ifndef FP_TENSORTYPE_LIST
#  define FP_TENSORTYPE_LIST \
    (FloatTensor, Float16Tensor, DoubleTensor)
#endif

#ifndef NUMERIC_TENSORTYPE_LIST
#  define NUMERIC_TENSORTYPE_LIST \
    (FloatTensor, Float16Tensor, Int8Tensor, Int16Tensor, \
     Int32Tensor, Uint8Tensor, Uint16Tensor, Int64Tensor, \
     Uint32Tensor, Uint64Tensor, DoubleTensor)
#endif

#ifndef ALL_TENSORTYPE_LIST
#  define ALL_TENSORTYPE_LIST \
    (FloatTensor, Float16Tensor, BooleanTensor, Int8Tensor, \
     Int16Tensor, Int32Tensor, Uint8Tensor, Uint16Tensor, \
     Int64Tensor, Uint32Tensor, Uint64Tensor, DoubleTensor, \
     StringTensor)
#endif
namespace onnc {
namespace internal {

template <typename F>
void visitTensorWithFunc(Tensor* inputTensor, const F& func) {
  assert(0 && "Should not reach here, inputTensor did not match any Tensor type");
}

template <typename FirstTensorType, typename... RestTensorTypes, typename F>
void visitTensorWithFunc(Tensor* inputTensor, const F& func) {
  if (inputTensor->kind() == FirstTensorType().kind()) {
    auto castedTensor = static_cast<FirstTensorType*>(inputTensor);
    func(castedTensor);
  } else {
    visitTensorWithFunc<RestTensorTypes...>(inputTensor, func);
  }
}

template <typename F>
void visitTensorWithFunc(const Tensor* inputTensor, const F& func)
{
  assert(0 && "Should not reach here, inputTensor did not match any Tensor type");
}

template <typename FirstTensorType, typename... RestTensorTypes, typename F>
void visitTensorWithFunc(const Tensor* inputTensor, const F& func)
{
  if (inputTensor->kind() == FirstTensorType().kind()) {
    auto castedTensor = static_cast<const FirstTensorType*>(inputTensor);
    func(castedTensor);
  } else {
    visitTensorWithFunc<RestTensorTypes...>(inputTensor, func);
  }
}

class BroadcastingTo1D {
public:
  explicit BroadcastingTo1D(const Tensor::Dimension & M): m_Dim(M) {}

  // Broadcasting the values in Tensor to 1D: (M)
  // Support tensor in shape of (1), (1, 1), (1, M), (M)
  template <typename TensorType>
  void operator() (TensorType* const tensor) const {
    const typename Tensor::Dimensions& tensorShape = tensor->getDimensions();

    // Check whether tensor is broadcastable to (M)
    assert(!tensorShape.empty());
    if (tensorShape.size() == 1) {
      assert(tensorShape[0] == 1 || tensorShape[0] == m_Dim);
    } else if (tensorShape.size() == 2) {
      assert(tensorShape[0] == 1);
      assert(tensorShape[1] == 1 || tensorShape[1] == m_Dim);
    } else {
      assert(0 && "Too many dimensions (> 2)");
    }

    if (*tensorShape.rbegin() != m_Dim) {
      // case: (1) and (1, 1)
      assert(tensor->getValues().size() == 1);
      typename TensorType::ValueType val = tensor->getValues()[0];
      tensor->getValues() = typename TensorType::ValueList(m_Dim, val);
    }
    assert(tensor->getValues().size() == m_Dim);
    tensor->setDimensions({m_Dim});
  }

private:
  Tensor::Dimension m_Dim;
};

class ApplyTransOn2DTensor {
public:
  // Apply transpose on a 2D tensor
  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    assert(tensor->getNumOfDimensions() == 2);
    typename Tensor::Dimension N, M;
    N = tensor->getDimensions()[0];
    M = tensor->getDimensions()[1];

    assert(static_cast<Tensor::Size>(N * M) == tensor->getValues().size());
    typename TensorType::ValueList transList(tensor->getValues().size());
    for (typename Tensor::Size i = 0; i < N; ++i) {
      for (typename Tensor::Size j = 0; j < M; ++j) {
        transList[j * N + i] = tensor->getValues()[i * M + j];
      }
    }
    tensor->getValues() = std::move(transList);

    typename Tensor::Dimensions resDims = {M, N};
    tensor->setDimensions(resDims);
  }
};

class ApplySplitOnChannel {
public:
  // Apply split on second dimension C of (N, C, H, W)
  ApplySplitOnChannel(Tensor* destTensor,
                      Tensor::Size dim,
                      IntsAttr::ValueType st,
                      IntsAttr::ValueType ed):
    m_DestTensor(destTensor), m_Dim(dim), m_St(st), m_Ed(ed) {}

  // Split the values from st to ed, store the values
  // to m_DestTensor
  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    assert(!tensor->getDimensions().empty());
    assert(!tensor->getValues().empty());

    auto destTensor = dynamic_cast<TensorType*>(m_DestTensor);
    assert(destTensor != nullptr);
    assert(destTensor->getValues().empty());

    typename Tensor::Dimensions dimOffsets(tensor->getNumOfDimensions(), 1);
    for (typename Tensor::Size idx = dimOffsets.size()-1; idx >= 1; --idx) {
      dimOffsets[idx-1] = dimOffsets[idx] * tensor->getDimensions()[idx];
    }

    typename Tensor::Dimension destSize = 1;
    typename Tensor::Dimensions splitDims;
    for (typename Tensor::Size idx = 0; idx < tensor->getNumOfDimensions(); ++idx) {
      if (idx == m_Dim) destSize *= (m_Ed - m_St);
      else destSize *= tensor->getDimensions()[idx];

      // check whether the dimensions are correct if the dimensions were set
      if (!destTensor->getDimensions().empty()) {
        if (idx == m_Dim) assert(destTensor->getDimensions()[idx] == m_Ed - m_St);
        else assert(destTensor->getDimensions()[idx] == tensor->getDimensions()[idx]);
      }
      else {
        if (idx == m_Dim) splitDims.emplace_back(m_Ed - m_St);
        else splitDims.emplace_back(tensor->getDimensions()[idx]);
      }
    }
    if (destTensor->getDimensions().empty()) destTensor->setDimensions(splitDims);

    typename Tensor::Size curDim = 0;
    typename Tensor::Dimension fromIdx = 0;
    typename Tensor::Dimension toIdx = 0;
    destTensor->getValues().resize(destSize);
    copyValue<TensorType>(curDim, tensor, dimOffsets, tensor->getValues(), fromIdx, destTensor->getValues(), toIdx);
  }

  template <typename TensorType>
  void copyValue(typename Tensor::Size curDim,
                 const TensorType* fromTensor,
                 const typename Tensor::Dimensions& dimOffsets,
                 typename TensorType::ValueList& fromValues,
                 typename TensorType::Dimension fromIdx,
                 typename TensorType::ValueList& toValues,
                 typename TensorType::Dimension& toIdx) const {
    if (curDim == fromTensor->getNumOfDimensions()) {
      toValues[toIdx] = fromValues[fromIdx];
      ++toIdx;
      return;
    }
    typename Tensor::Dimension low = 0;
    typename Tensor::Dimension high = fromTensor->getDimensions()[curDim];
    if (curDim == m_Dim) {
      fromIdx += dimOffsets[curDim] * m_St;
      low = static_cast<typename Tensor::Dimension>(m_St);
      high = static_cast<typename Tensor::Dimension>(m_Ed);
    }
    for (typename Tensor::Dimension st = low; st < high; ++st) {
      copyValue(curDim+1, fromTensor, dimOffsets, fromValues, fromIdx, toValues, toIdx);
      fromIdx += dimOffsets[curDim];
    }
  }

private:
  ApplySplitOnChannel() = default;

private:
  Tensor* m_DestTensor;
  Tensor::Size m_Dim;
  IntsAttr::ValueType m_St;
  IntsAttr::ValueType m_Ed;
};

class MultiplyTensorByRatio {
public:
  using CalcType = double;

  explicit MultiplyTensorByRatio(const FloatAttr& val): m_Val(val.value()) {}

  // Multiply tensor by m_Val, using CalcType
  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    for (typename TensorType::Size idx = 0, n = tensor->getValues().size(); idx < n; ++idx) {
      tensor->getValues()[idx] =
        static_cast<typename TensorType::ValueType>(
          static_cast<CalcType>(tensor->getValues()[idx]) * m_Val);
    }
  }

private:
  CalcType m_Val;
};

enum BinaryOpTypeEnum
{
  OPTYPE_ADD,
  OPTYPE_SUB,
  OPTYPE_MUL,
  OPTYPE_DIV
};

class ApplyBinaryOpOnTensorWithSameShape {
public:
  ApplyBinaryOpOnTensorWithSameShape(Tensor* tensorToApplyOp, BinaryOpTypeEnum opType)
    : m_TensorToApplyOp(tensorToApplyOp)
    , m_OpType(opType)
  {}

  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    using CalcType              = typename TensorType::ValueType;
    TensorType* tensorToApplyOp = dynamic_cast<TensorType*>(m_TensorToApplyOp);
    assert(tensorToApplyOp != nullptr);
    assert(tensor->getDimensions() == tensorToApplyOp->getDimensions() &&
           "The shape of the tensor to apply operation must be the same");
    assert(tensor->getValues().size() == tensorToApplyOp->getValues().size());

    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      CalcType result;
      CalcType x = static_cast<CalcType>(tensor->getValues()[idx]);
      CalcType y = static_cast<CalcType>(tensorToApplyOp->getValues()[idx]);
      switch(m_OpType) {
        case OPTYPE_ADD: result = x + y; break;
        case OPTYPE_SUB: result = x - y; break;
        case OPTYPE_MUL: result = x * y; break;
        case OPTYPE_DIV: result = x / y; break;
      }
      tensor->getValues()[idx] = result;
    }
  }

private:
  ApplyBinaryOpOnTensorWithSameShape() = default;

private:
  Tensor* m_TensorToApplyOp;
  BinaryOpTypeEnum m_OpType;
};

enum UnaryOpTypeEnum
{
  OPTYPE_SQRT
};

class ApplyUnaryOpOnTensor
{
public:
  ApplyUnaryOpOnTensor(UnaryOpTypeEnum opType)
    : m_OpType(opType)
  {}

  template <typename TensorType>
  void operator()(TensorType* pTensor) const
  {
    assert(pTensor != nullptr);

    for (typename TensorType::Size idx = 0; idx < pTensor->getValues().size(); ++idx) {
      typename TensorType::ValueType result;
      switch (m_OpType) {
      case OPTYPE_SQRT:
        result = std::sqrt(pTensor->getValues()[idx]);
        break;
      default:
        assert("m_OpType does not match any supported type");
      }
      pTensor->getValues()[idx] = result;
    }
  }

private:
  ApplyUnaryOpOnTensor() = default;

private:
  Tensor*    m_TensorToApplyOp;
  UnaryOpTypeEnum m_OpType;
};

class MulTensorOnFirstDim {
public:
  using CalcType = double;
  MulTensorOnFirstDim(Tensor* tensorToMultiply): m_TensorToMultiply(tensorToMultiply) {
    assert(m_TensorToMultiply->getNumOfDimensions() == 1);
  }

  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    using Dimension = typename Tensor::Dimension;
    using DimsSize = typename Tensor::Size;
    using ValuesSize = typename TensorType::Size;

    TensorType* tensorToMultiply = dynamic_cast<TensorType*>(m_TensorToMultiply);
    assert(tensorToMultiply != nullptr);
    assert(tensor->getDimensions()[0] == tensorToMultiply->getDimensions()[0]);

    ValuesSize offset = 1;
    for (DimsSize idx = 1; idx < tensor->getNumOfDimensions(); ++idx) {
      offset *= static_cast<ValuesSize>(tensor->getDimensions()[idx]);
    }

    Dimension K = tensor->getDimensions()[0];
    for (ValuesSize offsetNumber = 0; offsetNumber < static_cast<ValuesSize>(K); ++offsetNumber) {
      for (ValuesSize curIdx = offsetNumber * offset; curIdx < (offsetNumber + 1) * offset; ++curIdx) {
        CalcType result = static_cast<CalcType>(tensor->getValues()[curIdx]) *
                          static_cast<CalcType>(tensorToMultiply->getValues()[offsetNumber]);
        tensor->getValues()[curIdx] = result;
      }
    }
  }

private:
  MulTensorOnFirstDim() = default;

private:
  Tensor* m_TensorToMultiply;
};

class ClearTensorValues {
public:
  ClearTensorValues() = default;

  template <typename TensorType>
  void operator() (TensorType* tensor) const {
    tensor->getValues().clear();
  }
};

class SetTensorValuesToOnes
{
public:
  SetTensorValuesToOnes() = default;

  template <typename TensorType>
  void operator()(TensorType* tensor) const
  {
    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      tensor->getValues()[idx] = 1;
    }
  }
};

class SetTensorValuesToZeros
{
public:
  SetTensorValuesToZeros() = default;

  template <typename TensorType>
  void operator()(TensorType* tensor) const
  {
    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      tensor->getValues()[idx] = 0;
    }
  }
};

class SetTensorValuesToConstZero
{
public:
  SetTensorValuesToConstZero() = default;

  template <typename TensorType>
  void operator()(TensorType* tensor) const
  {
    Tensor::Size totSize = 1;
    if (tensor->getValues().size() != totSize) {
      tensor->getValues().resize(totSize);
    }

    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      tensor->getValues()[idx] = 0;
    }
  }
};

class AddConstToTensor
{
public:
  AddConstToTensor(double valueToBeAdded)
    : m_ValueToBeAdded(valueToBeAdded)
  {}

  template <typename TensorType>
  void operator()(TensorType* tensor) const
  {
    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      tensor->getValues()[idx] += static_cast<typename TensorType::ValueType>(m_ValueToBeAdded);
    }
  }

private:
  AddConstToTensor() = default;

private:
  double m_ValueToBeAdded;
};

class ValidateTensorValuesInRange
{
public:
  ValidateTensorValuesInRange(double low, double high, double epsilon = 1e-5)
    : m_Low(low)
    , m_High(high)
    , m_Epsilon(epsilon)
  {}

  template <typename TensorType>
  void operator()(TensorType* tensor) const
  {
    for (typename TensorType::Size idx = 0; idx < tensor->getValues().size(); ++idx) {
      assert(tensor->getValues()[idx] <= m_High + m_Epsilon);
      assert(tensor->getValues()[idx] >= m_Low - m_Epsilon);
    }
  }

private:
  double m_Low;
  double m_High;
  double m_Epsilon;
};

inline bool isDefinedByInitializer(const Value* pValue) {
  return isa<Initializer>(static_cast<ComputeOperator*>(pValue->getDefine()));
}

template <typename NodeType>
inline bool isDefinedBy(const Value* pValue) {
  return isa<NodeType>(static_cast<ComputeOperator*>(pValue->getDefine()));
}

Tensor::Size getTotalSizeOfDimensions(const Tensor::Dimensions& dims);

} // namespace of internal
} // namespace of onnc

#endif // ONNC_OPTIMIZATIONS_UTILS
