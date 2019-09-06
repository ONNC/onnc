#ifndef GENMEM_MEMORY_H
#define GENMEM_MEMORY_H

#include "header.hpp"

namespace genmem {

class Memory
{
  using BYTE = int;

public:
  Memory(int memWidth);

  virtual int request_memory(int total_entry);

protected:
  int memWidth;
  int used_entry;
};

} // namespace genmem

#endif
