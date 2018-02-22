#include <list>
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
  Define* getDefine();

  unsigned getDefineNo();

  // TODO: Iterator
  std::list<Use> *getUses();

  void replaceAllUsesWith(Value *v);

 private:
  Define* define;
  std::list<Use> *uses;
};

class Define
{
public:
  // TODO: Operator Type
  StringRef getName();

private:
  StringRef name;
};

class Use {
 public:
  Value *get();
  Define* getUser();

  unsigned getOperandNo() const;

 private:
  Define *user;
  unsigned int operand_num;
  Value *value;
};

class Operator : public Define
{
public:
  Operator(StringRef p_name): name(p_name) {}

  Value *getInput(unsigned index);

  Value *getOutput(unsigned index);

 private:
  std::vector<Value*> inputs;
  std::vector<Value*> outputs;
};

template<typename T>
class Tensor: public Value {
};

// XXX: AttrType
namespace AttrType {
class INT {};
class FLOAT {};
class STRING {};
class TENSOR {};
class GRAPH {};
class INTS {};
class FLOATS {};
class STRINGS {};
}

} // namespace onnx
} // namespace onnc
