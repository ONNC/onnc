/*
 * Copyright (c) 2017-2018, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "priv/Loadable.h"

#include "ErrorMacros.h"
#include "priv/loadable_generated.h"

namespace nvdla {
ILoadable::ILoadable() {}
ILoadable::~ILoadable() {}

namespace priv {
LoadableFactory::LoadablePrivPair LoadableFactory::newLoadable()
{
  ILoadable* loadable;
  Loadable*  loadable_priv;
  loadable = loadable_priv = new priv::Loadable();
  if (loadable) {
    s_priv.insert(loadable, loadable_priv);
    s_self.insert(loadable, loadable);
  }
  return LoadablePrivPair(loadable, loadable_priv);
}

void LoadableFactory::deleteLoadable(ILoadable* loadable)
{
  if (loadable) {
    Loadable* loadable_priv = priv(loadable);
    if (loadable_priv) {
      delete loadable_priv;
    }

    s_priv.remove(loadable);
    s_self.remove(loadable);
  }
}

Loadable* LoadableFactory::priv(ILoadable* loadable)
{
  BiMap<ILoadable*, Loadable*>::left_iterator f = s_priv.find_left(loadable);

  if (f == s_priv.end_left()) {
    return NULL;
  }
  return f->second;
}

ILoadable* LoadableFactory::i(Loadable* loadable)
{
  BiMap<ILoadable*, Loadable*>::right_iterator f = s_priv.find_right(loadable);
  if (f == s_priv.end_right()) {
    return NULL;
  }
  return f->second;
}

ILoadable* LoadableFactory::self(void* s)
{
  BiMap<void*, ILoadable*>::left_iterator f = s_self.find_left(s);
  if (f == s_self.end_left()) {
    return NULL;
  }
  return f->second;
}

BiMap<ILoadable*, Loadable*> LoadableFactory::s_priv;
BiMap<void*, ILoadable*>     LoadableFactory::s_self;

// there's only one type of loadable for now. so only one of these... so it
// looks silly.  see the same paths in "LayerFactory::deserialize*" for why it
// makes sense to organize this way preemptively.
ILoadable* LoadableFactory::deserializeLoadable(NvU8* buf)
{
  LoadableFactory::LoadablePrivPair n = LoadableFactory::newLoadable();
  if (!n) {
    // gLogError << __func__ << " error allocating new loadable" << endl;
    return NULL;
  }
  n.priv()->deserializeFrom(buf);
  return n.i();
}

Loadable::Loadable() {}

NvU16 Loadable::getFactoryType() const
{
  return 0; // only one type of loadable so far, not complicated by factory
            // splits
}

std::string Loadable::getName() const { return mName; }

int Loadable::getNumMemoryListEntries() const { return (int)mMemoryListEntries.size(); }

ILoadable::MemoryListEntry Loadable::getMemoryListEntry(NvU16 mem_id) const { return mMemoryListEntries[mem_id]; }

const std::vector<ILoadable::MemoryListEntry>& Loadable::getMemoryListEntries() const { return mMemoryListEntries; }

int Loadable::getNumEventListEntries() const { return (int)mEventListEntries.size(); }

ILoadable::EventListEntry Loadable::getEventListEntry(NvU16 event_id) const { return mEventListEntries[event_id]; }

const std::vector<ILoadable::EventListEntry>& Loadable::getEventListEntries() const { return mEventListEntries; }

int Loadable::getNumTaskListEntries() const { return mTaskListEntries.size(); }

ILoadable::TaskListEntry Loadable::getTaskListEntry(NvU16 task_id) const { return mTaskListEntries[task_id]; }
const std::vector<ILoadable::TaskListEntry>& Loadable::getTaskListEntries() const { return mTaskListEntries; }

int Loadable::getNumSubmitListEntries() const { return mSubmitListEntries.size(); }

ILoadable::SubmitListEntry Loadable::getSubmitListEntry(NvU16 submit_id) const { return mSubmitListEntries[submit_id]; }

const std::vector<ILoadable::SubmitListEntry>& Loadable::getSubmitListEntries() const { return mSubmitListEntries; }

int Loadable::getNumAddressListEntries() const { return mAddressListEntries.size(); }

ILoadable::AddressListEntry Loadable::getAddressListEntry(NvU16 address_list_index) const
{
  return mAddressListEntries[address_list_index];
}

const std::vector<ILoadable::AddressListEntry>& Loadable::getAddressListEntries() const { return mAddressListEntries; }

int Loadable::getNumTensorDescListEntries() const { return mTensorDescListEntries.size(); }

ILoadable::TensorDescListEntry Loadable::getTensorDescListEntry(NvU16 tensor_desc_list_index) const
{
  return mTensorDescListEntries[tensor_desc_list_index];
}

const std::vector<ILoadable::TensorDescListEntry>& Loadable::getTensorDescListEntries() const
{
  return mTensorDescListEntries;
}

int Loadable::getNumRelocEntries() const { return mRelocEntries.size(); }

ILoadable::RelocEntry Loadable::getRelocEntry(NvU16 i) const { return mRelocEntries[i]; }

const std::vector<ILoadable::RelocEntry>& Loadable::getRelocEntries() const { return mRelocEntries; }
//
// internally facing
//
void Loadable::setMemoryListEntries(const std::vector<ILoadable::MemoryListEntry>& m) { mMemoryListEntries = m; }

void Loadable::setEventListEntries(const std::vector<ILoadable::EventListEntry>& m) { mEventListEntries = m; }

void Loadable::setTaskListEntries(const std::vector<ILoadable::TaskListEntry>& m) { mTaskListEntries = m; }

void Loadable::setSubmitListEntries(const std::vector<ILoadable::SubmitListEntry>& m) { mSubmitListEntries = m; }

void Loadable::setAddressListEntries(const std::vector<ILoadable::AddressListEntry>& e) { mAddressListEntries = e; }

void Loadable::setTensorDescListEntries(const std::vector<ILoadable::TensorDescListEntry>& e)
{
  mTensorDescListEntries = e;
}

void Loadable::setRelocEntries(const std::vector<ILoadable::RelocEntry>& e) { mRelocEntries = e; }

int Loadable::setSymbolContent(std::string name, const ILoadable::Blob& b, NvU8* data)
{
  mSymbols[name].name         = b.name;
  mSymbols[name].interface    = b.interface;
  mSymbols[name].subInterface = b.subInterface;
  mSymbols[name].version      = b.version;
  mSymbols[name].size         = b.size;
  mSymbols[name].data         = data;

  return 0;
}

bool Loadable::getSymbolContent(std::string name, ILoadable::Blob& blob, NvU8*& data)
{
  std::map<std::string, Symbol>::iterator f = mSymbols.find(name);

  if (f == mSymbols.end()) {
    if (debugSymbolContent()) {
      // gLogInfo <<  "missing symbol content for name=" << name << endl;
    }
    return false;
  }

  blob.name         = f->second.name;
  blob.size         = f->second.size;
  blob.version      = f->second.version;
  blob.interface    = f->second.interface;
  blob.subInterface = f->second.subInterface;
  data              = f->second.data;
  return true;
}

NvDlaError Loadable::getNetworkDataType(DataType::UnderlyingType* d) const
{
  NvDlaError e = NvDlaSuccess;
  if (!d) {
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }

  *d = DataType::UnderlyingType(DataType::HALF);

fail:
  return e;
}

NvDlaError Loadable::getNumInputTensors(int* inputs) const
{
  NvDlaError e = NvDlaSuccess;
  if (!inputs) {
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }

  *inputs = 0;
  for (size_t mi = 0, MI = mMemoryListEntries.size(); mi != MI; ++mi) {
    if (mMemoryListEntries[mi].flags & ILoadable::MemoryListEntry::flags_input()) {
      (*inputs)++;
    }
  }
fail:
  return e;
}

NvDlaError Loadable::getNumOutputTensors(int* outputs) const
{
  NvDlaError e = NvDlaSuccess;
  if (!outputs) {
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }

  *outputs = 0;
  for (size_t mi = 0, MI = mMemoryListEntries.size(); mi != MI; ++mi) {
    if (mMemoryListEntries[mi].flags & ILoadable::MemoryListEntry::flags_output()) {
      (*outputs)++;
    }
  }
fail:
  return e;
}

class MemoryListEntry_Input_BindId_Is
{
public:
  MemoryListEntry_Input_BindId_Is(NvU16 id)
    : m_find_id(id)
  {}
  bool operator()(const ILoadable::MemoryListEntry& mle)
  {
    return (mle.flags & mle.flags_input()) && (mle.bind_id == m_find_id);
  }

protected:
  NvU16 m_find_id;
};

class MemoryListEntry_Output_BindId_Is
{
public:
  MemoryListEntry_Output_BindId_Is(NvU16 id)
    : m_find_id(id)
  {}
  bool operator()(const ILoadable::MemoryListEntry& mle)
  {
    return (mle.flags & mle.flags_output()) && (mle.bind_id == m_find_id);
  }

protected:
  NvU16 m_find_id;
};

NvDlaError Loadable::getInputTensorDesc(NvU16 id, TensorDescListEntry* t) const
{
  NvDlaError                                              e = NvDlaSuccess;
  std::vector<ILoadable::MemoryListEntry>::const_iterator f_mem;
  MemoryListEntry_Input_BindId_Is                         find_mle_with_input_id(id);

  if (!t) {
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }

  f_mem = std::find_if(mMemoryListEntries.begin(), mMemoryListEntries.end(), find_mle_with_input_id);

  if (f_mem == mMemoryListEntries.end()) {
    // doesn't exist
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }
  if (f_mem->tensor_desc_id >= mTensorDescListEntries.size()) {
    // exists but is bogus
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }
  *t = mTensorDescListEntries[f_mem->tensor_desc_id];

fail:
  return e;
}

NvDlaError Loadable::getOutputTensorDesc(NvU16 id, TensorDescListEntry* t) const
{
  NvDlaError                                              e = NvDlaSuccess;
  std::vector<ILoadable::MemoryListEntry>::const_iterator f_mem;
  MemoryListEntry_Output_BindId_Is                        find_mle_with_output_id(id);

  if (!t) {
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }

  f_mem = std::find_if(mMemoryListEntries.begin(), mMemoryListEntries.end(), find_mle_with_output_id);

  if (f_mem == mMemoryListEntries.end()) {
    // doesn't exist
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }
  if (f_mem->tensor_desc_id >= mTensorDescListEntries.size()) {
    // exists but is bogus
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);
  }
  *t = mTensorDescListEntries[f_mem->tensor_desc_id];

fail:
  return e;
}

Loadable::~Loadable()
{
  // clear the contents of loadable before delete
  std::map<std::string, Symbol>::iterator it;
  for (it = mSymbols.begin(); it != mSymbols.end(); it++) {
    Symbol symbol = it->second;
    if (symbol.data != NULL)
      delete[] symbol.data;
  }
  mMemoryListEntries.clear();
  mTaskListEntries.clear();
  mSubmitListEntries.clear();
  mEventListEntries.clear();
  mAddressListEntries.clear();
  mTensorDescListEntries.clear();
  mRelocEntries.clear();
  mFbb.Clear();
}

bool Loadable::serialize()
{
  std::vector<flatbuffers::Offset<nvdla::loadable::SubmitListEntry>>     submit_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::TaskListEntry>>       task_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::MemoryListEntry>>     memory_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::AddressListEntry>>    address_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::EventListEntry>>      event_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::Blob>>                blobs;
  std::vector<flatbuffers::Offset<nvdla::loadable::TensorDescListEntry>> tensor_desc_list;
  std::vector<flatbuffers::Offset<nvdla::loadable::RelocListEntry>>      reloc_list;

  nvdla::loadable::Version loadable_version(nvdla::loadable::LoadableVersionMajor_VAL,
                                            nvdla::loadable::LoadableVersionMinor_VAL,
                                            nvdla::loadable::LoadableVersionSubMinor_VAL);

  for (size_t ti = 0, TI = mTaskListEntries.size(); ti != TI; ++ti) {
    const ILoadable::TaskListEntry& tle            = mTaskListEntries[ti];
    auto                            addr_list_v    = mFbb.CreateVector<uint16_t>(tle.address_list);
    auto                            pre_actions_v  = mFbb.CreateVector<uint16_t>(tle.preactions);
    auto                            post_actions_v = mFbb.CreateVector<uint16_t>(tle.postactions);

    nvdla::loadable::TaskListEntryBuilder tleb(mFbb);
    tleb.add_address_list(addr_list_v);
    tleb.add_pre_actions(pre_actions_v);
    tleb.add_post_actions(post_actions_v);
    tleb.add_id(tle.id);
    nvdla::loadable::Interface if_id;
    if (tle.interface == nvdla::ILoadable::TaskListEntry::interface_DLA1()) {
      if_id = nvdla::loadable::Interface_DLA1;
    } else if (tle.interface == nvdla::ILoadable::TaskListEntry::interface_EMU1()) {
      if_id = nvdla::loadable::Interface_EMU1;
    } else {
      if_id = nvdla::loadable::Interface_NONE;
    }
    tleb.add_interface(if_id);
    tleb.add_instance(tle.instance);
    task_list.push_back(tleb.Finish());
  }

  for (size_t si = 0, SI = mSubmitListEntries.size(); si != SI; ++si) {
    const ILoadable::SubmitListEntry&       sle     = mSubmitListEntries[si];
    auto                                    tasks_v = mFbb.CreateVector<uint16_t>(sle.tasks);
    nvdla::loadable::SubmitListEntryBuilder sleb(mFbb);
    sleb.add_id(sle.id);
    sleb.add_task_id(tasks_v);
    submit_list.push_back(sleb.Finish());
  }

  for (size_t mi = 0, MI = mMemoryListEntries.size(); mi != MI; ++mi) {
    const ILoadable::MemoryListEntry&       mle        = mMemoryListEntries[mi];
    auto                                    contents_v = mFbb.CreateVectorOfStrings(mle.contents);
    auto                                    offsets_v  = mFbb.CreateVector<uint64_t>(mle.offsets);
    nvdla::loadable::MemoryListEntryBuilder mleb(mFbb);
    mleb.add_contents(contents_v);
    mleb.add_offsets(offsets_v);
    mleb.add_size(mle.size);
    mleb.add_alignment(mle.alignment);
    mleb.add_bind_id(mle.bind_id);
    mleb.add_tensor_desc_id(mle.tensor_desc_id);
    mleb.add_flags((nvdla::loadable::MemoryFlags)mle.flags);
    mleb.add_id(mle.id);
    mleb.add_domain((nvdla::loadable::MemoryDomain)mle.domain);
    memory_list.push_back(mleb.Finish());
  }

  for (size_t ai = 0, AI = mAddressListEntries.size(); ai != AI; ++ai) {
    const ILoadable::AddressListEntry&       ale = mAddressListEntries[ai];
    nvdla::loadable::AddressListEntryBuilder aleb(mFbb);
    aleb.add_size(ale.size);
    aleb.add_offset(ale.offset);
    aleb.add_mem_id(ale.mem_id);
    aleb.add_id(ale.id);
    address_list.push_back(aleb.Finish());
  }

  for (size_t ei = 0, EI = mEventListEntries.size(); ei != EI; ++ei) {
    const ILoadable::EventListEntry&       ele = mEventListEntries[ei];
    nvdla::loadable::EventListEntryBuilder eleb(mFbb);
    eleb.add_id(ele.id);
    eleb.add_op((nvdla::loadable::EventOp)ele.op);
    eleb.add_target(ele.target);
    eleb.add_val(ele.val);
    event_list.push_back(eleb.Finish());
  }

  for (std::map<std::string, Symbol>::const_iterator si = mSymbols.begin(); si != mSymbols.end(); ++si) {
    const Symbol&                sym    = si->second;
    auto                         data_v = mFbb.CreateVector<uint8_t>(sym.data, sym.size);
    auto                         name_s = mFbb.CreateString(sym.name.c_str());
    nvdla::loadable::BlobBuilder bb(mFbb);
    bb.add_data(data_v);
    bb.add_size(sym.size);
    nvdla::loadable::Version v(sym.version.major, sym.version.minor, sym.version.sub_minor);
    bb.add_version(&v);
    bb.add_interface((nvdla::loadable::Interface)sym.interface);
    bb.add_name(name_s);
    blobs.push_back(bb.Finish());
  }

  for (size_t tdi = 0, TDI = mTensorDescListEntries.size(); tdi != TDI; ++tdi) {
    const ILoadable::TensorDescListEntry&       ele    = mTensorDescListEntries[tdi];
    auto                                        name_s = mFbb.CreateString(ele.name.c_str());
    nvdla::loadable::TensorDescListEntryBuilder tdleb(mFbb);

    tdleb.add_name(name_s);
    tdleb.add_id(ele.id);
    tdleb.add_mem_id(ele.memId);
    tdleb.add_size(ele.size);
    tdleb.add_offset(ele.offset);

    tdleb.add_data_format(nvdla::loadable::DataFormat(ele.dataFormat));
    tdleb.add_data_type(nvdla::loadable::DataType(ele.dataType));
    tdleb.add_data_category(nvdla::loadable::DataCategory(ele.dataCategory));
    tdleb.add_pixel_format(nvdla::loadable::PixelFormat(ele.pixelFormat));
    tdleb.add_pixel_mapping(nvdla::loadable::PixelMapping(ele.pixelMapping));

    tdleb.add_n(ele.dims.n);
    tdleb.add_c(ele.dims.c);
    tdleb.add_h(ele.dims.h);
    tdleb.add_w(ele.dims.w);

    tdleb.add_stride_0(ele.stride[0]);
    tdleb.add_stride_1(ele.stride[1]);
    tdleb.add_stride_2(ele.stride[2]);
    tdleb.add_stride_3(ele.stride[3]);
    tdleb.add_stride_4(ele.stride[4]);
    tdleb.add_stride_5(ele.stride[5]);
    tdleb.add_stride_6(ele.stride[6]);
    tdleb.add_stride_7(ele.stride[7]);

    tensor_desc_list.push_back(tdleb.Finish());
  }

  for (size_t rli = 0, RLI = mRelocEntries.size(); rli != RLI; ++rli) {
    const ILoadable::RelocEntry&           ele = mRelocEntries[rli];
    nvdla::loadable::RelocListEntryBuilder rleb(mFbb);

    rleb.add_address_id(ele.addressListId);
    rleb.add_write_id(ele.writeId);
    rleb.add_offset(ele.offset);
    rleb.add_interface(ele.interface);
    rleb.add_sub_interface(ele.subInterface);
    rleb.add_reloc_type(ele.relocType);

    reloc_list.push_back(rleb.Finish());
  }

  flatbuffers::Offset<nvdla::loadable::Loadable> l =
    CreateLoadableDirect(mFbb, &loadable_version, &task_list, &memory_list, &address_list, &event_list, &blobs,
                         &tensor_desc_list, &reloc_list, &submit_list);

  mFbb.Finish(l, "NVDA");
  {
    FILE* fptr = fopen("out.nvdla", "wb");
    fwrite(mFbb.GetBufferPointer(), mFbb.GetSize(), 1, fptr);
    fclose(fptr);
  }

  return true;
}

NvDlaError Loadable::getSerializedData(NvU8* buffer)
{
  NvDlaError e    = NvDlaSuccess;
  NvU8*      tmp  = NULL;
  NvU64      size = 0;

  if (buffer == NULL)
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);

  size = mFbb.GetSize();
  tmp  = mFbb.GetBufferPointer();
  memcpy(buffer, tmp, size);

fail:
  return e;
}

NvDlaError Loadable::getSerializedDataSize(NvU64* size)
{
  NvDlaError e = NvDlaSuccess;
  if (size == NULL)
    ORIGINATE_ERROR_FAIL(NvDlaError_BadParameter);

  *size = mFbb.GetSize();

fail:
  return e;
}

bool Loadable::deserializeFrom(NvU8* flatbuf)
{
  const nvdla::loadable::Loadable* loadable = nvdla::loadable::GetLoadable(flatbuf);

  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::TaskListEntry>>*   task_list = loadable->task_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::SubmitListEntry>>* submit_list =
    loadable->submit_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::MemoryListEntry>>* memory_list =
    loadable->memory_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::AddressListEntry>>* address_list =
    loadable->address_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::EventListEntry>>* event_list = loadable->event_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::Blob>>*           blobs      = loadable->blobs();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::TensorDescListEntry>>* tensor_desc_list =
    loadable->tensor_desc_list();
  const flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::RelocListEntry>>* reloc_list = loadable->reloc_list();

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::TaskListEntry>>::const_iterator tli = task_list->begin();

  for (; tli != task_list->end(); ++tli) {
    ILoadable::TaskListEntry tle;

    tle.id        = tli->id();
    tle.interface = tli->interface();
    tle.instance  = tli->instance();

    if (tli->address_list()) {
      flatbuffers::Vector<short unsigned int>::const_iterator ali = tli->address_list()->begin();
      for (; ali != tli->address_list()->end(); ++ali) {
        tle.address_list.push_back(*ali);
      }
    }

    if (tli->pre_actions()) {
      flatbuffers::Vector<short unsigned int>::const_iterator preli = tli->pre_actions()->begin();
      for (; preli != tli->pre_actions()->end(); ++preli) {
        tle.preactions.push_back(*preli);
      }
    }

    if (tli->post_actions()) {
      flatbuffers::Vector<short unsigned int>::const_iterator postli = tli->post_actions()->begin();
      for (; postli != tli->post_actions()->end(); ++postli) {
        tle.postactions.push_back(*postli);
      }
    }
    mTaskListEntries.push_back(tle);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::SubmitListEntry>>::const_iterator sli = submit_list->begin();

  for (; sli != submit_list->end(); ++sli) {
    ILoadable::SubmitListEntry sle;

    sle.id = sli->id();

    if (sli->task_id()) {
      flatbuffers::Vector<short unsigned int>::const_iterator tli = sli->task_id()->begin();
      for (; tli != sli->task_id()->end(); ++tli) {
        sle.tasks.push_back(*tli);
      }
    }
    mSubmitListEntries.push_back(sle);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::MemoryListEntry>>::const_iterator li = memory_list->begin();

  for (; li != memory_list->end(); ++li) {
    ILoadable::MemoryListEntry mle;

    mle.id             = li->id();
    mle.size           = li->size();
    mle.alignment      = li->alignment();
    mle.flags          = li->flags();
    mle.domain         = li->domain();
    mle.bind_id        = li->bind_id();
    mle.tensor_desc_id = li->tensor_desc_id();

    if (li->contents()) {
      flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>::const_iterator mli = li->contents()->begin();
      for (; mli != li->contents()->end(); ++mli) {
        mle.contents.push_back(mli->str());
      }
    }

    if (li->offsets()) {
      flatbuffers::Vector<uint64_t>::const_iterator mli = li->offsets()->begin();
      for (; mli != li->offsets()->end(); ++mli) {
        mle.offsets.push_back(*mli);
      }
    }
    mMemoryListEntries.push_back(mle);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::AddressListEntry>>::const_iterator adli =
    address_list->begin();

  for (; adli != address_list->end(); ++adli) {
    ILoadable::AddressListEntry ale;

    ale.size   = adli->size();
    ale.offset = adli->offset();
    ale.mem_id = adli->mem_id();
    ale.id     = adli->id();

    mAddressListEntries.push_back(ale);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::EventListEntry>>::const_iterator eli = event_list->begin();

  for (; eli != event_list->end(); ++eli) {
    ILoadable::EventListEntry ele;

    ele.id     = eli->id();
    ele.op     = eli->op();
    ele.target = eli->target();
    ele.val    = eli->val();

    mEventListEntries.push_back(ele);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::Blob>>::const_iterator bi = blobs->begin();

  for (; bi != blobs->end(); ++bi) {
    std::string blob_name = bi->name()->str();

    mSymbols[blob_name].name              = blob_name;
    mSymbols[blob_name].size              = bi->size();
    mSymbols[blob_name].version.major     = bi->version()->major();
    mSymbols[blob_name].version.minor     = bi->version()->minor();
    mSymbols[blob_name].version.sub_minor = bi->version()->sub_minor();
    mSymbols[blob_name].interface         = (nvdla::ILoadable::Interface)bi->interface();
    mSymbols[blob_name].subInterface      = bi->sub_interface();

    NvU8* blob_data = new NvU8[mSymbols[blob_name].size];
    memset(blob_data, 0, mSymbols[blob_name].size);

    NvU8* binblob = (NvU8*)bi->data()->Data();
    memcpy((void*)blob_data, (void*)binblob, mSymbols[blob_name].size);

    mSymbols[blob_name].data = blob_data;
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::TensorDescListEntry>>::const_iterator tdle =
    tensor_desc_list->begin();

  for (; tdle != tensor_desc_list->end(); ++tdle) {
    ILoadable::TensorDescListEntry ele;

    ele.name   = tdle->name()->str();
    ele.id     = tdle->id();
    ele.memId  = tdle->mem_id();
    ele.size   = tdle->size();
    ele.offset = tdle->offset();

    ele.dataFormat   = tdle->data_format();
    ele.dataType     = tdle->data_type();
    ele.dataCategory = tdle->data_category();
    ele.pixelFormat  = tdle->pixel_format();
    ele.pixelMapping = tdle->pixel_mapping();

    ele.dims.n = tdle->n();
    ele.dims.c = tdle->c();
    ele.dims.h = tdle->h();
    ele.dims.w = tdle->w();

    ele.stride[0] = tdle->stride_0();
    ele.stride[1] = tdle->stride_1();
    ele.stride[2] = tdle->stride_2();
    ele.stride[3] = tdle->stride_3();
    ele.stride[4] = tdle->stride_4();
    ele.stride[5] = tdle->stride_5();
    ele.stride[6] = tdle->stride_6();
    ele.stride[7] = tdle->stride_7();

    mTensorDescListEntries.push_back(ele);
  }

  flatbuffers::Vector<flatbuffers::Offset<nvdla::loadable::RelocListEntry>>::const_iterator rle = reloc_list->begin();

  for (; rle != reloc_list->end(); ++rle) {
    ILoadable::RelocEntry ele(rle->address_id(), rle->write_id(), rle->offset(), rle->interface(), rle->sub_interface(),
                              rle->reloc_type());
    mRelocEntries.push_back(ele);
  }

  return true;
}

} // namespace priv

} // namespace nvdla
