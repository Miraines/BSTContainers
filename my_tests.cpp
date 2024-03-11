#include <gtest/gtest.h>
#include "bst.h"


// Тесты для метода begin
TEST(BinarySearchTreeTest, BeginEmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_EQ(bst.begin<InOrder>(), bst.end<InOrder>()); // Дерево пустое, begin должен равняться end
}

TEST(BinarySearchTreeTest, BeginSingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  auto it = bst.begin<InOrder>();
  EXPECT_EQ(*it, 42); // Дерево с одним элементом, begin должен указывать на этот элемент
}

TEST(BinarySearchTreeTest, BeginMultipleElements) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  bst.insert(21);
  bst.insert(84);
  auto it = bst.begin<InOrder>();
  EXPECT_EQ(*it, 21); // Наименьший элемент дерева
}

TEST(BinarySearchTreeTest, BeginCheckIncrement) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  bst.insert(21);
  bst.insert(84);
  auto it = bst.begin<InOrder>();
  ++it;
  EXPECT_EQ(*it, 42); // Проверяем, что инкремент работает корректно
}

TEST(BinarySearchTreeTest, BeginCheckFullTraversal) {
  BinarySearchTree<int> bst;
  bst.insert(50);
  bst.insert(30);
  bst.insert(70);
  bst.insert(20);
  bst.insert(40);
  bst.insert(60);
  bst.insert(80);

  std::vector<int> elements;
  for(auto it = bst.begin<InOrder>(); it != bst.end<InOrder>(); ++it) {
    elements.push_back(*it);
  }

  std::vector<int> expected = {20, 30, 40, 50, 60, 70, 80};
  EXPECT_EQ(elements, expected); // Проверяем полный обход
}

// Тесты для метода end
TEST(BinarySearchTreeTest, EndEmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_EQ(bst.begin<InOrder>(), bst.end<InOrder>()); // Дерево пустое, begin должен равняться end
}

TEST(BinarySearchTreeTest, EndSingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  auto it = bst.end<InOrder>();
  --it;
  EXPECT_EQ(*it, 42); // Дерево с одним элементом, end - 1 должен указывать на этот элемент
}

TEST(BinarySearchTreeTest, EndMultipleElements) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  bst.insert(21);
  bst.insert(84);
  auto it = bst.end<InOrder>();
  --it;
  EXPECT_EQ(*it, 84); // Наибольший элемент дерева
}

TEST(BinarySearchTreeTest, EndCheckDecrement) {
  BinarySearchTree<int> bst;
  bst.insert(42);
  bst.insert(21);
  bst.insert(84);
  auto it = bst.cend<InOrder>();
  --it;
  --it;
  EXPECT_EQ(*it, 42); // Проверяем, что декремент работает корректно
}

TEST(BinarySearchTreeTest, Find_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.find(5);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 5);
}

TEST(BinarySearchTreeTest, Find_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.find(15);
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, Find_EmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.find(10);
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, Find_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto it = bst.find(10);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 10);
}

TEST(BinarySearchTreeTest, Exist_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_TRUE(bst.exist(5));
}

TEST(BinarySearchTreeTest, Exist_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_FALSE(bst.exist(15));
}

TEST(BinarySearchTreeTest, Exist_EmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_FALSE(bst.exist(10));
}

TEST(BinarySearchTreeTest, Exist_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  EXPECT_TRUE(bst.exist(10));
}

TEST(BinarySearchTreeTest, FindMin_NonEmptyTree) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.findMin();
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 5);
}

TEST(BinarySearchTreeTest, FindMin_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto it = bst.findMin();
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 10);
}

TEST(BinarySearchTreeTest, FindMin_EmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.findMin();
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, FindMax_NonEmptyTree) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.findMax();
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 20);
}

TEST(BinarySearchTreeTest, FindMax_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto it = bst.findMax();
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 10);
}

TEST(BinarySearchTreeTest, FindMax_EmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.findMax();
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, Erase_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_EQ(bst.erase(10), 1);
  EXPECT_FALSE(bst.exist(10));
}

TEST(BinarySearchTreeTest, Erase_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_EQ(bst.erase(15), 0);
}

TEST(BinarySearchTreeTest, Erase_EmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_EQ(bst.erase(10), 0);
}

TEST(BinarySearchTreeTest, Erase_AllElements) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  bst.erase(10);
  bst.erase(5);
  bst.erase(20);
  EXPECT_TRUE(bst.empty());
}


