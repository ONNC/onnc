#include <vector>
#include <string>

#pragma once

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
class OperatorVisitor;

class Value
{
public:
  typedef std::vector<Use> UseList;

public:
  Value(StringRef pName)
    : m_Name(pName) {
  }

  StringRef getName() const { return m_Name; }

  Define* getDefine() { return m_pDefine; }

  unsigned int getDefineNo() { return m_DefineNo; }

  // TODO: Iterator
  const UseList& getUses() const { return m_Uses; }

  // TODO: Iterator
  UseList& getUses() { return m_Uses; }

  void replaceAllUsesWith(Value *v);

private:
  Define* m_pDefine;
  unsigned int m_DefineNo;
  UseList m_Uses;
  std::string m_Name;
};

class Define
{
public:
  Define(StringRef p_name): m_Name(p_name) {}

  StringRef getName();

private:
  std::string m_Name;
};

class Use
{
public:
  Value *get();

  Operator *getUser();

  unsigned getOperandNo() const;

private:
  Operator *m_User;
  unsigned int m_OperandNo;
  Value *m_Value;
};

class Operator : public Define
{
public:
  Operator(const std::string& pName)
    : Define(pName) {
  }

  Value *getInput(unsigned pIdx);

  const Value *getInput(unsigned pIdx) const;

  Value *getOutput(unsigned pIdx);

  const Value *getOutput(unsigned pIdx) const;

  virtual void accept(OperatorVisitor& pV);

private:
  typedef std::vector<Value*> ValueRefList;

private:
  ValueRefList m_Inputs;  //< all values used as inputs in this operator
  ValueRefList m_Outputs; //< all values used as outputs in this operator
};


// Value
// Tensor
// TensorT
class Tensor: public Value {
public:
  std::vector<int64_t> dimensions;
};
template<typename T>
class TensorT: public Tensor {
};

// XXX: AttrType
namespace AttrType {
typedef int64_t INT;
typedef float FLOAT;
typedef std::string STRING;
class TENSOR {};
class GRAPH {};
typedef std::vector<INT> INTS;
typedef std::vector<FLOAT> FLOATS;
typedef std::vector<STRING> STRINGS;
}

} // namespace tensor
} // namespace onnc
