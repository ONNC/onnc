void Interpreter::visit${OperatorName}(::onnx::Node *pNode) {
  // Prepare input
  ${visitor_prepare_input}
  // Prepare output
  ${visitor_prepare_output}
  // Prepare attributes
  ${visitor_prepare_attribute}

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "${operator_name}" << std::endl;
  ONNC_RUNTIME_${operator_name}_float(
    m_pContext
    ${visitor_pass_input}
    ${visitor_pass_output}
    ${visitor_pass_attribute}
  );
};
