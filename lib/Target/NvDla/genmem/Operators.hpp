#ifndef GENMEM_OPERATOR_H
#define GENMEM_OPERATOR_H

#include "Cube.hpp"
#include "Memory.hpp"
#include "Merge.hpp"
#include "Register.hpp"
#include "Split.hpp"
#include "header.hpp"

namespace genmem {

class SliceParm
{
public:
  SliceParm() {}

  SliceParm(std::initializer_list<vector<int>> li)
  {
    for (const auto& i : li)
      parm.push_back(i);
  }

  int size() const { return parm.size(); }

  vector<vector<int>> parm;
};

class Slice
{
public:
  static void init_stride(int W, int H, int C, int line_stride[2], int surf_stride[2], int planar_stride[2],
                          const SliceParm& parm);

  static vector<int> get_new_shape(int W, int H, int C, const SliceParm& parm);
  static vector<int> get_new_shape(const vector<int>& shape, const SliceParm& parm);

  static void slice(Cube& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs, const SliceParm& parm);

  static void print_information(int W, int H, int C, const SliceParm& parm);

private:
  static void slice_c(Cube& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs,
                      const vector<int>& parm);

  static void slice_c_negative_step(Cube& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs,
                                    const vector<int>& parm);

  static vector<int> get_valid_start_end_step(const vector<int>& parm, int lim);
  static bool        is_stop(int i, int end, int step)
  {
    if (step < 0)
      return i >= end;
    else
      return i <= end;
  };
};

class Concat
{
public:
  static void concat(vector<Cube>& in_cube, Cube& out_cube, Memory& mem, vector<Register>& regs, int axis,
                     int input_tensor_num);

private:
  static void concat_axis_0(vector<Cube>& input_cube, Cube& output_cube, Memory& mem, vector<Register>& regs,
                            int input_tensor_num);
};

class Shuffle
{
public:
  static void shuffle(Cube& in_cube, Cube& out_cube, Memory& mem, vector<Register>& regs, int axis, int group);
};

class Copy
{
public:
  enum CopyMode
  {
    BDMA_COPY = 0,
    SPLIT_MERGE_COPY
  };

  static void copy(Cube& in_cube, Cube& out_cube, Memory& mem, vector<Register>& reg, CopyMode copy_mode = BDMA_COPY);
  static void copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs,
                   CopyMode copy_mode = BDMA_COPY);

private:
  static void bdma_copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs);
  static void split_merge_copy(vector<Cube>& in_cubes, vector<Cube>& out_cubes, Memory& mem, vector<Register>& regs);
};

class SplittParm
{
public:
  SplittParm() {}

  SplittParm(std::initializer_list<int> li)
  {
    for (const auto& i : li)
      parm.push_back(i);
  }

  int         size() const { return parm.size(); }
  vector<int> parm;
};

class Splitt
{
public:
  static SplittParm read_attribute(int argc, char* []);
  static void       splitt(Cube& input_cube, vector<Cube>& output_cube, Memory& mem, vector<Register>& regs, int axis,
                           const SplittParm& parm);

private:
  static void splitt_c(Cube& input_cube, vector<Cube>& output_cube, Memory& mem, vector<Register>& regs,
                       const vector<int>& parm);
};

} // namespace genmem

#endif
