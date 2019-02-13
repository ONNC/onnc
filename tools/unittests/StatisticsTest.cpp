//===- StatisticsTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Analysis/Statistics.h>
#include <onnc/JSON/Reader.h>
#include <onnc/Support/FileSystem.h>
#include <onnc/ADT/Rope.h>
#include <cstring>

using namespace onnc;


class Tester
{
public:
  Tester()
    : m_Source(TOPDIR), m_Target("/tmp/target.json") {
    m_Source.append("tools/unittests/data/empty.json");
    copy_file(source(), target(), CopyOptions::kOverwriteIfExists);
  }

  Tester(const Path& pSource, const Path& pTarget)
    : m_Source(pSource), m_Target(pTarget) {
    copy_file(source(), target(), CopyOptions::kOverwriteIfExists);
  }

  ~Tester() {
    remove(m_Target);
  }

  const Path& source() const { return m_Source; }

  const Path& target() const { return m_Target; }

private:
  Path m_Source;
  Path m_Target;
};


//===----------------------------------------------------------------------===//
// Statistics Test
//===----------------------------------------------------------------------===//
SKYPAT_F(StatisticsTest, invalid_statistics)
{
  Statistics statistics1(0);
  EXPECT_FALSE(statistics1.isValid());

  StringRef str2;
  Statistics statistics2(str2);
  EXPECT_FALSE(statistics2.isValid());

  StringRef str3("{");
  Statistics statistics3(str3);
  EXPECT_FALSE(statistics3.isValid());
}

SKYPAT_F(StatisticsTest, construct_from_str)
{
  StringRef str1("{}");
  Statistics statistics1(str1);
  EXPECT_TRUE(statistics1.isValid());
  EXPECT_EQ(statistics1.accessMode(), Statistics::kReadOnly);

  StringList group_list1 = statistics1.groupList();
  EXPECT_EQ(group_list1.size(), 0);

  std::string str2("{\"0\":0}");
  Statistics statistics2(str2);
  EXPECT_TRUE(statistics2.isValid());
  EXPECT_EQ(statistics2.accessMode(), Statistics::kReadOnly);

  StringList group_list2 = statistics2.groupList();
  EXPECT_EQ(group_list2.size(), 0);

  const char *str3 = "{\"0\":{}}";
  Statistics statistics3(str3);
  EXPECT_TRUE(statistics3.isValid());
  EXPECT_EQ(statistics3.accessMode(), Statistics::kReadOnly);

  StringList group_list3 = statistics3.groupList();
  EXPECT_EQ(group_list3.size(), 1);
}

SKYPAT_F(StatisticsTest, read_only)
{
  Tester tester;
  {
    Statistics statistics(tester.target(), Statistics::kReadOnly);
    statistics.addGroup("hello").writeEntry("world", "!");
  }

  {
    Statistics statistics(tester.target(), Statistics::kReadOnly);
    StringRef value = statistics.group("hello").readEntry("world", "no value");
    EXPECT_EQ(value, "no value");
  }
}

SKYPAT_F(StatisticsTest, merge_null)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/empty.json");

  Statistics statistics(path, Statistics::kReadOnly);
  EXPECT_TRUE(json::Group::Null().isNull());
  statistics.merge("null", json::Group::Null());

  EXPECT_TRUE(statistics.hasGroup("null"));
  EXPECT_TRUE(statistics.group("null").isEmpty());
  EXPECT_FALSE(statistics.group("null").isNull());
}

SKYPAT_F(StatisticsTest, clear_group)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/statistics.json");
  Statistics statistics(path, Statistics::kReadOnly);

  ASSERT_EQ(statistics.group("group1").readEntry("gkey1", 0), 10);
  statistics.group("group1").clear();
  ASSERT_TRUE(statistics.group("group1").isEmpty());
}

