#include "Register.hpp"

using namespace genmem;

void Register::fill_register_with_in_cube(const Cube& cube)
{
  W                     = cube.W;
  H                     = cube.H;
  C                     = cube.C;
  line_stride[DATAIN]   = cube.line_stride > 0 ? cube.line_stride : 0;
  surf_stride[DATAIN]   = cube.surf_stride > 0 ? cube.surf_stride : 0;
  planar_stride[DATAIN] = cube.planar_stride > 0 ? cube.planar_stride : 0;

  high_address[DATAIN] = 0;
  low_address[DATAIN]  = cube.address_entry;
}

void Register::fill_register_with_out_cube(const Cube& cube)
{
  line_stride[DATAOUT]   = cube.line_stride > 0 ? cube.line_stride : 0;
  surf_stride[DATAOUT]   = cube.surf_stride > 0 ? cube.surf_stride : 0;
  planar_stride[DATAOUT] = cube.planar_stride > 0 ? cube.planar_stride : 0;

  high_address[DATAOUT] = 0;
  low_address[DATAOUT]  = cube.address_entry;

  dataout_channel = cube.C;
}

void Register::add_bdma_transfer(const Cube& source, const Cube& dest)
{
  GEN_assert(rubik_mode == BDMAMODE);
  GEN_assert(source.type == dest.type);

  if (source.type == Cube::TYPE::CUBE)
    add_bdma_transfer_cube(source, dest);
  else
    add_bdma_transfer_planar(source, dest);
}

void Register::add_bdma_transfer_cube(const Cube& source, const Cube& dest)
{
  BDMATransfer trans;

  trans.source_type      = MCIF;
  trans.destination_type = MCIF;

  trans.source_entry      = source.address_entry;
  trans.destination_entry = dest.address_entry;

  trans.line_size   = source.W * genmem::memWidth;
  trans.line_repeat = source.H;

  trans.source_line      = source.line_stride * genmem::memWidth;
  trans.destination_line = dest.line_stride * genmem::memWidth;
  ;

  trans.surface_repeat = ceil(1.0 * source.C / ATOMIC_K);
  trans.source_surface = source.surf_stride * genmem::memWidth;
  ;
  trans.destination_surface = dest.surf_stride * genmem::memWidth;
  bdma_transfer.push_back(std::move(trans));
}

void Register::add_bdma_transfer_planar(const Cube& source, const Cube& dest)
{
  BDMATransfer trans;

  trans.source_type      = MCIF;
  trans.destination_type = MCIF;

  trans.source_entry      = source.address_entry;
  trans.destination_entry = dest.address_entry;

  trans.line_size   = ceil(1.0 * source.W / ATOMIC_K) * genmem::memWidth;
  trans.line_repeat = source.H;

  trans.source_line      = source.line_stride * genmem::memWidth;
  trans.destination_line = dest.line_stride * genmem::memWidth;
  ;

  trans.surface_repeat = source.C;
  trans.source_surface = source.planar_stride * genmem::memWidth;
  ;
  trans.destination_surface = dest.planar_stride * genmem::memWidth;
  bdma_transfer.push_back(std::move(trans));
}
