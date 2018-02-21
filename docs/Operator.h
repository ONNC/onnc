


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

class StringRef;
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
  Operator(StringRef p_names): names(p_names);
  // TODO: Operator Type
  StringRef getName();
  Value *getInput(unsigned index);
  Value *getOutput(unsigned index);
 private:
  StringRef name;
  std::vector<Value> *inputs;
  std::vector<Value> *outputs;
};

class Tensor: public Value {
};

}
}
