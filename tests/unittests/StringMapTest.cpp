//===- StringMapTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringMap.h>
#include <skypat/skypat.h>

using namespace skypat;
using namespace onnc;

namespace {

struct PtrHash
{
  size_t operator()(const int* pKey) const
  {
    return (unsigned((uintptr_t)pKey) >> 4) ^
           (unsigned((uintptr_t)pKey) >> 9);
  }
};

struct IntHash
{
  size_t operator()(int pKey) const { return pKey; }
};

struct FixHash
{
  size_t operator()(int pKey) const { return 10; }
};

} // anonymous namespace

//===----------------------------------------------------------------------===//
// Testcases
//===----------------------------------------------------------------------===//
SKYPAT_F(StringMapTest, insert)
{
  StringMap<int> m;
  ASSERT_EQ(m.numOfEntries(), 0);
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  ASSERT_FALSE(exist);
  ASSERT_EQ(m.numOfEntries(), 1);

  ASSERT_EQ(m.insert("abc", exist)->value(), 123);
  ASSERT_TRUE(exist);
}

SKYPAT_F(StringMapTest, find)
{
  StringMap<int> m;
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  {
    auto it = m.find("abc");
    ASSERT_TRUE(it != m.end());
    ASSERT_EQ(it->value(), 123);
  }
  {
    auto it = m.find("def");
    ASSERT_TRUE(it == m.end());
  }
}

SKYPAT_F(StringMapTest, count)
{
  StringMap<int> m;
  bool exist = true;
  m.insert("abc", exist)->value() = 123;
  m.count("abc");
  ASSERT_EQ(m.count("abc"), 1);
  ASSERT_EQ(m.count("def"), 0);
}

SKYPAT_F(HashTableTest, constructor_test)
{
  StringMap<int> hash(10);

  EXPECT_GE(hash.numOfBuckets(), 10);
  EXPECT_TRUE(hash.empty());
  EXPECT_EQ(0, hash.numOfEntries());
  EXPECT_EQ(0, hash.numOfTombstones());
}

SKYPAT_F(HashTableTest, constructor_test2)
{
  typedef HashTable<int*, int, PtrHash> HashTable;
  HashTable table(16);
  EXPECT_EQ(table.numOfBuckets(), 17); //< I knew the magic. 
  EXPECT_TRUE(table.empty());
  EXPECT_EQ(table.numOfEntries(), 0);
}

SKYPAT_F(HashTableTest, ptr_entry)
{
  HashTable<int*, int, PtrHash>* hash = new HashTable<int*, int, PtrHash>(0);

  bool exist;
  HashTable<int*, int, PtrHash>::entry_type* entry = 0;

  int A = 1;
  int* pA = &A; 
  entry = hash->insert(pA, exist);

  EXPECT_FALSE(hash->empty());

  HashTable<int*, int, PtrHash>::iterator iter;
  iter = hash->find(NULL);
  EXPECT_TRUE(iter == hash->end());
  delete hash;
}

SKYPAT_F(HashTableTest, allocation)
{
  typedef StringMap<int> HashTable;
  HashTable* table = new HashTable(22);

  bool exist;
  const char* key = "key";
  HashTable::entry_type* val = table->insert(key, exist);
  val->setValue(999);

  EXPECT_FALSE(table->empty());
  EXPECT_FALSE(exist);
  EXPECT_TRUE(NULL != val);

  HashTable::iterator entry = table->find(key);
  EXPECT_EQ(entry->value(), 999);
  delete table;
}

SKYPAT_F(HashTableTest, alloc100IntTest)
{
  typedef HashTable<int, int, IntHash> Hash;

  Hash* table = new Hash(22);

  bool exist;
  Hash::entry_type* entry = 0;
  for (int key = 0; key <100; ++key) {
    entry = table->insert(key, exist);
    EXPECT_FALSE(table->empty());
    EXPECT_FALSE(exist);
    EXPECT_TRUE(NULL != entry);
    EXPECT_EQ(entry->key(), key);
    entry->setValue(key+10);
  }

  EXPECT_FALSE(table->empty());
  EXPECT_EQ(table->numOfEntries(), 100);
  EXPECT_EQ(table->numOfBuckets(), 197);
  delete table;
}

SKYPAT_F(HashTableTest, erase100IntTest)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key <100; ++key)
    entry = table->insert(key, exist);

  EXPECT_FALSE(table->empty());

  int count;
  HashTableType::iterator iter;
  for (unsigned int key = 0; key <100; ++key) {
    count = table->erase(key);
    EXPECT_EQ(count, 1);
    iter = table->find(key);
    EXPECT_TRUE(iter == table->end());
  }

  EXPECT_TRUE(table->empty());
  delete table;
}

SKYPAT_F(HashTableTest, clear_test)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType(22);

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key <100; ++key) {
    entry = table->insert(key, exist);
  }

  table->clear();

  HashTableType::iterator iter;
  for (unsigned int key = 0; key < 100; ++key) {
    iter = table->find(key);
    EXPECT_TRUE(table->end() == iter);
  }

  EXPECT_TRUE(table->empty());
  delete table;
}

SKYPAT_F(HashTableTest, tombstone_test)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key <100; ++key) {
    entry = table->insert(key, exist);
  }
  EXPECT_FALSE(table->empty());

  int count;
  HashTableType::iterator iter;
  for (unsigned int key = 0; key <20; ++key) {
    count = table->erase(key);
    EXPECT_EQ(1, count);
    iter = table->find(key);
    EXPECT_TRUE(table->end() == iter);
  }
  EXPECT_EQ(table->numOfEntries(), 80);

  for (unsigned int key=20; key < 100; ++key) {
    iter = table->find(key);
    EXPECT_TRUE(iter != table->end());
  }

  for (unsigned int key = 0; key < 20; ++key) {
    entry = table->insert(key, exist);
  }
  EXPECT_EQ(table->numOfEntries(), 100);
  EXPECT_EQ(table->numOfBuckets(), 197);

  delete table;
}

