# ONNC Statistics API Design

## Infrastructure

目前 Statistics API 是建立在 *Statistics* 這個類別之上, 在全域會有一個 *Statistics* 物件用來儲存所有的資料, 使用者必須透過 _global::stats()_ 來取得這個全域物件:
```c++
// include/onnc/Analysis/GlobalStatistics.h
namespace onnc {
namespace global {

Statistics& stats();

} // namespace global
} // namespace onnc
```

*Statistics* 是 *json::Storage* 的子類別, 亦即是: 其內部的狀態都會透過 *json::Group* (JSON 物件的抽象化型別) 提供的介面來維護

## API

### Introduction

作為統計的最基本功能: 記數, 希望使用者可以簡單創立 *Counter* 計數器物件後就可以馬上使用:
```c++
// counter's name is "name of counter", counter's description is "description of counter", counter's value is 0
Counter counter{"name of counter", "description of counter", 0};
```
每個 *Counter* 物件都是作為代理人 (proxy) 而存在, 其所操作的計數器目標存放在 _global::stats()_ 裡面.  


如果想查看已建立的所有 *Counter*, 透過 *Counter View Adaptor* 就可以取得:
```c++
auto counters = global::stats() | view::counter; // view::counter is the view adaptor for Counter
for (auto counter : counters) {
  // do things with individual counter
}
```

若想查看所有的統計資料, 可以透過 *Statistics* 提供的 _print()_ 方法來將內容輸出到 *std::ostream* 物件
```c++
global::stats().print(); // default output to std::cout
global::stats().print(std::cerr); // output to std::cerr
```

### Interface Layers

以下是相關型別以及他們的角色描述:

| Class | Role  | User |
| :---: | :---- | :--: |
|Counter|作為代理人對計數器作存取|Library User|
|CounterIterator|對 json::GroupIterator 的封裝, 可用來尋訪存放在 Statistics 裡所有的 Counter 物件. CounterIterator 滿足 InputIterator 概念|Library User|
|IteratorRange|成對的 CounterIterator 封裝, 用來套接 range-based for 或 STL Algorithms|Library User|
|Statistics|所有統計資料的存放地|Developer|
|json::Group|JSON 物件的抽象化型別, 做為資料儲存的一種方案|Developer|

預期使用者會明確寫出來的型別只會有 *Counter*, 而 *CounterIterator* / *IteratorRange* 物件都是透過函式產生, 透過 Generic Programming 語言習慣來操作.

### Code Synopsis

*Counter*

```c++
// include/onnc/Analysis/Counter.h
namespace onnc {

class Counter final
{
public:
  // implementation type is int
  using value_type = int;

public:
  // constructe counter object by given name, description (optional), 
  // and default value (optional). The parameter allowPrint is a flag to decide
  // whether this counter is allowed to be printed by global::stats().print()
  Counter(StringRef pName, StringRef pDesc, value_type pValue = 0, bool allowPrint = true);
  Counter(StringRef pName, value_type pValue = 0, bool allowPrint = true);
  
  // getter/setter for the counter attributes
  StringRef name() const; // get counter's name
  StringRef desc() const; // get counter's description
  Counter& desc(StringRef pDesc); // set counter's description
  value_type value() const; // get counter's value
  bool isAllowPrint() const; // test if this counter will be print via global::stats().print()
  
  // reset counter value to the default value
  Counter& reset();

  // support implicit convert to int
  operator value_type() const;

  // support integer like operations
  Counter& operator++();
  Counter& operator++(int);
  Counter& operator--();
  Counter& operator--(int);
  Counter& operator=(value_type pNumber);
  Counter& operator+=(value_type number);
  Counter& operator-=(value_type number);                                                                                             
};

inline std::ostream& operator<<(std::ostream& stream, const Counter& counter)
{
  return stream << counter.value();
}

} // namespace onnc
```

*Counter View Adaptor*

