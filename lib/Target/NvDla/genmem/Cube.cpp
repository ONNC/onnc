#include "Cube.hpp"

using namespace genmem;

Cube::Cube(TYPE type, int w, int h, int c, int line_stride, int surf_stride, int planar_stride)
{
  W = w;
  H = h;
  C = c;

  this->line_stride   = line_stride;
  this->surf_stride   = surf_stride;
  this->planar_stride = planar_stride;
  this->type          = type;

  address_entry = -1;
}

void Cube::allocate_memory(Memory& mem) { address_entry = mem.request_memory(used_entry_number()); }

int Cube::used_entry_number() const
{
  if (type == TYPE::CUBE)
    return surf_stride * ceil(1.0 * C / ATOMIC_K);
  else
    return C * planar_stride;
}

int Cube::get_compact_line_stride(TYPE type, const vector<int>& shape)
{
  GEN_assert(shape.size() == 3);
  return get_compact_line_stride(type, shape[0], shape[1], shape[2]);
}

int Cube::get_compact_planar_stride(TYPE type, const vector<int>& shape)
{
  GEN_assert(shape.size() == 3);
  return get_compact_planar_stride(type, shape[0], shape[1], shape[2]);
}

int Cube::get_compact_surface_stride(TYPE type, const vector<int>& shape)
{
  GEN_assert(shape.size() == 3);
  return get_compact_surface_stride(type, shape[0], shape[1], shape[2]);
}

int Cube::get_compact_line_stride(TYPE type, int W, int H, int C)
{
  if (type == TYPE::CUBE)
    return W;
  else {
    int num = ceil(1.0 * W / ATOMIC_K);
    return num % 2 == 0 ? num : (num + 1);
  }
}

int Cube::get_compact_planar_stride(TYPE type, int W, int H, int C)
{
  if (type == TYPE::CUBE)
    cout << "[Warning] Type::Cube don't have planar stride\n";
  else
    return get_compact_line_stride(type, W, H, C) * H;

  return 0;
}

int Cube::get_compact_surface_stride(TYPE type, int W, int H, int C)
{
  if (type == TYPE::PLANAR)
    cout << "[Warning] Type::Planar don't have surface stride\n";
  else
    return get_compact_line_stride(type, W, H, C) * H;

  return 0;
}
