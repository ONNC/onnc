# Statistic API

ONNC provides Statistic APIs, a set of analytic tools for user's development.

The APIs could be embedded in any parts in the ONNC project.

## Supported API Class

These classes are provided in Statistic API.
Currently ONNC only provides the `Counter` class, other classes may be delivered in the future.

| Class      | Description |
| ---------- | ----------- |
| Counter | Advanced counter |

## The `Statistic` Class

The datum manipulated by the classes under Statistic API, currently only the `Counter`, are kept by `Statistic` class.

By accessing `Statistic` object with `onnc::global::stats()`, the `print()` method passes outputs to Standard Output or Standard Error.

This is an example for outputing to different stream:

```cpp
global::stats().print(); // print to std::cout
global::stats().print(std::cerr, ","); // print to std::cerr, separated by comma
```

`Statistic` is derived from `json::Storage`, and it uses `json::Group` as the interface to manipulate low-level data.

## The `Counter` Class

The [Counter](/include/onnc/Analysis/Counter.h) class serves as a regular `int` counter in ONNC.

To use Counter, include `<onnc/Analysis/Counter.h>` header, use `onnc` namespace, and create an instance of `Counter` class.

```cpp
#include <onnc/Analysis/Counter.h>
using namespace onnc;
Counter counter {"name of counter", "description of counter", 0};
```


### Constructor

Declare a `Counter` type instance by the constructor. It takes `counter` as the example in the following sample codes.

There is only **one** instance for an unique name of a counter. In other word, all counters of the same name are pointed to the same object. The `Counter` instance can be regarded as a reference object.

```cpp
Counter counter {name, description, value, allowPrint};
```

| Parameter | Type | Optional | Default | Description |
| ----------- | ----------- |----------- |----------- |----------- |
| name | StringRef | required | |the name of the counter |
| description | StringRef | optional | same with the name | the description of the counter |
| value | int | optional | 0| counting value|
| allowPrint | bool | optional | true| allow the object printable of not |

Examples:

```cpp
// Create counters
Counter counter1{"counter#1", "description #1", 1, false};
Counter counter2{"counter#2", "description #2", 1};
Counter counter3{"counter#3", "description #3"};
Counter counter4{"counter#4", 1, false};
Counter counter5{"counter#5", 1};
Counter counter6{"counter#6"};

// Get existed counters
Counter foo{"foo", "description"};
Counter same_foo{"foo", "new description"}; // point to `foo` and ignore any parameters
```

### Operators

The `Counter` has operators for updating the value.

- `Counter++`: add one after fetch the value
- `Counter+=`: add and equal to the value given
- `++Counter`: fetch the value and add one
- `Counter--`: minus one after fetch the value
- `Counter-=`: minus and equal to the value given
- `--Counter`: fetch the value and minus one
- `<<Counter`: output the value to the stream
- `=Counter`: re-bind to another `Counter`

examples:

```cpp
Counter foo {"foo"}; // value default from 0
foo++; // value becomes 1
foo += 9; // value becomes 10

Counter bar {"bar"};
std::cout << bar.name(); // output "bar"
bar = foo;
std::cout << bar.name(); // output "foo"
```

### Methods

- `Counter.name()`: get the name in `std::String`
- `Counter.desc()`: get the description in `std::String`
- `Counter.value()`: get the value in `int`
- `Counter.reset()`: set the value to the original setting

Examples:

```cpp
Counter foo{"foo", 99}; // use 99 as default value
++foo; // value is 100
foo.reset(); // reset value backing to 99
```

### Outputs

Call the getter methods to fetch the value of `Counter`. It returns the `value` of `Counter` when the `Counter` is called directly.

Use a stream to output:

```cpp
const Counter counter{"name of counter", "description of counter", 99}; // create new counter

std::cout << counter.name(); // output "name of counter"
std::cout << counter.desc(); // output "description of counter"
std::cout << counter.value(); // output 99
std::cout << counter; // output 99
```

Use the `print` method in `Statistic` to output:

```cpp
Counter foo{"foo", 0, true}; // allowPrint == true
Counter bar{"bar", 0, false}; // allowPrint == false

global::stats().print(); // only show foo counter
```

### Inspect All Counters

There is an example to inspect all counters:

```cpp
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
