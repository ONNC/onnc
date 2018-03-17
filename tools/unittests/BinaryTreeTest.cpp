//===- BinaryTreeTest.cpp -------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, 2014, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/ADT/BinaryTree.h>
#include <onnc/ADT/Rope.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BinaryTreeTest
//===----------------------------------------------------------------------===//
/// merge two trees
SKYPAT_F(BinaryTreeTest, merge_two_trees)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  int a = 0, b = 1, c = 1, d = 2, e = 2;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, a);
  --pos;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, b);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos, c);
  --pos;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, d);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos, e);
  --pos;

  EXPECT_EQ(tree.size(), 5);

  BinaryTree<int> tree2;
  BinaryTree<int>::iterator pos2 = tree2.root();
  tree2.join<bintree::TreeIteratorBase::Rightward>(pos2, b);
  --pos2;
  tree2.join<bintree::TreeIteratorBase::Rightward>(pos2, c);
  tree2.join<bintree::TreeIteratorBase::Leftward>(pos2, c);

  EXPECT_EQ(tree2.size(), 3);

  tree.merge<bintree::TreeIteratorBase::Rightward>(pos,tree2);

  EXPECT_EQ(tree.size(), 8);
}

/// merge two trees, left is null tree
SKYPAT_F(BinaryTreeTest, merge_two_trees_left_is_null_tree)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  int a = 0, b = 1, c = 2;
  BinaryTree<int> tree2;
  tree2.join<bintree::TreeIteratorBase::Rightward>(pos, a);
  --pos;
  tree2.join<bintree::TreeIteratorBase::Rightward>(pos, b);
  tree2.join<bintree::TreeIteratorBase::Leftward>(pos, b);
  --pos;
  tree2.join<bintree::TreeIteratorBase::Rightward>(pos, c);
  tree2.join<bintree::TreeIteratorBase::Leftward>(pos, c);

  tree.merge<bintree::TreeIteratorBase::Rightward>(pos,tree2);

  EXPECT_TRUE(tree.size()==5);
}

/// merge trees, right is null tree
SKYPAT_F(BinaryTreeTest, merge_two_trees_right_is_null_tree)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  int a = 0, b = 1, c = 1, d = 2, e = 2;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, a);
  --pos;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, b);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos, c);
  --pos;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, d);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos, e);

  BinaryTree<int> tree2;
  BinaryTree<int>::iterator pos2 = tree2.root();
  tree2.merge<bintree::TreeIteratorBase::Rightward>(pos2,tree);

  EXPECT_TRUE(tree2.size()==5);
}

/// merge two null trees
SKYPAT_F(BinaryTreeTest, merge_two_null_trees)
{
  BinaryTree<int> tree, tree2;
  BinaryTree<int>::iterator pos = tree.root();
  BinaryTree<int>::iterator pos2 = tree2.root();

  tree2.merge<bintree::TreeIteratorBase::Rightward>(pos2,tree);

  EXPECT_TRUE(tree2.size()==0);
}

/// DFS traveral
SKYPAT_F(BinaryTreeTest, dfs_travesal)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();

  int a = 111, b = 10, c = 9, d = 8, e = 7;
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,a);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,b);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,c);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,d);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,e);

  BinaryTree<int>::dfs_iterator dfs_it = tree.dfs_begin();
  BinaryTree<int>::dfs_iterator dfs_end = tree.dfs_end();

  ASSERT_EQ(111, **dfs_it);
  ++dfs_it;
  EXPECT_EQ(9, **dfs_it);
  ++dfs_it;
  EXPECT_EQ(7, **dfs_it);
  ++dfs_it;
  EXPECT_EQ(8, **dfs_it);
  ++dfs_it;
  EXPECT_EQ(10, **dfs_it);
  ++dfs_it;
  EXPECT_TRUE( dfs_it ==  dfs_end);
}