SKYPAT_F(StatisticsTest, read_str)
{
  std::string str((
    Rope("{\n") +
    Rope("  \"hello\" : \"world\",\n") +
    Rope("  \"statistics1\" : {\n") +
    Rope("    \"hello\": 123,\n") +
    Rope("    \"world\": \"abc\"\n") +
    Rope("  }\n") +
    Rope("}")).str());

  Statistics statistics1(str);
  Statistics statistics2(0);
  statistics2.read(str);

  StringList group_list1 = statistics1.groupList();
  StringList group_list2 = statistics2.groupList();
  ASSERT_EQ(group_list1.size(), group_list2.size())
    << "group_list1 size: " << group_list1.size()
    << "\ngroup_list2 size: " << group_list2.size();

  StringList::iterator iter1 = group_list1.begin();
  StringList::iterator iter2 = group_list2.begin();
  while ((iter1 != group_list1.end()) && (iter2 != group_list2.end())) {
    ASSERT_TRUE(0 == ::strcmp(iter1->data(), iter2->data()));
    ++iter1;
    ++iter2;
  }
}

SKYPAT_F(StatisticsTest, read_file)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/statistics.json");
  Statistics statistics(path, Statistics::kReadOnly);

  ASSERT_TRUE(statistics.hasGroup("group1"));
  ASSERT_TRUE(statistics.hasGroup("group2"));
  ASSERT_FALSE(statistics.hasGroup("no such group"));

  StringList group_list = statistics.groupList();
  EXPECT_EQ(group_list.size(), 3);
  StringList::iterator str = group_list.begin();
  ASSERT_TRUE(0 == ::strcmp(str->data(), "group1"));
  ++str;
  ASSERT_TRUE(0 == ::strcmp(str->data(), "group2"));
  ++str;
  ASSERT_TRUE(0 == ::strcmp(str->data(), "group3"));
  ++str;
  EXPECT_TRUE(str == group_list.end());

  EXPECT_TRUE(statistics.deleteGroup("group1"));
  EXPECT_FALSE(statistics.hasGroup("group1"));
  group_list = statistics.groupList();
  EXPECT_EQ(group_list.size(), 2);

  EXPECT_TRUE(statistics.deleteGroup("group2"));
  EXPECT_FALSE(statistics.hasGroup("group2"));
  group_list = statistics.groupList();
  EXPECT_EQ(group_list.size(), 1);

  statistics.deleteGroup("group3");
  EXPECT_FALSE(statistics.hasGroup("group3"));
  group_list = statistics.groupList();
  EXPECT_TRUE(group_list.empty());

  ASSERT_FALSE(statistics.deleteGroup("no such group"));
}

SKYPAT_F(StatisticsTest, read_entry)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/statistics.json");
  Statistics statistics(path, Statistics::kReadOnly);

  int value = statistics.group("group1").readEntry("gkey1", 33);
  ASSERT_EQ(value, 10);

  value = statistics.group("no such group").readEntry("key1", 44);
  ASSERT_EQ(value, 44);
}

SKYPAT_F(StatisticsTest, read_entry_wrong_type)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/statistics.json");
  Statistics statistics(path, Statistics::kReadOnly);

  StringRef value1 = statistics.group("group1").readEntry("gkey1", "no value");
  ASSERT_EQ(value1, "no value");

  int value2 = statistics.group("group1").readEntry("gkey2", 0);
  ASSERT_EQ(value2, 0);
}

SKYPAT_F(StatisticsTest, read_entry_group_iterator)
{
  Statistics statistics("{ \"group1\": { \"value\": 1 }, \"group2\": { \"value\": 2 } }");
  json::Group::GroupIterator group = statistics.top().gBegin();
  json::Group::GroupIterator end = statistics.top().gEnd();

  ASSERT_FALSE(group == end);
  EXPECT_EQ(group.name(), "group1");
  EXPECT_EQ(group.group().readEntry("value", 0), 1);
  group.next();
  ASSERT_FALSE(group == end);
  EXPECT_EQ(group.name(), "group2");
  EXPECT_EQ(group.group().readEntry("value", 0), 2);
  group.next();
  ASSERT_TRUE(group == end);
}

SKYPAT_F(StatisticsTest, read_entry_value_iterator)
{
  Statistics statistics("{ \"group\": [{ \"value\": 1 }, { \"value\": 2 }] }");
  json::Group::ValueIterator group = statistics.top().vBegin("group");
  json::Group::ValueIterator end = statistics.top().vEnd("group");

  ASSERT_FALSE(group == end);
  EXPECT_EQ(group.group().readEntry("value", 0), 1);
  group.next();
  ASSERT_FALSE(group == end);
  EXPECT_EQ(group.group().readEntry("value", 0), 2);
  group.next();
  ASSERT_TRUE(group == end);
}

