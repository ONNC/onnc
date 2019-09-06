#ifndef GENMEM_MERGE_H
#define GENMEM_MERGE_H

#include "Cube.hpp"
#include "Register.hpp"
#include "header.hpp"

namespace genmem {

class Merge
{
public:
  static void init_stride(int W, int H, int C, int line_stride[2], int surf_stride[2], int planar_stride[2]);
  static void merge(Cube& in_cube, Cube& out_cube, Memory& mem, Register& reg);
};

} // namespace genmem
#endif