/// DFS to build right-most tree
SKYPAT_F(BinaryTreeTest, DFS_right_most_tree)
{
  int a = 0, b = 1, c = 2, d = 3, e = 4;
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,a);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,b);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,c);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,d);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,e);

  BinaryTree<int>::dfs_iterator dfs_it = tree.dfs_begin();
  BinaryTree<int>::dfs_iterator dfs_end = tree.dfs_end();

  ASSERT_EQ(0, **dfs_it);
  ++dfs_it;
  ASSERT_EQ(1, **dfs_it);
  ++dfs_it;
  ASSERT_EQ(2, **dfs_it);
  ++dfs_it;
  ASSERT_EQ(3, **dfs_it);
  ++dfs_it;
  ASSERT_EQ(4, **dfs_it);
  ++dfs_it;
  ASSERT_TRUE( dfs_it ==  dfs_end);
}


/// DFS for a single node
SKYPAT_F(BinaryTreeTest, DFSIterator_SingleNode)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  int a = 0;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, a);
  BinaryTree<int>::dfs_iterator dfs_it = tree.dfs_begin();
  BinaryTree<int>::dfs_iterator dfs_end = tree.dfs_end();
  int counter = 0;
  while ( dfs_it != dfs_end ) {
    ++counter;
    ++dfs_it;
  }
  ASSERT_EQ(1, counter);
}

/// BFS traversal
SKYPAT_F(BinaryTreeTest, bfs_traversal)
{
  int a = 111, b = 10, c = 9, d = 8, e = 7;
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();

  tree.join<bintree::TreeIteratorBase::Leftward>(pos,a);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,b);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,c);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,d);
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,e);

  BinaryTree<int>::bfs_iterator bfs_it = tree.bfs_begin();
  BinaryTree<int>::bfs_iterator bfs_end = tree.bfs_end();

  ASSERT_EQ(111, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(10, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(9, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(8, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(7, **bfs_it);
  ++bfs_it;
  ASSERT_TRUE(bfs_it ==  bfs_end);
  bfs_it = tree.bfs_begin();
  bfs_end = tree.bfs_end();
}

/// BFS to build right-most tree
SKYPAT_F(BinaryTreeTest, BFS_right_most)
{
  int a = 0, b = 1, c = 2, d = 3, e = 4;
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,a);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,b);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,c);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,d);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,e);

  BinaryTree<int>::bfs_iterator bfs_it = tree.bfs_begin();
  BinaryTree<int>::bfs_iterator bfs_end = tree.bfs_end();

  ASSERT_EQ(0, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(1, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(2, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(3, **bfs_it);
  ++bfs_it;
  ASSERT_EQ(4, **bfs_it);
  ++bfs_it;
  ASSERT_TRUE( bfs_it ==  bfs_end);
}

/// BFS for a single node
SKYPAT_F(BinaryTreeTest, BFSIterator_SingleNode)
{
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  int a = 0;
  tree.join<bintree::TreeIteratorBase::Rightward>(pos, a);
  BinaryTree<int>::bfs_iterator bfs_it = tree.bfs_begin();
  BinaryTree<int>::bfs_iterator bfs_end = tree.bfs_end();
  int counter = 0;
  while ( bfs_it != bfs_end ) {
    ++counter;
    ++bfs_it;
  }
  ASSERT_EQ(1, counter);
}

/// Normal iterator
SKYPAT_F(BinaryTreeTest, norma_tree_iterator)
{
  int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5;
  BinaryTree<int> tree;
  BinaryTree<int>::iterator pos = tree.root();
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,a);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,b);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Leftward>(pos,c);
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,f);
  pos.move<bintree::TreeIteratorBase::Leftward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,d);
  pos.move<bintree::TreeIteratorBase::Rightward>();
  tree.join<bintree::TreeIteratorBase::Rightward>(pos,e);

  BinaryTree<int>::iterator it = tree.begin();
  BinaryTree<int>::iterator end = tree.end();

  ASSERT_EQ(0, **it);
  ++it;
  ASSERT_EQ(1, **it);
  --it;
  ASSERT_EQ(2, **it);
  ++it;
  ASSERT_EQ(3, **it);
  ++it;
  ASSERT_EQ(4, **it);
  ++it;
  ASSERT_TRUE(it == end);

  it = tree.begin();
  ++it;
  ++it;
  ASSERT_EQ(5, **it);
  ++it;
  ASSERT_TRUE(it == end);
}