SKYPAT_F(StatisticsTest, read_only_cannot_write)
{
  Tester tester;

  // try to write to a read-only file
  ASSERT_TRUE(chmod(tester.target(), 0400));

  // statistics can not sync back the result to target file.
  Statistics statistics(tester.target(), Statistics::kReadWrite);
  EXPECT_TRUE(statistics.isValid()); //< able to read

  json::Group group = statistics.addGroup("group");
  group.writeEntry("test", "value");
  StringRef value = statistics.group("group").readEntry("test", "no value");
  EXPECT_TRUE_MSG(value.equals("value"), value);

  // Since the file is readonly, synchronization should be fall.
  EXPECT_FALSE(statistics.sync());

  Statistics statistics2(tester.target());
  StringRef value2 = statistics2.group("group").readEntry("test", "no value");
  EXPECT_TRUE_MSG(value2.equals("no value"), value2);
}

SKYPAT_F(StatisticsTest, write_object)
{
  Tester tester;

  // try to write to a readable and writable file
  ASSERT_TRUE(chmod(tester.target(), 0644));

  Statistics statistics(tester.target(), Statistics::kReadWrite);
  statistics.addGroup("group").writeEntry("test", "value");

  EXPECT_TRUE(statistics.sync());

  Statistics statistics2(tester.target());
  StringRef value = statistics2.group("group").readEntry("test", "no");
  EXPECT_TRUE_MSG(value.equals("value"), value);
}

SKYPAT_F(StatisticsTest, multiple_writes)
{
  Tester tester;

  // try to write to a readable and writable file
  ASSERT_TRUE(chmod(tester.target(), 0644));

  Statistics statistics(tester.target(), Statistics::kReadWrite);
  json::Group group = statistics.addGroup("group");

  // write identical key twice.
  group.writeEntry("test", "value1");
  group.writeEntry("test", "value2");

  // read from the same group object
  StringRef content = group.readEntry("test", "no");
  EXPECT_TRUE_MSG(content.equals("value2"), content);

  EXPECT_TRUE(statistics.sync());

  // read from outside statistics.
  Statistics statistics2(tester.target());
  StringRef value = statistics2.group("group").readEntry("test", "no");
  EXPECT_TRUE_MSG(value.equals("value2"), value);
}

SKYPAT_F(StatisticsTest, statistics_impl_read)
{
  Path path(TOPDIR);
  path.append("/tools/unittests/data/statistics.json");
  json::Reader reader;
  json::Value value;
  reader.parse(path, value); 

  json::Group impl(value.asObject());

  StringRef value13 = impl.group("group1").readEntry("gkey3", "no value");
  EXPECT_TRUE(value13.equals("value13"));

  StringList def;
  StringList list = impl.group("group1").readEntry("gkey4", def);
  EXPECT_EQ(list.size(), 3);
  EXPECT_TRUE(list.front().equals("1"));
  EXPECT_TRUE(list.back().equals("3"));

  StringRef value23 = impl.group("group2").readEntry("g2key3", std::string("no value"));
  EXPECT_TRUE(value23.equals("value23"));

  std::vector<long> int_list = impl.group("group3").readEntry("int_list", std::vector<long>());
  EXPECT_EQ(int_list.size(), 3);
  EXPECT_EQ(int_list.front(), 1);
  EXPECT_EQ(int_list.back(), 3);
}

