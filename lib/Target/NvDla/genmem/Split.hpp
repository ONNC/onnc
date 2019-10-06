#ifndef GENMEM_SPLIT_H
#define GENMEM_SPLIT_H

#include "Cube.hpp"
#include "Register.hpp"
#include "header.hpp"

namespace genmem {

class Split
{
public:
  static void init_stride(int W, int H, int C, int line_stride[2], int surf_stride[2], int planar_stride[2]);

  static void split(Cube& in_cube, Cube& out_cube, Memory& mem, Register& reg);
};

} // namespace genmem
#endif
