
#include "NvdlaMemory.hpp"

using namespace genmem;

NvDlaMemory::NvDlaMemory(int memWidth, int memoryListEntryId)
  : Memory(memWidth)
{
  memory_list_entry_id = memoryListEntryId;
}

int NvDlaMemory::request_memory(int total_entry)
{
  GEN_assert(memory_list_entry_id != -1);

  int start                    = request_memory_with_mid(total_entry, memory_list_entry_id);
  mle_info.back().should_alloc = true;

  memory_list_entry_id++;
  return start;
}

int NvDlaMemory::request_memory_with_mid(int total_entry, int mid)
{
  int start = Memory::request_memory(total_entry);

  mid_to_mle_info_id[mid] = mle_info.size();
  mle_info.emplace_back(total_entry * memWidth, mid, start, 0);

  GEN_assert(address_entry_to_mid.size() == start);
  for (int i = 0; i < total_entry; i++)
    address_entry_to_mid.push_back(mid);

  return start;
}

int NvDlaMemory::genmem_addrress_entry_to_mid(int address_entry) const
{
  if (address_entry >= address_entry_to_mid.size() or address_entry < 0) {
    std::cout << "[Fatal] " << address_entry << " is not allocated to obtain mid" << std::endl;
    exit(-1);
  }
  return address_entry_to_mid[address_entry];
}

int NvDlaMemory::get_ale_offset(int address_entry) const
{
  int mid = genmem_addrress_entry_to_mid(address_entry);

  auto it = mid_to_mle_info_id.find(mid);
  if (it == mid_to_mle_info_id.end()) {
    std::cout << "[Fatal] illegal mid" << mid << " which is not used in genmem.\n";
    exit(-1);
  } else {
    GEN_assert(address_entry >= mle_info[it->second].start_entry_id);
    return (address_entry - mle_info[it->second].start_entry_id) * memWidth;
  }
}
