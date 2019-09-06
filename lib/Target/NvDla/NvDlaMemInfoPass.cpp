//===- NvDlaMemInfoPass.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaMemInfoPass.h"

#include "NvDlaUtil.h"

#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>

#include <cassert>
#include <unordered_set>

using namespace ::nvdla::loadable;

//===----------------------------------------------------------------------===//
// NvDlaMemInfoPass
//===----------------------------------------------------------------------===//
namespace onnc {

NvDlaMemInfoPass::NvDlaMemInfoPass(const NvDlaConstants& constants, NvDlaBackendMeta* pMeta) noexcept
  : NvDlaConstants{constants}
  , m_pMeta{pMeta}
{}

Pass::ReturnType NvDlaMemInfoPass::runOnModule(Module& pModule)
{
  using namespace nvdla;
  // [0] entry of memory & address list
  {
    const MemoryListEntryId memoryId =
      m_pMeta->allocateMemory(ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC, 4096);
    m_pMeta->acquireMemory(memoryId, 0, 4096);
  }

  using std::begin;
  using std::end;

  std::unordered_set<const Tensor*> outputTensors;
  std::vector<const Tensor*>        tensors;
  for (ComputeOperator& cm : *pModule.getRootComputeGraph()) {
    if (OutputOperator* outputOperator = dyn_cast<OutputOperator>(&cm)) {
      for (unsigned idx = 0; idx < outputOperator->getNumOfInputs(); ++idx) {
        outputTensors.insert(static_cast<const Tensor*>(outputOperator->getInput(idx)));
      }
    }

    for (unsigned idx = 0; idx < cm.getNumOfOutputs(); ++idx) {
      const Tensor* output = static_cast<const Tensor*>(cm.getOutput(idx));
      if (std::find(begin(tensors), end(tensors), output) == end(tensors)) {
        tensors.emplace_back(output);
      }
    }
  }
  assert(!outputTensors.empty());

  using std::end;
  const auto isOutput = [&outputTensors](const Tensor* tensor) {
    return outputTensors.find(tensor) != end(outputTensors);
  };

  for (const Tensor* tensor : tensors) {
    if (isConstant(*tensor)) {
      continue;
    }

    // skip allocating memory for Reshape input tensors
    if (!(m_pMeta->shouldOwnMemory(*tensor) || isOutput(tensor))) {
      continue;
    }

    // skip for already-allocated-memory tensors
    if (m_pMeta->hasMemoryListEntry(*tensor)) {
      continue;
    }

    int dims[4] = {1, 1, 1, 1};
    int idx     = 0;
    for (auto i : tensor->getDimensions())
      dims[idx++] = i;

    const NvDlaCubeInfo cubeinfo(*this, NVDLA_CUBE_FEATURE, dims[0], dims[1], dims[2], dims[3], 0, 0);
   
    const bool isInput = isa<InputOperator>(getProducer(*tensor));
    if (isInput && !m_pMeta->hasMemoryListEntry(*tensor)) {
      const MemoryListEntryId memoryId =
        m_pMeta->allocateMemoryFor(*tensor, ILoadable::MemoryDomain_SYSMEM,
                                   ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_INPUT, cubeinfo.size);

      ILoadable::TensorDescListEntry tle;
      tle.name   = "data";
      tle.id     = 0;
      tle.memId  = memoryId;
      tle.size   = m_pMeta->getMemoryListEntrySize(memoryId);
      tle.offset = 0;

      tle.dims.n       = cubeinfo.dim_n;
      tle.dims.c       = cubeinfo.dim_c;
      tle.dims.h       = cubeinfo.dim_h;
      tle.dims.w       = cubeinfo.dim_w;
      tle.dataFormat   = 3;
      tle.dataType     = DATA_TYPE;
      tle.dataCategory = DataCategory_FEATURE;
      tle.pixelFormat  = INPUT_PIXEL_FORMAT;
      tle.pixelMapping = 0;

      tle.stride[0] = cubeinfo.stride_channel;
      tle.stride[1] = cubeinfo.stride_line;
      tle.stride[2] = cubeinfo.stride_surface;
      tle.stride[3] = 0;
      tle.stride[4] = 0;
      tle.stride[5] = 0;
      tle.stride[6] = 0;
      tle.stride[7] = 0;

      m_pMeta->m_TensorDescListEntries.emplace(m_pMeta->m_TensorDescListEntries.begin(), tle);
    } else if (isOutput(tensor)) {
      const NvDlaBackendMeta::MemoryFlags flags    = ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_OUTPUT;
      MemoryListEntryId                   memoryId = NvDlaBackendMeta::getInvalidMemoryListEntryId();
      if (m_pMeta->shouldOwnMemory(*tensor)) {
        memoryId = m_pMeta->allocateMemoryFor(*tensor, ILoadable::MemoryDomain_SYSMEM, flags, cubeinfo.size,
                                              true /* is output */);
      } else {
        NvDlaBackendMeta::MemoryListEntry& memory = m_pMeta->getMemoryListEntry(*tensor);

        memory.tensor_desc_id = 1; // mark this MemoryListEntry is for output
        memory.flags          = flags;
        memoryId              = memory.id;
      }
      assert(memoryId != NvDlaBackendMeta::getInvalidMemoryListEntryId());

      ILoadable::TensorDescListEntry tle;
      tle.name   = "probe";
      tle.id     = 1;
      tle.memId  = memoryId;
      tle.size   = m_pMeta->getMemoryListEntrySize(memoryId);
      tle.offset = 0;

      tle.dims.n       = cubeinfo.dim_n;
      tle.dims.c       = cubeinfo.dim_c;
      tle.dims.h       = cubeinfo.dim_h;
      tle.dims.w       = cubeinfo.dim_w;
      tle.dataFormat   = 3;
      tle.dataType     = DATA_TYPE;
      tle.dataCategory = DataCategory_FEATURE;
      tle.pixelFormat  = OUTPUT_PIXEL_FORMAT;
      tle.pixelMapping = 0;

      tle.stride[0] = cubeinfo.stride_channel;
      tle.stride[1] = cubeinfo.stride_line;
      tle.stride[2] = cubeinfo.stride_surface;
      tle.stride[3] = 0;
      tle.stride[4] = 0;
      tle.stride[5] = 0;
      tle.stride[6] = 0;
      tle.stride[7] = 0;

      m_pMeta->m_TensorDescListEntries.push_back(tle);
    } else {
      m_pMeta->tryAllocateMemoryFor(*tensor, ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC,
                                    cubeinfo.size);
    }
  }

  return Pass::kModuleNoChanged;
}
} // namespace onnc