SKYPAT_F(HashTableTest, rehash_test)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key < 400000; ++key) {
    entry = table->insert(key, exist);
    entry->setValue(key+10);
  }

  HashTableType::iterator iter;
  for (int key = 0; key < 400000; ++key) {
    iter = table->find(key);
    EXPECT_EQ((key+10), iter->value());
  }

  delete table;
}

SKYPAT_F(HashTableTest, bucket_iterator_test)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key < 400000; ++key) {
    entry = table->insert(key, exist);
    entry->setValue(key+10);
  }

  HashTableType::iterator iter, iEnd = table->end();
  int counter = 0;
  for (iter = table->begin(); iter != iEnd; ++iter) {
    EXPECT_EQ(iter->key()+10, iter->value());
    ++counter;
  }
  EXPECT_EQ(counter, 400000);
  delete table;
}

SKYPAT_F(HashTableTest, chain_iterator_single_test)
{
  typedef HashTable<int, int, IntHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (int key = 0; key < 16; ++key) {
    entry = table->insert(key*37, exist);
    entry->setValue(key+10);
  }
  for (int key = 0; key < 16; ++key) {
    int counter = 0;
    HashTableType::chain_iterator iter, iEnd = table->end(key*37);
    for (iter = table->begin(key*37); iter != iEnd; ++iter) {
      EXPECT_EQ(key+10, iter->value());
      ++counter;
    }
    EXPECT_EQ(counter, 1);
  }
  delete table;
}

SKYPAT_F(HashTableTest, chain_iterator_list_test)
{
  typedef HashTable<int, int, FixHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key < 16; ++key) {
    entry = table->insert(key, exist);
    ASSERT_FALSE(exist);
    entry->setValue(key);
  }
  ASSERT_EQ(table->numOfEntries(), 16);
  ASSERT_EQ(table->numOfBuckets(), 37);

  unsigned int key = 0;
  int count = 0;
  HashTableType::chain_iterator iter, iEnd = table->end(key);
  for (iter = table->begin(key); iter != iEnd; ++iter) {
    count++;
  }
  ASSERT_EQ(count, 16);
  delete table;
}

SKYPAT_F(StringHashTableTest, string_hash_table)
{
  typedef StringHashTable<int> HashTable;
  HashTable* table = new HashTable(22);

  bool exist;
  const char* key = "key";
  HashTable::entry_type* val = table->insert(key, exist);
  val->setValue(999);

  EXPECT_FALSE(table->empty());
  EXPECT_FALSE(exist);
  EXPECT_TRUE(NULL != val);

  HashTable::iterator entry = table->find(key);
  EXPECT_EQ(entry->value(), 999);

  delete table;
}

SKYPAT_F(HashTableTest, copy_constructor)
{
  typedef HashTable<int, int, FixHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key < 16; ++key) {
    entry = table->insert(key, exist);
    ASSERT_FALSE(exist);
    entry->setValue(key);
  }
  ASSERT_EQ(table->numOfEntries(), 16);
  ASSERT_EQ(table->numOfBuckets(), 37);

  // copy constructor
  HashTableType table2(*table);
  ASSERT_EQ(table2.numOfEntries(), 16);
  ASSERT_EQ(table2.numOfBuckets(), 37);

  unsigned int count = 0;
  HashTableType::iterator iter, iEnd = table2.end();
  for (iter = table2.begin(); iter != iEnd; ++iter) {
    count++;
  }
  ASSERT_EQ(count, 16);

  HashTableType::iterator en2 = table2.find(2);
  ASSERT_EQ(en2->value(), 2);

  delete table;
}

SKYPAT_F(HashTableTest, assignment)
{
  typedef HashTable<int, int, FixHash> HashTableType;
  HashTableType *table = new HashTableType();

  bool exist;
  HashTableType::entry_type* entry = 0;
  for (unsigned int key = 0; key < 16; ++key) {
    entry = table->insert(key, exist);
    ASSERT_FALSE(exist);
    entry->setValue(key);
  }
  ASSERT_EQ(table->numOfEntries(), 16);
  ASSERT_EQ(table->numOfBuckets(), 37);

  // copy constructor
  HashTableType table2;
  table2 = *table;

  ASSERT_EQ(table2.numOfEntries(), 16);
  ASSERT_EQ(table2.numOfBuckets(), 37);

  unsigned int count = 0;
  HashTableType::iterator iter, iEnd = table2.end();
  for (iter = table2.begin(); iter != iEnd; ++iter) {
    count++;
  }
  ASSERT_EQ(count, 16);

  HashTableType::iterator en2 = table2.find(2);
  ASSERT_EQ(en2->value(), 2);
  delete table;
}

SKYPAT_F(StringHashTableTest, copy_constructor)
{
  typedef StringHashTable<int> HashTable;
  HashTable* table = new HashTable(22);

  bool exist;
  const char* key = "key";
  HashTable::entry_type* val = table->insert(key, exist);
  val->setValue(999);

  HashTable table2(*table);

  EXPECT_FALSE(table2.empty());
  HashTable::iterator entry = table2.find(key);
  EXPECT_EQ(entry->value(), 999);

  delete table;
}