```c++
// include/onnc/Analysis/Counter.h
namespace onnc {

namespace view {
  struct counter_view_adaptor_tag {};
  static constexpr counter_view_adaptor_tag counter;
}

IteratorRange<CounterIterator> operator|(Statistics&, view::counter_view_adaptor_tag);

} // namespace onnc
```
*Statistics*

```c++
// include/onnc/Analysis/Statistics.h
class Statistics final : public json::Storage
{
public:
  // print all statistics data, entries are separated by a separator
  std::ostream& print(std::ostream& stream = std::cout, const std::string& separator = "\n") const;
};
```

### Usage

#### Get/Create Counter

```c++
// 1. create counter
Counter counter1{"counter#1", "description of counter#2", 1, false}; // create new counter with name = "counter#1", description = "description of counter#1", default value = 1, allow print = false
Counter counter2{"counter#2", "description of counter#2", 1}; // create new counter with name = "counter#2", description = "description of counter#2", default value = 1, allow print = true (default)
Counter counter3{"counter#3", "description of counter#3"}; // create new counter with name = "counter#3", description = "description of counter#3", default value = 0 (default), allow print = true (default)
Counter counter4{"counter#4", 1, false}; // create new counter with name = "counter#4", description = "counter#4" (default), default value = 1, allow print = false
Counter counter5{"counter#5", 1}; // create new counter with name = "counter#5", description = "counter#5" (default), default value = 1, allow print = true (default)
Counter counter6{"counter#6"}; // create new counter with name = "counter#6", description = "counter#6" (default), default value = 0 (default), allow print = true (default)

// 2. get existing counter
Counter foo{"foo", "description"};
Counter same_foo{"foo", "new description"}; // get the just created counter "foo", parameters other than the name are ignored
```

#### Count Things

```c++
Counter foo{"foo"}; // counter value starts from 0

foo++; // counter value now becomes 1

foo += 9; // counter value now becomes 10
```

#### Counter Attributes

```c++
const Counter counter{"name of counter", "description of counter", 99}; // create new counter

std::cout << counter.name() << std::endl; // output "name of counter"
std::cout << counter.desc() << std::endl; // output "description of counter"
std::cout << counter.value() << std::endl; // output 99
std::cout << counter << std::endl; // output 99
```

#### Conversion To Built-in Type

```c++
const Counter foo{"foo"};

const int ci = static_cast<int>(foo); // explicit conversion to int

const auto ci2 = static_cast<Counter::value_type>(foo); // explicit conversion to Counter::value_type, more safe way
assert(ci == foo); // implicit conversion to int, then compare with the other
```

#### Re-bind Counter

```c++
Counter foo{"foo"};
Counter bar{"bar"};

std::cout << bar.name() << std::endl; // output "bar"

bar = foo;
std::cout << bar.name() << std::endl; // output "foo"
```

#### Reset Counter

```c++
Counter foo{"foo", 99}; // use 99 as default value 

++foo; // now foo's value is 100

foo.reset(); // reset foo's value back to 99
```

#### Inspect All Counters

```c++
// 1. use range-based for loop
for (auto counter : global::stats() | view::counter) { // use operator| with adaptor to create view range
  std::cout << "name: " << counter.name() << ", value: " << counter << std::endl;
}

// 2. use STL algorithm
const auto counters = global::stats() | view::counter; // type of variable 'counters' is 'const IteratorRange<CounterIterator>'
const auto numberOfLargeCounters = std::count_if(
  std::begin(counters), std::end(counters),
  [](const auto& counter) { return 10 <= counter; }
);
```

#### Display All Statistics Data

```c++
// 1. output to different stream
global::stats().print(); // print to std::cout
global::stats().print(std::cerr, ","); // print to std::cerr, separated by comma

// 2. show counters conditionally
Counter foo{"foo", 0, true}; // allowPrint == true
Counter bar{"bar", 0, false}; // allowPrint == false

global::stats().print(); // only show foo counter
```
