//===- Tile.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tile.h>

using namespace onnc;

char Tile::ID = 0;

//===----------------------------------------------------------------------===//
// Tile
//===----------------------------------------------------------------------===//
Tile::Tile()
  : ComputeOperator("Tile", ID) {
}



void Tile::print(std::ostream& pOS) const
{
}

bool Tile::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
