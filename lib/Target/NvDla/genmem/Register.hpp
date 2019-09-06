#ifndef GENMEN_REGISTER_H
#define GENMEN_REGISTER_H

#include "Cube.hpp"
#include "header.hpp"

namespace genmem {

class BDMATransfer;

class Register
{
public:
  void set_mode(int mode) { rubik_mode = mode; }
  void fill_register_with_in_cube(const Cube& cube);
  void fill_register_with_out_cube(const Cube& cube);
  void add_bdma_transfer(const Cube& source, const Cube& dest);

  Register()
  {
    if (elementSize == 1)
      in_precison = 0;
    else if (elementSize == 2)
      in_precison = 1;
    else
      in_precison = -1;
  }

  int ram_type[2] = {MCIF, MCIF};
  int in_precison = -1;
  // into two classes
  int rubik_mode;
  int W, H, C;

  // All strides are in entry
  int line_stride[2];
  int surf_stride[2];
  int planar_stride[2];
  int high_address[2];
  int low_address[2]; // 27-bits : entry number

  int dataout_channel = 0;
  int deconv_x_stride = 0;
  int deconv_y_stride = 0;

  int contract_stride_0 = 0;
  int contract_stride_1 = 0;

  vector<BDMATransfer> bdma_transfer;

  void add_bdma_transfer_cube(const Cube& source, const Cube& dest);
  void add_bdma_transfer_planar(const Cube& source, const Cube& dest);
};

class BDMATransfer
{
public:
  int source_type;
  int destination_type;

  int source_entry; // entry in genmem:Memory
  int destination_entry;

  // valid data size per line
  int line_size;

  // H
  int line_repeat;

  // Number of bytes per src/dst line (padding are included).
  int source_line;
  int destination_line;

  // Number of surfaces in one data cube
  int surface_repeat;

  // Number of bytes per surface (line padding are included).
  int source_surface;
  int destination_surface;
};

} // namespace genmem
#endif
