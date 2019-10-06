#include "Operators.hpp"

using namespace genmem;

SplittParm Splitt::read_attribute(int argc, char* argv[])
{
  SplittParm parm;
  for (int i = 0; i < argc; i++) {
    parm.parm.push_back(atoi(argv[i]));
  }
  return parm;
}

void Splitt::splitt(Cube& input_cube, vector<Cube>& output_cube, Memory& mem, vector<Register>& regs, int axis,
                    const SplittParm& parm)
{
  GEN_assert(axis == 0);

  splitt_c(input_cube, output_cube, mem, regs, parm.parm);
}

void Splitt::splitt_c(Cube& input_cube, vector<Cube>& output_cube, Memory& mem, vector<Register>& regs,
                      const vector<int>& parm)
{
  Cube split_out(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C,
                 Cube::get_compact_line_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C), -1,
                 Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, input_cube.C));
  split_out.allocate_memory(mem);

  Register split_reg;
  Split::split(input_cube, split_out, mem, split_reg);
  regs.push_back(split_reg);

  for (int i = 0; i < parm.size(); i++) {
    Cube merge_in(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, parm[i],
                  Cube::get_compact_line_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, parm[i]), -1,
                  Cube::get_compact_planar_stride(Cube::TYPE::PLANAR, input_cube.W, input_cube.H, parm[i]));

    int planar_used = 0;
    for (int j = 0; j < i; j++) {
      planar_used += parm[j];
    }
    merge_in.address_entry = split_out.address_entry + split_out.planar_stride * planar_used;

    Register merge_reg;
    Merge::merge(merge_in, output_cube[i], mem, merge_reg);
    regs.push_back(merge_reg);
  }
}
