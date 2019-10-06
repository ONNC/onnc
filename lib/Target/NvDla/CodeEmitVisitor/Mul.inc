PP_DEFINE_VISIT(Mul, pMul) { emitSdp(SDP_OP_MUL, *pMul.getA(), *pMul.getB(), *pMul.getOutput(0)); }
