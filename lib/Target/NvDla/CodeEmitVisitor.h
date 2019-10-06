//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_CODE_EMIT_VISITOR_H
#define TARGET_NVDLA_CODE_EMIT_VISITOR_H

#include "NvDlaDefine.h"
#include "NvDlaMeta.h"
#include "Compute/NvDlaShuffle.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/CustomVisitor.h>
#include <onnc/Support/Preprocessor.h>
#include <onnc/Support/Span.h>

#include <functional>

#ifndef PP_NVDLA_OP_LIST
#  define PP_NVDLA_OP_LIST                                                                              \
    (Add, AveragePool, Concat, Conv, Initializer, InputOperator, LRN, MaxPool, Mul, \
     OutputOperator, Relu, Reshape, Softmax, Split, Sum, Transpose)
#else
#  error "already defined PP_NVDLA_OP_LIST macro before"
#endif

#define PP_DECL_VISIT(type)         \
  void visit(const type&) override; \
  void visitImpl(const type&);

namespace onnc {
namespace nvdla {

class CodeEmitVisitor : public CustomVisitor<CodeEmitVisitor>, private NvDlaConstants
{
public:
  using CbufAllocTypeGetter =
    std::function<CbufAllocType(const NvDlaCubeInfo&, const NvDlaCubeInfo&, Tensor::Dimension, unsigned&)>;

public:
  CodeEmitVisitor(const NvDlaConstants& constants, NvDlaBackendMeta& meta, CbufAllocTypeGetter cbufAllocTypeGetter,
                  unsigned verboseLevel) noexcept
    : NvDlaConstants{constants}
    , m_pMeta{meta}
    , m_CbufAllocTypeGetter{std::move(cbufAllocTypeGetter)}
    , m_VerboseLevel{verboseLevel}
  {}

  PP_LIST_FOR_EACH(PP_DECL_VISIT, PP_UNWRAP(PP_NVDLA_OP_LIST))

  void visit(NvDlaShuffle& op) { visit(const_cast<const NvDlaShuffle&>(op)); }
  void visit(const NvDlaShuffle&);
  void visitImpl(const NvDlaShuffle&);

private:
  MemoryListEntryId packWeight(const Tensor& weight, NvDlaDims destDims, Tensor::Dimension numFrontPaddingChannels,
                               Tensor::Dimension outputChannelOffset);
  MemoryListEntryId packImageWeight(const Tensor& weight, NvDlaDims destDims, Tensor::Dimension outputChannelOffset);
  MemoryListEntryId packBias(const Tensor& bias, Tensor::Dimension numDestChannels,
                             Tensor::Dimension srcChannelOffset = 0);
  MemoryListEntryId packSDPOperand(const Tensor* aluTensor, const Tensor* mulTensor, const NvDlaCubeInfo& cubeInfo);

  MemoryListEntryId  packFeature(const Tensor& tensor, const NvDlaCubeInfo& cube);
  void               issueEmuOp(NvDlaEmuOperation* op);
  AddressListEntryId issueEmuAddr(MemoryListEntryId mid);
  void               issueDlaOp(NvDlaDlaOperation* op, NvDlaDlaOperation* op_fuse, NvDlaDlaOperation* op_prev);
  void               issueDlaOp(std::unique_ptr<NvDlaDlaOperation> op);
  AddressListEntryId issueDlaAddr(const Tensor& tensor, const NvDlaCubeInfo& cube, Tensor::Dimension channelOffset,
                                  NvDlaBackendMeta::Offset hOffset);
  AddressListEntryId issueDlaAddr(const Tensor& tensor, const NvDlaCubeInfo& cube);
  AddressListEntryId issueDlaAddr(MemoryListEntryId memoryId, const NvDlaCubeInfo& cube);
  AddressListEntryId issueSDPOperand(const Tensor& tensor, const NvDlaCubeInfo& cube, MemoryListEntryId& memoryId);

  void SetLUTParam(dla_lut_param* lut_param, float alpha, float beta, float bias, int size, float outdata_scale, float outdata_offset);

  // Perform SDP for 2 input tensors and an output tensor,
  // the possible value for parameter 'opType' is:
  //
  //   1. SDP_OP_ADD
  //   2. SDP_OP_MUL
  //
  void emitSdp(std::uint8_t opType, const Tensor& firstInput, const Tensor& secondInput, const Tensor& output);
  std::pair<unsigned, bool> tryAllocateDataAndWeightsIntoCBuf(const NvDlaCubeInfo& data, NvDlaCubeInfo& weight,
                                                              Tensor::Dimension yDilation) const;

private:
  MemoryListEntryId packWeight(span<const float> weight, const Tensor* weightTensor, NvDlaDims srcDims,
                               NvDlaDims destDims, Tensor::Dimension numFrontPaddingChannels,
                               Tensor::Dimension outputChannelOffset);

  MemoryListEntryId packWeight(const Tensor& weight, NvDlaDims srcDims, NvDlaDims destDims,
                               Tensor::Dimension numFrontPaddingChannels, Tensor::Dimension outputChannelOffset);

  template <typename Type>
  void packWeightImpl(Type* destData, NvDlaDims destDimsWithFrontPadding, const Tensor* tensor, const float* srcData,
                      NvDlaDims srcDims, Tensor::Dimension numFrontPaddingChannels,
                      Tensor::Dimension outputChannelOffset);

  template <typename Type>
  void packImageWeightImpl(Type* blob, NvDlaDims blobDims, const Tensor* tensor, const float* srcData,
                           NvDlaDims srcDims, Tensor::Dimension outputChannelOffset);
  template <typename Type>
  void packBiasImpl(Type* destData, Tensor::Dimension numDestChannels, const Tensor* tensor, const float* srcData,
                    Tensor::Dimension srcChannelOffset);
  void packSDPOperandImpl(NvU8* blob, const Tensor* aluTensor, const float* aluData, const Tensor* mulTensor,
                          const float* mulData, const NvDlaCubeInfo& cubeInfo);

private:
  NvDlaBackendMeta&         m_pMeta;
  const CbufAllocTypeGetter m_CbufAllocTypeGetter;
  const unsigned            m_VerboseLevel;
};

} // namespace nvdla
} // namespace onnc

#undef PP_DECL_VISIT
#undef PP_NVDLA_OP_LIST

#endif
