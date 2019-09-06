//===- InterpreterPass.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaTaskSubmitPass.h"

#include "NvDlaMeta.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/String.h>
#include <onnc/Support/Timer.h>

using ::nvdla::ILoadable;

namespace onnc {

namespace internal {

ILoadable::Version& assign(ILoadable::Version& destVersion, const Version& srcVersion)
{
  destVersion.major     = srcVersion.major;
  destVersion.minor     = srcVersion.minor;
  destVersion.sub_minor = srcVersion.subMinor;

  return destVersion;
}

} // namespace internal

//===----------------------------------------------------------------------===//
// NvDlaTaskSubmitPass
//===----------------------------------------------------------------------===//
NvDlaTaskSubmitPass::NvDlaTaskSubmitPass(NvDlaBackendMeta* pMeta, Version pDlaVersion, Version pEmuVersion)
  : m_pMeta(pMeta)
  , m_DlaVersion(pDlaVersion)
  , m_EmuVersion(pEmuVersion)
{}

Pass::ReturnType NvDlaTaskSubmitPass::runOnModule(Module& pModule)
{
  using namespace internal;

  using OperationCategory = NvDlaBackendMeta::OperationMeta::Category;

  unsigned taskIndex = 0;
  for (std::size_t iTaskStart = 0; iTaskStart < m_pMeta->m_OperationMetas.size(); ++taskIndex) {
    const OperationCategory category = m_pMeta->m_OperationMetas[iTaskStart].category;

    // find last operation (in task) which has same category
    std::size_t iTaskEnd = iTaskStart + 1;
    for (; iTaskEnd < m_pMeta->m_OperationMetas.size(); ++iTaskEnd) {
      if (m_pMeta->m_OperationMetas[iTaskEnd].category != category) {
        break;
      }
    }
    assert(iTaskEnd <= m_pMeta->m_OperationMetas.size());

    const std::size_t numTasks = (iTaskEnd - iTaskStart);
    // submit for different type tasks
    if (category == OperationCategory::dla) {
      int dla_start;
      {
        std::string blob_name = to_string("task-", taskIndex, "-addr0");

        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = sizeof(struct dla_network_desc);
        b.interface    = ILoadable::Interface_DLA1;
        b.subInterface = 0;
        assign(b.version, m_DlaVersion);

        NvU8* blob_data = new NvU8[b.size];

        m_pMeta->m_DlaNetworkDesc.operation_desc_index   = m_pMeta->m_AddressListEntries.size() + 2;
        m_pMeta->m_DlaNetworkDesc.surface_desc_index     = m_pMeta->m_AddressListEntries.size() + 3;
        m_pMeta->m_DlaNetworkDesc.dependency_graph_index = m_pMeta->m_AddressListEntries.size() + 1;
        m_pMeta->m_DlaNetworkDesc.lut_data_index =
          (m_pMeta->m_LUTList.empty() ? -1 : m_pMeta->m_AddressListEntries.size() + 4);
        m_pMeta->m_DlaNetworkDesc.roi_array_index = -1;
        m_pMeta->m_DlaNetworkDesc.surface_index   = -1;
        m_pMeta->m_DlaNetworkDesc.stat_list_index = -1;
        m_pMeta->m_DlaNetworkDesc.stat_list_index = -1;

        m_pMeta->m_DlaNetworkDesc.num_rois       = 1;
        m_pMeta->m_DlaNetworkDesc.num_operations = numTasks;
        m_pMeta->m_DlaNetworkDesc.num_luts       = m_pMeta->m_NumLUTs;
        m_pMeta->m_DlaNetworkDesc.num_addresses  = m_pMeta->m_AddressListEntries.size() + 5;

        m_pMeta->m_DlaNetworkDesc.input_layer = 0;
        m_pMeta->m_DlaNetworkDesc.dynamic_roi = 0;

        memcpy(blob_data, &(m_pMeta->m_DlaNetworkDesc), sizeof(struct dla_network_desc));

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        dla_start = submitMemAllocAddress(b.size, blob_name);
      }

      {
        std::string     blob_name = to_string("task-", taskIndex, "-dep_graph");
        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = numTasks * sizeof(struct dla_common_op_desc);
        b.interface    = ILoadable::Interface_DLA1;
        b.subInterface = 0;
        assign(b.version, m_DlaVersion);

        NvU8*                      blob_data = new NvU8[b.size];
        struct dla_common_op_desc* op_blob   = (struct dla_common_op_desc*)blob_data;
        for (std::size_t i = iTaskStart; i < iTaskEnd; i++) {
          const auto& opMeta = m_pMeta->m_OperationMetas[i];

          NvDlaDlaOperation* op = m_pMeta->m_DLAOperationList[opMeta.index];
          memcpy(op_blob + (i - iTaskStart), &(op->op_dep), sizeof(struct dla_common_op_desc));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      {
        std::string     blob_name = to_string("task-", taskIndex, "-op_list");
        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = numTasks * sizeof(union dla_operation_container);
        b.interface    = ILoadable::Interface_DLA1;
        b.subInterface = 0;
        assign(b.version, m_DlaVersion);

        NvU8*                          blob_data = new NvU8[b.size];
        union dla_operation_container* op_blob   = (union dla_operation_container*)blob_data;
        for (std::size_t i = iTaskStart; i < iTaskEnd; i++) {
          const auto& opMeta = m_pMeta->m_OperationMetas[i];

          NvDlaDlaOperation* op = m_pMeta->m_DLAOperationList[opMeta.index];
          memcpy(op_blob + (i - iTaskStart), &(op->op_desc), sizeof(union dla_operation_container));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      {
        std::string     blob_name = to_string("task-", taskIndex, "-surf_list");
        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = numTasks * sizeof(union dla_surface_container);
        b.interface    = ILoadable::Interface_DLA1;
        b.subInterface = 0;
        assign(b.version, m_DlaVersion);

        NvU8*                        blob_data = new NvU8[b.size];
        union dla_surface_container* op_blob   = (union dla_surface_container*)blob_data;
        for (std::size_t i = iTaskStart; i < iTaskEnd; i++) {
          const auto& opMeta = m_pMeta->m_OperationMetas[i];

          NvDlaDlaOperation* op = m_pMeta->m_DLAOperationList[opMeta.index];
          memcpy(op_blob + (i - iTaskStart), &(op->op_surf), sizeof(union dla_surface_container));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      if (!m_pMeta->m_LUTList.empty()) {
        std::string     blob_name = to_string("task-", taskIndex, "-lut_list");
        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = m_pMeta->m_LUTList.size() * sizeof(struct dla_lut_param);
        b.interface    = ILoadable::Interface_DLA1;
        b.subInterface = 0;
        assign(b.version, m_DlaVersion);

        NvU8*                 blob_data = new NvU8[b.size];
        struct dla_lut_param* op_blob   = (struct dla_lut_param*)blob_data;
        for (int i = 0; i < m_pMeta->m_LUTList.size(); i++) {
          struct dla_lut_param* lut = m_pMeta->m_LUTList[i];
          memcpy(op_blob + i, lut, sizeof(struct dla_lut_param));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      {
        ILoadable::MemoryListEntry mle;
        mle.id             = m_pMeta->m_MemoryListEntries.size();
        mle.alignment      = 4096;
        mle.bind_id        = 0;
        mle.domain         = ILoadable::MemoryDomain_SYSMEM;
        mle.flags          = ILoadable::MemoryFlags_ALLOC;
        mle.size           = 4096;
        mle.tensor_desc_id = 0;
        m_pMeta->m_MemoryListEntries.push_back(mle);

        ILoadable::AddressListEntry ale;
        ale.size   = 0;
        ale.offset = 0;
        ale.mem_id = mle.id;
        ale.id     = m_pMeta->m_AddressListEntries.size();
        m_pMeta->m_AddressListEntries.push_back(ale);
      }

      {
        ILoadable::SubmitListEntry sle;
        ILoadable::TaskListEntry   tle;

        tle.id        = m_pMeta->m_TaskListEntries.size();
        tle.interface = ILoadable::Interface_DLA1;
        tle.instance  = -1;

        if (0 < taskIndex) {
          tle.preactions.push_back(submitEvent(tle.id, NVDLA_LOADABLE_EVENT_OP_WAIT));
          tle.postactions.push_back(submitEvent(tle.id, NVDLA_LOADABLE_EVENT_OP_SIGNAL));
        } else {
          tle.preactions.push_back(submitEvent(tle.id, NVDLA_LOADABLE_EVENT_OP_SIGNAL));
        }

        tle.address_list.push_back(dla_start);
        for (int i = 1; i < m_pMeta->m_AddressListEntries.size(); i++)
          tle.address_list.push_back(i);
        m_pMeta->m_TaskListEntries.push_back(tle);

        sle.id = m_pMeta->m_SubmitListEntries.size();
        sle.tasks.push_back(tle.id);
        m_pMeta->m_SubmitListEntries.push_back(sle);
      }
    } else if (category == OperationCategory::emu) {
      int emu_start;
      {
        std::string blob_name = to_string("task-", taskIndex, "-addr0");

        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = sizeof(struct emu_network_desc);
        b.interface    = ILoadable::Interface_EMU1;
        b.subInterface = 0;
        assign(b.version, m_EmuVersion);

        NvU8* blob_data = new NvU8[b.size];

        m_pMeta->m_EmuNetworkDesc.operation_desc_index        = m_pMeta->m_AddressListEntries.size() + 1;
        m_pMeta->m_EmuNetworkDesc.operation_buffer_desc_index = m_pMeta->m_AddressListEntries.size() + 2;
        m_pMeta->m_EmuNetworkDesc.num_operations              = numTasks;
        memcpy(blob_data, &(m_pMeta->m_EmuNetworkDesc), sizeof(struct emu_network_desc));

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        emu_start = submitMemAllocAddress(b.size, blob_name);
      }

      {
        std::string blob_name = to_string("task-", taskIndex, "-op_list");

        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = numTasks * sizeof(union emu_operation_container);
        b.interface    = ILoadable::Interface_EMU1;
        b.subInterface = 0;
        assign(b.version, m_EmuVersion);

        NvU8*                          blob_data = new NvU8[b.size];
        union emu_operation_container* op_blob   = (union emu_operation_container*)blob_data;
        for (std::size_t i = iTaskStart; i < iTaskEnd; i++) {
          const auto& opMeta = m_pMeta->m_OperationMetas[i];

          NvDlaEmuOperation* op = m_pMeta->m_EMUOperationList[opMeta.index];
          memcpy(op_blob + (i - iTaskStart), &(op->op_desc), sizeof(union emu_operation_container));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      {
        std::string blob_name = to_string("task-", taskIndex, "-op_buf_list");

        ILoadable::Blob b;
        b.name         = blob_name;
        b.size         = numTasks * sizeof(union emu_operation_buffer_container);
        b.interface    = ILoadable::Interface_EMU1;
        b.subInterface = 0;
        assign(b.version, m_EmuVersion);

        NvU8*                                 blob_data = new NvU8[b.size];
        union emu_operation_buffer_container* op_blob   = (union emu_operation_buffer_container*)blob_data;
        for (std::size_t i = iTaskStart; i < iTaskEnd; i++) {
          const auto& opMeta = m_pMeta->m_OperationMetas[i];

          NvDlaEmuOperation* op = m_pMeta->m_EMUOperationList[opMeta.index];
          memcpy(op_blob + (i - iTaskStart), &(op->op_buf), sizeof(union emu_operation_buffer_container));
        }

        m_pMeta->m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);
        submitMemAllocAddress(b.size, blob_name);
      }

      {
        for (int i = 0; i < 3; i++) {
          ILoadable::MemoryListEntry mle;
          mle.id             = m_pMeta->m_MemoryListEntries.size();
          mle.alignment      = 4096;
          mle.bind_id        = 0;
          mle.domain         = ILoadable::MemoryDomain_SYSMEM;
          mle.flags          = ILoadable::MemoryFlags_ALLOC;
          mle.size           = 4096;
          mle.tensor_desc_id = 0;
          m_pMeta->m_MemoryListEntries.push_back(mle);

          ILoadable::AddressListEntry ale;
          ale.size   = 0;
          ale.offset = 0;
          ale.mem_id = mle.id;
          ale.id     = m_pMeta->m_AddressListEntries.size();
          m_pMeta->m_AddressListEntries.push_back(ale);
        }

        int                        prepost_cnt = 0;
        ILoadable::SubmitListEntry sle;
        ILoadable::TaskListEntry   tle;

        tle.id        = m_pMeta->m_TaskListEntries.size();
        tle.interface = ILoadable::Interface_EMU1;
        tle.instance  = -1;

        tle.preactions.push_back(submitEvent(tle.id, NVDLA_LOADABLE_EVENT_OP_WAIT));
        tle.postactions.push_back(submitEvent(tle.id, NVDLA_LOADABLE_EVENT_OP_SIGNAL));
        tle.address_list.push_back(emu_start);
        for (int i = 1; i < m_pMeta->m_AddressListEntries.size(); i++)
          tle.address_list.push_back(i);
        m_pMeta->m_TaskListEntries.push_back(tle);

        sle.id = m_pMeta->m_SubmitListEntries.size();
        sle.tasks.push_back(tle.id);
        m_pMeta->m_SubmitListEntries.push_back(sle);
      }
    }

    iTaskStart = iTaskEnd;
  }

  return Pass::kModuleNoChanged;
}

int NvDlaTaskSubmitPass::submitEvent(int task_id, int event_type)
{
  ILoadable::EventListEntry ele;
  ele.id     = m_pMeta->m_EventListEntries.size();
  ele.op     = event_type;
  ele.target = 0;
  ele.val    = task_id + ele.op;

  m_pMeta->m_EventListEntries.push_back(ele);
  return ele.id;
}

int NvDlaTaskSubmitPass::submitMemAllocAddress(int size, std::string blob_name)
{
  int aid = m_pMeta->m_AddressListEntries.size();

  ILoadable::AddressListEntry ale;

  ILoadable::MemoryListEntry mle;
  mle.size           = size;
  mle.id             = m_pMeta->m_MemoryListEntries.size();
  mle.alignment      = 4096;
  mle.flags          = ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET;
  mle.domain         = ILoadable::MemoryDomain_SYSMEM;
  mle.bind_id        = 0;
  mle.tensor_desc_id = 0;
  mle.contents.push_back(blob_name);
  mle.offsets.push_back(0);
  m_pMeta->m_MemoryListEntries.push_back(mle);

  ale.size   = 0;
  ale.offset = 0;
  ale.mem_id = mle.id;
  ale.id     = aid;

  m_pMeta->m_AddressListEntries.push_back(ale);
  return aid;
}

} // namespace onnc
