PP_DEFINE_VISIT(Add, pAdd) { emitSdp(SDP_OP_ADD, *pAdd.getA(), *pAdd.getB(), *pAdd.getOutput(0)); }
