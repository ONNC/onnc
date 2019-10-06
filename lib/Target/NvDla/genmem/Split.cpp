#include "Split.hpp"

using namespace genmem;

void Split::init_stride(int W, int H, int C, int line_stride[2], int surf_stride[2], int planar_stride[2])
{
  if (line_stride[DATAIN] == -1 or surf_stride[DATAIN] == -1) {
    line_stride[DATAIN] = Cube::get_compact_line_stride(Cube::TYPE::CUBE, W, H, C);
    surf_stride[DATAIN] = Cube::get_compact_surface_stride(Cube::TYPE::CUBE, W, H, C);
  }
  planar_stride[DATAIN] = -1;

  if (line_stride[DATAOUT] == -1 or planar_stride[DATAOUT] == -1) {
    line_stride[DATAOUT]   = Cube::get_compact_line_stride(Cube::TYPE::PLANAR, W, H, C);
    planar_stride[DATAOUT] = Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, W, H, C);
  }
  surf_stride[DATAOUT] = -1;
}

// split with
/*
 * 0. fill register
 * 1. load cube from memory
 * 2. manipulate data
 * 3. write back data to memory
 */
void Split::split(Cube& in_cube, Cube& out_cube, Memory& mem, Register& reg)
{
  reg.rubik_mode = SPLITMODE;

  reg.fill_register_with_in_cube(in_cube);
  reg.fill_register_with_out_cube(out_cube);
}
