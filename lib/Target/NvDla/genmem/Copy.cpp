#include "Operators.hpp"

using namespace genmem;

// Copy in_cube to out_cube
void Copy::copy(Cube& in_cube, Cube& out_cube, Memory& mem, vector<Register>& regs, CopyMode mode)
{
  vector<Cube> in_cubes{in_cube};
  vector<Cube> out_cubes{out_cube};
  copy(in_cubes, out_cubes, mem, regs, mode);
}

// Copy for each in_cubes[i] to out_cubes[i]
void Copy::copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs, CopyMode mode)
{
  GEN_assert(in_cubes.size() == out_cubes.size());
  if (mode == CopyMode::BDMA_COPY)
    bdma_copy(in_cubes, out_cubes, mem, regs);
  else if (mode == CopyMode::SPLIT_MERGE_COPY)
    split_merge_copy(in_cubes, out_cubes, mem, regs);
  else
    GEN_assert(false);
}

void Copy::bdma_copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs)
{
  int num = in_cubes.size();

  for (int iter = 0; iter < ceil(1.0 * num / genmem::NUM_MAX_BDMA_TRANSFER); iter++) {
    Register reg;
    reg.set_mode(RUBIK_MODE::BDMAMODE);
    int start = iter * NUM_MAX_BDMA_TRANSFER;
    int end   = min((iter + 1) * NUM_MAX_BDMA_TRANSFER, num);
    for (int index = start; index < end; index++) {
      reg.add_bdma_transfer(in_cubes[index], out_cubes[index]);
    }
    regs.push_back(reg);
  }
}

void Copy::split_merge_copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs)
{
  for (int i = 0; i < in_cubes.size(); i++) {
    Cube& from = in_cubes[i];
    Cube& to   = out_cubes[i];

    GEN_assert(from.type == Cube::TYPE::CUBE);
    GEN_assert(to.type == Cube::TYPE::CUBE);

    Cube split_out(Cube::TYPE::PLANAR, from.W, from.H, from.C,
                   Cube::get_compact_line_stride(Cube::TYPE::PLANAR, from.W, from.H, from.C), -1,
                   Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, from.W, from.H, from.C));
    split_out.allocate_memory(mem);

    Register reg_split;
    Split::split(from, split_out, mem, reg_split);
    regs.push_back(reg_split);

    Register reg_merge;
    Merge::merge(split_out, to, mem, reg_merge);
    regs.push_back(reg_merge);
  }
}
