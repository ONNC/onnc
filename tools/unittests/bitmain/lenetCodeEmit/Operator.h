#include <vector>
#include <string>

namespace onnc {
namespace tensor {

// Outputs(OValue1, OValue2, ...) Operator1 (Inputs(IValue1, IValue2, ...))
//            ^                   Node (maybe Operator: Node)
//            |                   Layer
//            | value             User
//            |
//            +----\
//              OValue1Use1 <--------+
//                  | user           | one of "uses"
//                  v                |
// Outputs(...) Operator2 (Inputs(OValue1, ...))
//
//
//
//

// XXX: Resource owner!?

// XXX: StringRef
// class StringRef;
typedef ::std::string StringRef;
class Operator;
class Use;
class Define;

class Value {
public:
  StringRef getName();

  // TODO: Program input?
  Define *getDefine();

  unsigned getDefineNo();

  // TODO: Iterator
  std::vector<Use> *getUses();

  void replaceAllUsesWith(Value *v);

private:
  Define *define;
  unsigned int defineNo;
  std::vector<Use> *uses;
};

class Define {
public:
  Define(StringRef p_name) : name(p_name) {}

  // TODO: Operator Type
  StringRef getName();

private:
  StringRef name;
};

class Use {
public:
  Value *get();
  Operator *getUser();
  unsigned getOperandNo() const;

private:
  Operator *user;
  unsigned int operand_num;
  Value *value;
};

class Operator : public Define {
public:
  Operator(StringRef p_name) : Define(p_name) {}

  Value *getInput(unsigned index);

  Value *getOutput(unsigned index);

private:
  ::std::vector<Value *> inputs;
  ::std::vector<Value *> outputs;
};

template <typename T> class Tensor : public Value {};

// XXX: AttrType
namespace AttrType {
typedef int64_t INT;
typedef float FLOAT;
typedef ::std::string STRING;
class TENSOR {};
class GRAPH {};
typedef ::std::vector<INT> INTS;
typedef ::std::vector<FLOAT> FLOATS;
typedef ::std::vector<STRING> STRINGS;
}

} // namespace tensor
} // namespace onnc
