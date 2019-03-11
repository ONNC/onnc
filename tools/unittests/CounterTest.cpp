//===- CounterTest.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringRef.h>
#include <onnc/Analysis/Counter.h>
#include <skypat/skypat.h>

#include <algorithm>
#include <sstream>
#include <string>

using namespace onnc;
using namespace onnc::json;

std::string getNewCounterName()
{
  static unsigned serial_number = 0u;
  std::ostringstream oss;
  oss << "counter#" << serial_number++;
  return oss.str();
}

//===----------------------------------------------------------------------===//
// CounterTest
//===----------------------------------------------------------------------===//
SKYPAT_F(CounterTest, ctor)
{
  {
    const auto name = getNewCounterName();
    const Counter counter{name, ""};
    EXPECT_EQ(counter.name() , name);
    EXPECT_EQ(counter.desc() , ""  );
    EXPECT_EQ(counter.value(), 0   );
  }
  {
    const auto name = getNewCounterName();
    const Counter counter{name, "", 99};
    EXPECT_EQ(counter.name() , name);
    EXPECT_EQ(counter.desc() , ""  );
    EXPECT_EQ(counter.value(), 99  );
  }
  {
    const auto name = getNewCounterName();
    const Counter counter{name};
    EXPECT_EQ(counter.name() , name);
    EXPECT_EQ(counter.desc() , name);
    EXPECT_EQ(counter.value(), 0   );
  }
  {
    const auto name = getNewCounterName();
    const Counter counter{name, 100};
    EXPECT_EQ(counter.name() , name);
    EXPECT_EQ(counter.desc() , name);
    EXPECT_EQ(counter.value(), 100 );
  }
  {
    // create multiple counter instance with same name
    const auto name = getNewCounterName();
    Counter counter{name, 100};

    Counter counter2{name};
    EXPECT_EQ(counter2.name() , counter.name() );
    EXPECT_EQ(counter2.desc() , counter.desc() );
    EXPECT_EQ(counter2.value(), counter.value());

    ++counter;
    EXPECT_EQ(counter2.value(), 101);

    ++counter2;
    EXPECT_EQ(counter.value(), 102);
  }
}

SKYPAT_F(CounterTest, setter)
{
  {
    const auto name = getNewCounterName();
    Counter counter{name};
    EXPECT_EQ(counter.desc(), name);

    counter.desc("new desc");
    EXPECT_EQ(counter.desc() , "new desc");
  }
}

SKYPAT_F(CounterTest, conversion)
{
  {
    const auto name = getNewCounterName();
    const Counter counter{name};
    EXPECT_EQ(static_cast<Counter::value_type>(counter), counter.value());
  }
}

SKYPAT_F(CounterTest, increase)
{
  {
    const auto name = getNewCounterName();
    Counter counter{name};

    EXPECT_EQ(counter.value(), 0);
    ++counter;
    EXPECT_EQ(counter.value(), 1);

    counter++;
    EXPECT_EQ(counter.value(), 2);

    ++counter++;
    EXPECT_EQ(counter.value(), 4);

    counter += 10;
    EXPECT_EQ(counter.value(), 14);
  }
}

SKYPAT_F(CounterTest, decrease)
{
  {
    const auto name = getNewCounterName();
    Counter counter{name, 20};

    EXPECT_EQ(counter.value(), 20);
    --counter;
    EXPECT_EQ(counter.value(), 19);

    counter--;
    EXPECT_EQ(counter.value(), 18);

    --counter--;
    EXPECT_EQ(counter.value(), 16);

    counter -= 10;
    EXPECT_EQ(counter.value(), 6);
  }
}

SKYPAT_F(CounterTest, copy)
{
  {
    const auto name = getNewCounterName();
    Counter counter{name};

    EXPECT_EQ(counter.value(), 0);
    ++counter;
    EXPECT_EQ(counter.value(), 1);

    Counter counter2{counter};
    EXPECT_EQ(counter2.name() , name);
    EXPECT_EQ(counter2.value(), 1   );

    ++counter2;
    EXPECT_EQ(counter.value() , 2);
    EXPECT_EQ(counter2.value(), 2);
  }
}

SKYPAT_F(CounterTest, assignment)
{
  {
    const auto name = getNewCounterName();
    Counter counter{name, 99};

    EXPECT_EQ(counter.value(), 99);

    const auto name2 = getNewCounterName();
    Counter counter2{name2};

    EXPECT_EQ(counter2.value(), 0);
    ++counter2;
    EXPECT_EQ(counter2.value(), 1);

    counter2 = counter;
    EXPECT_EQ(counter.value() , 99);
    EXPECT_EQ(counter2.value(), 99);

    const auto name3 = getNewCounterName();
    Counter counter3{name3};

    EXPECT_EQ(counter3.value(), 0);
    counter3 = counter++;
    EXPECT_EQ(counter3.value(), 100);
  }
}

SKYPAT_F(CounterTest, isAllowPrint)
{
  const auto name = getNewCounterName();

  EXPECT_EQ((Counter{name, 0, false}.isAllowPrint()), false);

  EXPECT_EQ((Counter{name, 0, true}.isAllowPrint()), false);
}

SKYPAT_F(CounterTest, print)
{
  const auto name = getNewCounterName();

  const auto getNumOfNewLines = []() {
    std::ostringstream oss;
    global::stats().print(oss, "\n");

    const auto output = oss.str();
    return std::count(begin(output), end(output), '\n');
  };

  const auto beforeCreateNotAllowPrint = getNumOfNewLines();

  Counter{getNewCounterName(), 0, false};
  const auto afterCreateNotAllowPrint = getNumOfNewLines();
  EXPECT_EQ(beforeCreateNotAllowPrint, afterCreateNotAllowPrint);

  Counter{getNewCounterName(), 0, true};
  const auto afterCreateAllowPrint = getNumOfNewLines();
  EXPECT_EQ(beforeCreateNotAllowPrint + 1, afterCreateAllowPrint);
}

SKYPAT_F(CounterTest, reset)
{
  const auto name = getNewCounterName();
  Counter counter{name, 99};
  EXPECT_EQ(counter, 99);

  ++counter;
  EXPECT_EQ(counter, 100);

  counter.reset();
  EXPECT_EQ(counter, 99);
}

SKYPAT_F(CounterTest, stream)
{
  {
    const auto name = getNewCounterName();
    const Counter counter{name, 404};
    std::ostringstream oss;
    oss << counter;

    EXPECT_EQ(oss.str(), "404");
  }
}

SKYPAT_F(CounterTest, view)
{
  const auto targetCounterName = getNewCounterName();

  // counter which has targetCounterName should not exist
  for (auto counter : global::stats() | view::counter) {
    EXPECT_FALSE(counter.name() == targetCounterName);
  }

  // there should be only one counter instance in global::stats()
  Counter counter{targetCounterName};
  ++counter;
  EXPECT_EQ(counter.value(), 1);

  unsigned meetTargetCount = 0;
  for (auto counter : global::stats() | view::counter) {
    if (counter.name() == targetCounterName) {
      EXPECT_EQ(counter.value(), 1);
      ++meetTargetCount;
    }
  }
  EXPECT_EQ(meetTargetCount, 1);

  // updates through range-based for should affect all the created instances
  for (auto counter : global::stats() | view::counter) {
    if (counter.name() == targetCounterName) {
      ++counter;
    }
  }
  EXPECT_EQ(counter.value(), 2);
}
