void Interpreter::visit(${OperatorName}& pOp) {
  // Prepare input
  ${visitor_prepare_input}
  // Prepare output
  ${visitor_prepare_output}
  // Prepare attributes
  ${visitor_prepare_attribute}

  // Call to Runtime
  ONNC_RUNTIME_${operator_name}_float(
    m_pContext
    ${visitor_pass_input}
    ${visitor_pass_output}
    ${visitor_pass_attribute}
  );
};