SKYPAT_F(StatisticsTest, statistics_impl_write)
{
  json::Object obj;
  json::Group impl(obj);

  impl.writeEntry("key1", 1);
  impl.writeEntry("key2", 2.2);
  impl.writeEntry("key3", true);
  impl.writeEntry("key4", "4strings");

  impl.addGroup("grp1");
  impl.group("grp1").writeEntry("kk", "test");

  StringRef kk = impl.group("grp1").readEntry("kk", "no test");
  EXPECT_TRUE(kk.equals("test"));

  int       value1 = impl.readEntry("key1", 19);
  double    value2 = impl.readEntry("key2", 9.9);
  bool      value3 = impl.readEntry("key3", false);
  StringRef value4 = impl.readEntry("key4", "not exist");

  EXPECT_EQ(value1, 1);
  EXPECT_EQ(value2, 2.2);
  EXPECT_TRUE(value3);
  EXPECT_TRUE(value4.equals("4strings"));

  std::vector<int> array;
  array.push_back(1);
  array.push_back(2);
  array.push_back(3);
  array.push_back(4);
  array.push_back(5);

  impl.writeEntry("key5", array.begin(), array.end());

  std::vector<int> def;
  std::vector<int> ret;
  ret = impl.readEntry("key5", def);

  EXPECT_EQ(ret.size(), 5);
  EXPECT_EQ(ret[0], 1);
  EXPECT_EQ(ret[1], 2);
  EXPECT_EQ(ret[2], 3);
  EXPECT_EQ(ret[3], 4);
  EXPECT_EQ(ret[4], 5);
}

SKYPAT_F(StatisticsTest, merge_object)
{
  std::string str1((
    Rope("{\n") +
    Rope("  \"hello\" : \"world\",\n") +
    Rope("  \"statistics1\" : {\n") +
    Rope("    \"hello\": 123,\n") +
    Rope("    \"world\": \"abc\"\n") +
    Rope("  }\n") +
    Rope("}")).str());
  Statistics statistics1(str1);

  Path path2(TOPDIR);
  path2.append("/tools/unittests/data/statistics.json");
  Statistics statistics2(path2, Statistics::kReadOnly);

  statistics1.merge("statistics2", statistics2.group("group1"));

  int value1 = statistics1.group("statistics1").readEntry("hello", 0);
  ASSERT_EQ(value1, 123);

  double value2 = statistics1.group("statistics2").readEntry("gkey2", 2.2);
  ASSERT_EQ(value2, 1.1);
}

SKYPAT_F(StatisticsTest, constructor_for_json)
{
  std::string s1((
    Rope("{\n") +
    Rope("  \"option\" : \"ffp-contract\",\n") +
    Rope("  \"occurrence\" : 0.5,\n") +
    Rope("  \"probability\" : {\n") +
    Rope("     \"fast\" : 0.6,\n") +
    Rope("     \"on\" : 0.2,\n") +
    Rope("     \"off\" : 0.2\n") +
    Rope("   },\n") +
    Rope("  \"otherGroup\" : {\n") +
    Rope("     \"otherEntry1\" : 0.01,\n") +
    Rope("     \"otherEntry2\" : 0.02\n") +
    Rope("    },\n") +
    Rope("  \"nestedGroup\" : {\n") +
    Rope("     \"innerGroup\" : {\n") +
    Rope("        \"innerEntry1\" : 0.001,\n") +
    Rope("        \"innerEntry2\" : 0.002\n") +
    Rope("       },\n") +
    Rope("     \"otherEntry1\" : 0.03\n") +
    Rope("    }\n") +
    Rope("}")).str());

  Statistics statistics1(s1);
  ASSERT_TRUE(statistics1.hasGroup("probability"));
  ASSERT_FALSE(statistics1.hasGroup("other Entry1"));

  double value = statistics1.group("probability").readEntry("fast", -1.0);
  EXPECT_EQ(value, 0.6);
  value = statistics1.group("probability").readEntry("on", -1.0);
  EXPECT_EQ(value, 0.2);
  value = statistics1.group("probability").readEntry("off", -1.0);
  EXPECT_EQ(value, 0.2);

  ASSERT_TRUE(statistics1.hasGroup("otherGroup"));
  value = statistics1.group("otherGroup").readEntry("otherEntry1", -1.0);
  EXPECT_EQ(value, 0.01);
  value = statistics1.group("otherGroup").readEntry("otherEntry2", -1.0);
  EXPECT_EQ(value, 0.02);

  ASSERT_TRUE(statistics1.hasGroup("nestedGroup"));
  ASSERT_TRUE(statistics1.group("nestedGroup").hasGroup("innerGroup"));
  value = statistics1.group("nestedGroup").group("innerGroup").
                                       readEntry("innerEntry1", -1.0);
  EXPECT_EQ(value, 0.001);
  value = statistics1.group("nestedGroup").group("innerGroup").
                                       readEntry("innerEntry2", -1.0);
  EXPECT_EQ(value, 0.002);
}
