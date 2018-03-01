class TGOperator {
public:
  TGOperator(){};
  virtual ~TGOperator(){};

  virtual void emit() const = 0;
};

class TGConv : public TGOperator {
public:
  TGConv() {}
  virtual void emit() const;
};
