#include "Memory.hpp"

using namespace genmem;

Memory::Memory(int memWidth)
  : memWidth(memWidth)
{
  used_entry = 0;
}

int Memory::request_memory(int total_entry)
{
  int given_entry = used_entry;
  used_entry += total_entry;
  return given_entry;
}