TEST(BinarySearchTreeTest, Extract_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.find(15);
  auto extracted = bst.extract(it);
  EXPECT_EQ(extracted.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, Extract_FromEmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.find(10);
  auto extracted = bst.extract(it);
  EXPECT_EQ(extracted.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, Extract_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto it = bst.find(10);
  auto extracted = bst.extract(it);
  EXPECT_EQ(extracted.get_node(), nullptr);
  EXPECT_TRUE(bst.empty());
}

TEST(BinarySearchTreeTest, Merge_NonEmptyTrees) {
  BinarySearchTree<int> bst1, bst2;
  bst1.insert(10);
  bst1.insert(5);
  bst2.insert(20);
  bst2.insert(15);
  bst1.merge(bst2);
  EXPECT_TRUE(bst2.empty());
  EXPECT_TRUE(bst1.exist(10) && bst1.exist(5) && bst1.exist(20) && bst1.exist(15));
}

TEST(BinarySearchTreeTest, Merge_IntoEmptyTree) {
  BinarySearchTree<int> bst1, bst2;
  bst2.insert(10);
  bst2.insert(5);
  bst1.merge(bst2);
  EXPECT_TRUE(bst2.empty());
  EXPECT_TRUE(bst1.exist(10) && bst1.exist(5));
}

TEST(BinarySearchTreeTest, Merge_FromEmptyTree) {
  BinarySearchTree<int> bst1, bst2;
  bst1.insert(10);
  bst1.insert(5);
  bst1.merge(bst2);
  EXPECT_TRUE(bst2.empty());
  EXPECT_TRUE(bst1.exist(10) && bst1.exist(5));
}

TEST(BinarySearchTreeTest, Merge_EmptyTrees) {
  BinarySearchTree<int> bst1, bst2;
  bst1.merge(bst2);
  EXPECT_TRUE(bst1.empty() && bst2.empty());
}

TEST(BinarySearchTreeTest, Count_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  bst.insert(5);
  EXPECT_EQ(bst.count(5), 1);
}

TEST(BinarySearchTreeTest, Count_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_EQ(bst.count(15), 0);
}

TEST(BinarySearchTreeTest, Count_EmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_EQ(bst.count(10), 0);
}

TEST(BinarySearchTreeTest, Count_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  EXPECT_EQ(bst.count(10), 1);
}

TEST(BinarySearchTreeTest, Contains_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_TRUE(bst.contains(5));
}

TEST(BinarySearchTreeTest, Contains_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  EXPECT_FALSE(bst.contains(15));
}

TEST(BinarySearchTreeTest, Contains_EmptyTree) {
  BinarySearchTree<int> bst;
  EXPECT_FALSE(bst.contains(10));
}

TEST(BinarySearchTreeTest, Contains_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  EXPECT_TRUE(bst.contains(10));
}

TEST(BinarySearchTreeTest, LowerBound_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.lower_bound(10);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 10);
}

TEST(BinarySearchTreeTest, LowerBound_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.lower_bound(15);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 20); // The lower bound for 15 in this tree is 20
}

TEST(BinarySearchTreeTest, LowerBound_EmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.lower_bound(10);
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, LowerBound_LargerThanAll) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.lower_bound(25);
  EXPECT_EQ(it.get_node(), nullptr); // No element is greater or equal to 25
}

TEST(BinarySearchTreeTest, UpperBound_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.upper_bound(10);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 20); // The upper bound for 10 in this tree is 20
}

TEST(BinarySearchTreeTest, UpperBound_NonExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.upper_bound(15);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 20); // The upper bound for 15 in this tree is 20
}

TEST(BinarySearchTreeTest, UpperBound_EmptyTree) {
  BinarySearchTree<int> bst;
  auto it = bst.upper_bound(10);
  EXPECT_EQ(it.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, UpperBound_SmallerThanAll) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto it = bst.upper_bound(3);
  EXPECT_NE(it.get_node(), nullptr);
  EXPECT_EQ(*it, 5); // The upper bound for 3 in this tree is 5
}

TEST(BinarySearchTreeTest, EqualRange_ExistingElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(20);
  auto range = bst.equal_range(10);
  EXPECT_NE(range.first.get_node(), nullptr);
  EXPECT_NE(range.second.get_node(), nullptr);
  EXPECT_EQ(*range.first, 10);
  EXPECT_EQ(*range.second, 20); // Assuming the next element after 10 is 20.
}

TEST(BinarySearchTreeTest, EqualRange_EmptyTree) {
  BinarySearchTree<int> bst;
  auto range = bst.equal_range(10);
  EXPECT_EQ(range.first.get_node(), nullptr);
  EXPECT_EQ(range.second.get_node(), nullptr);
}

TEST(BinarySearchTreeTest, EqualRange_SingleElement) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto range = bst.equal_range(10);
  EXPECT_NE(range.first.get_node(), nullptr);
  EXPECT_EQ(range.second.get_node(), nullptr);
  EXPECT_EQ(*range.first, 10);
}

TEST(BinarySearchTreeTest, MaxSize) {
  BinarySearchTree<int> bst;
  EXPECT_GT(bst.max_size(), 0);
}

TEST(BinarySearchTreeTest, GetAllocator) {
  BinarySearchTree<int> bst;
  auto allocator = bst.get_allocator();
  auto ptr = allocator.allocate(1); // Выделяем память для 1го инта
  ASSERT_NE(ptr, nullptr);
  allocator.construct(ptr, 42);
  EXPECT_EQ(*ptr, 42);
  allocator.destroy(ptr);
  allocator.deallocate(ptr, 1);
}