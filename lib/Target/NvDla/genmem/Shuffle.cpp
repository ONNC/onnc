#include "Operators.hpp"

#include <iostream>
#include <vector>

using namespace genmem;

void Shuffle::shuffle(Cube& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs, int axis, int group)
{
  GEN_assert(input_cube.C % group == 0);

  // step1: split
  int line_stride_split = Cube::get_compact_line_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C);
  int planar_stride_split =
    Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C);

  Cube out_cube_split(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C, line_stride_split, -1,
                      planar_stride_split);
  out_cube_split.allocate_memory(mem);

  // split
  Register reg;
  Split::split(input_cube, out_cube_split, mem, reg);
  regs.push_back(reg);

  // step2: bdma copy
  Cube after_copy(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C, line_stride_split, -1,
                  planar_stride_split);
  after_copy.allocate_memory(mem);

  vector<Cube> targets;
  vector<Cube> dests;
  for (int i = 0; i < input_cube.C / group; i++) {
    Cube target(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, group, line_stride_split, -1,
                planar_stride_split * (input_cube.C / group));
    target.address_entry = out_cube_split.address_entry + i * planar_stride_split;

    Cube dest(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, group, line_stride_split, -1, planar_stride_split);
    dest.address_entry = after_copy.address_entry + i * group * planar_stride_split;

    targets.push_back(target);
    dests.push_back(dest);

    if (targets.size() >= 20) {
      Copy::copy(targets, dests, mem, regs, Copy::CopyMode::BDMA_COPY);
      targets.clear();
      dests.clear();
    }
  }
  if (!targets.empty())
    Copy::copy(targets, dests, mem, regs, Copy::CopyMode::BDMA_COPY);

  // step3: merge
  Register reg_merge;
  Merge::merge(after_copy, output_cube, mem, reg_merge);
  regs.push_back(reg_merge);
}
