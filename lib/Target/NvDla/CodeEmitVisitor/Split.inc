PP_DEFINE_VISIT(Split, pSplit)
{
	assert(pSplit.getInput(0)->getNumOfDimensions() == 4);
	assert(pSplit.getInput(0)->dimension(0) == 1);

	// prepare intput
	const Tensor* input_tensor = pSplit.getInput(0);
	std::vector<int32_t> input_tensor_dim;
	for (int i = 0; i < input_tensor->getNumOfDimensions(); ++i)
		input_tensor_dim.push_back( input_tensor->dimension(i) );

	MemoryListEntryId          input_mid = m_pMeta.getMemoryListEntryId(*input_tensor);
	ILoadable::MemoryListEntry input_mle = m_pMeta.getMemoryListEntry(input_mid);

	// prepare attributes
	int axis = pSplit.getAxis().value();
	assert(axis == 1);

	genmem::SplittParm split_parm;
	for(size_t i=0; i<pSplit.getSplit().vector().size(); i++) {
		split_parm.parm.push_back(pSplit.getSplit().at(i));
	}

	// prepare output
	int output_tensor_num = pSplit.getNumOfOutputs();
	std::vector<int> output_mid;
	std::vector<ILoadable::MemoryListEntry> output_mle;
	std::vector<std::vector<int32_t>> output_tensor_dim;
	for(int i = 0; i < output_tensor_num; i++){
		const Tensor* output_tensor = pSplit.getOutput(i);
		std::vector<int32_t> single_tensor_dim;
		for (int j = 0; j < output_tensor->getNumOfDimensions(); j++)
			single_tensor_dim.push_back( output_tensor->dimension(j));

		output_tensor_dim.push_back(single_tensor_dim);
		assert(output_tensor_dim[i][0] == 1);

		output_mid.push_back(m_pMeta.getMemoryListEntryId(*output_tensor));
		output_mle.push_back(m_pMeta.getMemoryListEntry(output_mid[i]));
	}

	// generate a input cube
	genmem::Cube input_cube(genmem::Cube::TYPE::CUBE, input_tensor_dim[3], input_tensor_dim[2], input_tensor_dim[1],
			genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, input_tensor_dim[3], input_tensor_dim[2], input_tensor_dim[1]),
			genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE, input_tensor_dim[3], input_tensor_dim[2], input_tensor_dim[1]),
			-1
			);
	genmem::NvDlaMemory mem(genmem::memWidth, this->m_pMeta.m_MemoryListEntries.size());
	input_cube.address_entry = mem.request_memory_with_mid(input_cube.used_entry_number(), input_mid);

	// generate output cubes
	std::vector<genmem::Cube> output_cubes;
	for (int i = 0; i < output_tensor_num; i++){
		genmem::Cube output_cube(genmem::Cube::TYPE::CUBE, output_tensor_dim[i][3], output_tensor_dim[i][2], output_tensor_dim[i][1], 
				genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, output_tensor_dim[i][3], output_tensor_dim[i][2], output_tensor_dim[i][1]),
				genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE, output_tensor_dim[i][3], output_tensor_dim[i][2], output_tensor_dim[i][1]),
				-1
				);
		output_cube.address_entry = mem.request_memory_with_mid(output_cube.used_entry_number(), output_mid[i]);
		output_cubes.push_back(output_cube);
	}

	// split
	std::vector<genmem::Register> regs;
	genmem::Splitt::splitt(input_cube, output_cubes, mem, regs, axis-1, split_parm);

	auto nvdla_ops = genmem::genmemRegisterToDalOp(m_pMeta, regs, mem);
	for(auto* op : nvdla_ops) issueDlaOp(op, NULL, m_pMeta.m_pPrevOp);
}
