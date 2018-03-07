//===- DigraphTest.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/Digraph.h>
#include <onnc/ADT/NodeIterator.h>
#include <onnc/ADT/ArcIterator.h>
#include <onnc/Support/IOStream.h>

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
  MyNode(int pData) : data(pData) { }

  int data;
};

struct MyArc : public onnc::DigraphArc<MyNode, MyArc>
{
  MyArc(int pData) : value(pData) {}

  int value;
};

typedef Digraph<MyNode, MyArc> MyGraph;

} // anonymous namespace

SKYPAT_F(DigraphTest, trivial)
{
  MyGraph g;
  MyNode* n1 = g.addNode(5);
  MyNode* n2 = g.addNode(4);

  MyArc* a = g.connect(*n1, *n2, 3);

  ASSERT_EQ(n1->data, a->getSource()->data);
  ASSERT_EQ(n2->data, a->getTarget()->data);
  ASSERT_EQ(g.getNodeSize(), 2);
  ASSERT_EQ(g.getArcSize(), 1);
}

SKYPAT_F(DigraphTest, clear)
{
  MyGraph g;
  MyNode* n1 = g.addNode(5);
  MyNode* n2 = g.addNode(4);

  MyArc* a = g.connect(*n1, *n2, 3);

  ASSERT_EQ(n1->data, a->getSource()->data);
  ASSERT_EQ(n2->data, a->getTarget()->data);
  ASSERT_EQ(g.getNodeSize(), 2);
  ASSERT_EQ(g.getArcSize(), 1);

  g.clear();
  ASSERT_EQ(g.getNodeSize(), 0);
  ASSERT_EQ(g.getArcSize(), 0);
}

SKYPAT_F(DigraphTest, node_iterator)
{
  MyGraph g;
  MyNode* n1 = g.addNode(5);
  MyNode* n2 = g.addNode(4);
  g.addNode(3);
  g.addNode(2);
  g.connect(*n1, *n2, 3);

  MyGraph::node_iterator iter = g.nodeBegin();
  ASSERT_EQ(iter->data, 5);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 4);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 3);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 2);
  ASSERT_FALSE(iter.hasNext());
  ++iter;
  ASSERT_TRUE(iter == g.nodeEnd());
}

SKYPAT_F(DigraphTest, node_erase_head)
{
  MyGraph g;
  MyNode* n1 = g.addNode(5);
  MyNode* n2 = g.addNode(4);
  g.addNode(3);
  g.addNode(2);
  g.connect(*n1, *n2, 3);
  g.erase(*n1);

  NodeIterator<MyNode> iter(n1);
  // n1 became a free node. g shall not delete n1 until its destrunction.
  ASSERT_EQ(n1->data, 5);
  ASSERT_FALSE(iter.hasNext());
}

SKYPAT_F(DigraphTest, node_erase_middle)
{
  MyGraph g;
  MyNode* n1 = g.addNode(1);
  MyNode* n2 = g.addNode(2);
  g.addNode(3);
  g.addNode(4);
  g.connect(*n1, *n2, 3);

  g.erase(*n2);

  MyGraph::node_iterator iter = g.nodeBegin();
  ASSERT_EQ(iter->data, 1);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 3);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 4);
  ASSERT_FALSE(iter.hasNext());
}

SKYPAT_F(DigraphTest, node_erase_rear)
{
  MyGraph g;
  MyNode* n1 = g.addNode(1);
  MyNode* n2 = g.addNode(2);
  g.addNode(3);
  MyNode* n4 = g.addNode(4);
  g.connect(*n1, *n2, 3);

  g.erase(*n4);

  NodeIterator<MyNode> iter(n1);
  ASSERT_EQ(iter->data, 1);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 2);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->data, 3);
  ASSERT_FALSE(iter.hasNext());
}

SKYPAT_F(DigraphTest, edge_iterator)
{
  MyGraph g;
  MyNode* n1 = g.addNode(1);
  MyNode* n2 = g.addNode(2);
  MyNode* n3 = g.addNode(3);
  MyNode* n4 = g.addNode(4);
  MyNode* n5 = g.addNode(5);
  g.connect(*n1, *n2, 1);
  g.connect(*n1, *n3, 2);
  g.connect(*n3, *n2, 3);
  g.connect(*n2, *n4, 4);
  g.connect(*n3, *n5, 5);

  InArcIterator<MyNode, MyArc> iter(*n2);
  ASSERT_EQ(iter->value, 3);
  ASSERT_TRUE(iter.hasNext());
  ++iter;
  ASSERT_EQ(iter->value, 1);
  ASSERT_FALSE(iter.hasNext());
  ++iter;
  ASSERT_TRUE(iter.isEnd());

  OutArcIterator<MyNode, MyArc> out(*n1);
  ASSERT_EQ(out->value, 2);
  ASSERT_TRUE(out.hasNext());
  ++out;
  ASSERT_EQ(out->value, 1);
  ASSERT_FALSE(out.hasNext());
  ++out;
  ASSERT_TRUE(out.isEnd());
}
