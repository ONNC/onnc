#include "Operators.hpp"

#include <iostream>
#include <vector>

using namespace genmem;

void Concat::concat(vector<Cube>& in_cube, Cube& out_cube, Memory& mem, vector<Register>& regs, int axis,
                    int input_tensor_num)
{
  switch (axis) {
  case 0:
    concat_axis_0(in_cube, out_cube, mem, regs, input_tensor_num);
    break;
  }
}

void Concat::concat_axis_0(vector<Cube>& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs,
                           int input_tensor_num)
{
  Cube before_merge_cube(
    Cube::TYPE::PLANAR, output_cube.W, output_cube.H, output_cube.C,
    Cube::get_compact_line_stride(Cube::TYPE::PLANAR, output_cube.W, output_cube.H, output_cube.C), -1,
    Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, output_cube.W, output_cube.H, output_cube.C));
  before_merge_cube.allocate_memory(mem);

  // split
  for (int i = 0; i < input_tensor_num; i++) {
    // output planar
    Cube out_cube_split(
      Cube::TYPE::PLANAR, input_cube[i].W, input_cube[i].H, input_cube[i].C,
      Cube::get_compact_line_stride(Cube::TYPE::PLANAR, input_cube[i].W, input_cube[i].H, input_cube[i].C), -1,
      Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, input_cube[i].W, input_cube[i].H, input_cube[i].C));
    GEN_assert(before_merge_cube.planar_stride == out_cube_split.planar_stride);
    GEN_assert(before_merge_cube.line_stride == out_cube_split.line_stride);

    int total_planar_used = 0;
    for (int j = 0; j < i; j++) {
      total_planar_used += input_cube[j].C;
    }
    out_cube_split.address_entry = before_merge_cube.address_entry + total_planar_used * out_cube_split.planar_stride;

    // split
    Register reg;
    Split::split(input_cube[i], out_cube_split, mem, reg);
    regs.push_back(reg);
  }

  // merge
  Register reg;
  Merge::merge(before_merge_cube, output_cube, mem, reg);
  regs.push_back(reg);
}
