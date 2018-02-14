//===- DigraphTest.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/Digraph.h>

using namespace skypat;
using namespace onnc;

//===----------------------------------------------------------------------===//
// DigraphTest
//===----------------------------------------------------------------------===//
namespace {

struct MyNode;
struct MyArc;

struct MyNode : public onnc::DigraphNode<MyNode, MyArc>
{
  int data;
};

struct MyArc : public onnc::DigraphArc<MyNode, MyArc>
{
  int value;
};

typedef Digraph<MyNode, MyArc> MyGraph;

} // anonymous namespace

SKYPAT_F(DigraphTest, trivial)
{
  MyGraph g;
  MyNode* n1 = g.addNode();
  n1->data = 5;
  MyNode* n2 = g.addNode();
  n2->data = 4;

  MyArc* a = g.connect(*n1, *n2);
  a->value = 3;

  ASSERT_EQ(n1->data, a->getSource()->data);
  ASSERT_EQ(n2->data, a->getTarget()->data);
  ASSERT_EQ(g.getNodeSize(), 2);
  ASSERT_EQ(g.getArcSize(), 1);
}
