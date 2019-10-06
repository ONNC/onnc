
#include "genmem.hpp"

namespace genmem {

unsigned long long start_address         = 0xc0000000;
int                ATOMIC_K              = 16;
int                elementSize           = 2;
int                memWidth              = 32;
int                NUM_MAX_BDMA_TRANSFER = 20;

void init_genmem(int eleSize, int start_addr)
{
  elementSize = eleSize;
  ATOMIC_K    = elementSize == 1 ? 32 : 16;

  start_address = start_addr;
}

} // namespace genmem
