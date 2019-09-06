#ifndef GENMEM_NVDLA_MEMORY_H
#define GENMEM_NVDLA_MEMORY_H

#include "genmem/Memory.hpp"
#include "genmem/header.hpp"

namespace genmem {

class NvDlaMemory : public Memory
{
public:
  struct MleInfo
  {
    int  size;
    int  mid;
    int  start_entry_id;
    bool should_alloc; // should be alloced in Nvdla

    MleInfo(int size, int mid, int start_entry_id, bool should_alloc)
      : size(size)
      , mid(mid)
      , start_entry_id(start_entry_id)
      , should_alloc(should_alloc)
    {}
  };

  NvDlaMemory(int memWidth, int memoryListEntryId);

  int request_memory(int total_entry) override;
  // for the request from input tensor and output tensor
  int request_memory_with_mid(int total_entry, int mid);

  // Turn memory entry to MemoryListEntryID
  int genmem_addrress_entry_to_mid(int address_entry) const;

  int get_ale_offset(int address_entry) const;

  vector<MleInfo>& get_total_mle() { return mle_info; }

private:
  int             memory_list_entry_id = -1;
  vector<MleInfo> mle_info;
  vector<int>     address_entry_to_mid;
  map<int, int>   mid_to_mle_info_id;
};

} // namespace genmem

#endif
