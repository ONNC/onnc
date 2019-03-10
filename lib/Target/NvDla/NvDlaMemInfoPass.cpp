//===- InterpreterPass.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaMemInfoPass.h"

#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// NvDlaMemInfoPass
//===----------------------------------------------------------------------===//
NvDlaMemInfoPass::NvDlaMemInfoPass(NvDlaBackendMeta* pMeta)
  : m_pMeta{pMeta}
{}

Pass::ReturnType NvDlaMemInfoPass::runOnModule(Module& pModule)
{
  // [0] entry of memory & address list
  {
    ILoadable::MemoryListEntry mle;
    mle.id             = m_pMeta->m_MemoryListEntries.size();
    mle.alignment      = 4096;
    mle.bind_id        = 0;
    mle.domain         = nvdla::ILoadable::MemoryDomain_SYSMEM;
    mle.flags          = nvdla::ILoadable::MemoryFlags_ALLOC;
    mle.size           = 4096;
    mle.tensor_desc_id = 0;
    m_pMeta->m_MemoryListEntries.push_back(mle);

    ILoadable::AddressListEntry ale;
    ale.size   = 4096;
    ale.offset = 0;
    ale.mem_id = mle.id;
    ale.id     = m_pMeta->m_AddressListEntries.size();
    m_pMeta->m_AddressListEntries.push_back(ale);
  }

  std::unordered_map<Value*, bool> isOutputMap;
  for (ComputeOperator& cm : *pModule.getRootComputeGraph()) {
    if (OutputOperator* out = dyn_cast<OutputOperator>(&cm)) {
      for (int i = 0; i < out->getNumOfInputs(); ++i) {
        Value* v       = out->getInput(i);
        isOutputMap[v] = true;
        Tensor* t      = static_cast<Tensor*>(v);
        for (auto j : t->getDimensions()) {
          NVDLA_DBG("output dim[%ld]\n", j);
        }
      }
    } else if (Reshape* reshape = dyn_cast<Reshape>(&cm)) {
      Tensor* input_t                   = reshape->getInput(0);
      Tensor* output_t                  = reshape->getOutput(0);
      m_pMeta->m_ReshapeTable[output_t] = input_t;
    } else if (Concat* concat = dyn_cast<Concat>(&cm)) {
      int32_t num_inputs  = concat->getNumOfInputs();
      int32_t num_outputs = concat->getNumOfOutputs();
      Tensor* output_t    = concat->getOutput(0);

      int channels = 0;
      for (int i = 0; i < num_inputs; i++) {
        Tensor*     input_t = concat->getInput(i);
        concat_meta meta;
        meta.t                          = output_t;
        meta.ofs                        = channels;
        m_pMeta->m_ConcatTable[input_t] = meta;
        NVDLA_DBG("\tconcat input[%d] dim(%d %d %d %d)\n", i, input_t->dimension(0), input_t->dimension(1),
                  input_t->dimension(2), input_t->dimension(3));
        channels += input_t->dimension(1);
      }
    }
  }

  for (ComputeOperand* co : pModule.getComputeOperands()) {
    NVDLA_DBG("ComputeOperand: ");
    if (ComputeMemOperand* mem = dyn_cast<ComputeMemOperand>(co)) {
      Value* v = co->getValue();
      if (mem->isWeight()) {
        // for weight, memory buffers are allocated & blob files are also generated in ComputeOperator.

        FloatTensor* t = static_cast<FloatTensor*>(v);
        NVDLA_DBG("weight size:%d %d\n", mem->length(), t->getValues().size());

      } else {
        NVDLA_DBG("operand size:%d\n", mem->length());
        // alocation only, no blobs
        Tensor* t = static_cast<Tensor*>(v);

        // skip reshape output memory allocation
        if (m_pMeta->m_ReshapeTable.find(t) != m_pMeta->m_ReshapeTable.end() ||
            m_pMeta->m_ConcatTable.find(t) != m_pMeta->m_ConcatTable.end()) {
          continue;
        }

        ILoadable::MemoryListEntry     mle;
        ILoadable::TensorDescListEntry tle;
        mle.id = m_pMeta->m_MemoryListEntries.size();

        int dims[4] = {1, 1, 1, 1};
        int idx     = 0;
        for (auto i : t->getDimensions())
          dims[idx++] = i;
        NvDlaCubeInfo cubeinfo(NVDLA_CUBE_FEATURE, dims[0], dims[1], dims[2], dims[3], sizeof(unsigned short));
        mle.size = cubeinfo.size;

        mle.alignment      = 4096;
        mle.flags          = nvdla::ILoadable::MemoryFlags_ALLOC;
        mle.domain         = nvdla::ILoadable::MemoryDomain_SYSMEM;
        mle.bind_id        = 0;
        mle.tensor_desc_id = 0;

        if (mem->isInput()) {
          mle.flags |= nvdla::ILoadable::MemoryFlags_INPUT;
          mle.tensor_desc_id = 0;

          tle.name   = "data";
          tle.id     = 0;
          tle.memId  = mle.id;
          tle.size   = mle.size;
          tle.offset = 0;

          tle.dims.n       = cubeinfo.dim_n;
          tle.dims.c       = cubeinfo.dim_c;
          tle.dims.h       = cubeinfo.dim_h;
          tle.dims.w       = cubeinfo.dim_w;
          tle.dataFormat   = 3;
          tle.dataType     = nvdla::loadable::DataType_HALF;
          tle.dataCategory = nvdla::loadable::DataCategory_FEATURE;
          tle.pixelFormat  = TENSOR_PIXEL_FORMAT_FEATURE;
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
        } else if (isOutputMap.find(v) != isOutputMap.end()) {
          mle.flags |= nvdla::ILoadable::MemoryFlags_OUTPUT;
          mle.tensor_desc_id = 1;

          tle.name   = "probe";
          tle.id     = 1;
          tle.memId  = mle.id;
          tle.size   = mle.size;
          tle.offset = 0;

          tle.dims.n       = cubeinfo.dim_n;
          tle.dims.c       = cubeinfo.dim_c;
          tle.dims.h       = cubeinfo.dim_h;
          tle.dims.w       = cubeinfo.dim_w;
          tle.dataFormat   = 3;
          tle.dataType     = nvdla::loadable::DataType_HALF;
          tle.dataCategory = nvdla::loadable::DataCategory_FEATURE;
          tle.pixelFormat  = TENSOR_PIXEL_FORMAT_FEATURE;
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
        }
        m_pMeta->m_MemIdxTable[v] = m_pMeta->m_MemoryListEntries.size();
        m_pMeta->m_MemoryListEntries.push_back(mle);
      }
    }
  }
  return Pass::kModuleNoChanged;
}
