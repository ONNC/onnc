#include <list>
#include <vector>
#include <string>

namespace onnc {
namespace onnx {


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

class Value {
 public:
  StringRef getName();
  // TODO: Program input?
  Operator* getDefine();
  unsigned getDefineNo();
  // TODO: Iterator
  std::list<Use> *getUses();
  void replaceAllUsesWith(Value *v);
 private:
  Operator* define;
  std::list<Use> *uses;
};
class Use {
 public:
  Value *get();
  Operator *getUser();
  unsigned getOperandNo() const;
 private:
  Operator *user;
  Value *value;
};
class Operator {
 public:
  Operator(StringRef p_name): name(p_name) {}
  // TODO: Operator Type
  StringRef getName();
  Value *getInput(unsigned index);
  Value *getOutput(unsigned index);
 private:
  StringRef name;
  std::vector<Value> *inputs;
  std::vector<Value> *outputs;
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
