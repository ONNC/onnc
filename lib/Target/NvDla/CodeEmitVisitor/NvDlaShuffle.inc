PP_DEFINE_VISIT(NvDlaShuffle, pOp)
{
	// Prepare input
	const Tensor* input_tensor = pOp.getInput(0);
	std::vector<int32_t> input_tensor_dim;
	assert(input_tensor->getNumOfDimensions() == 4);

	for (int i = 0; i < 4; i++) {
		input_tensor_dim.push_back(input_tensor->dimension(i));
	}
	assert(input_tensor_dim[0] == 1);

	MemoryListEntryId          input_mid = m_pMeta.getMemoryListEntryId(*input_tensor);
	ILoadable::MemoryListEntry input_mle = m_pMeta.getMemoryListEntry(input_mid);

	// Prepare output
	const Tensor* output_tensor = pOp.getOutput(0);
	std::vector<int32_t> output_tensor_dim;
	assert(output_tensor->getNumOfDimensions() == 4);

	for (int i = 0; i < 4; i++){
		output_tensor_dim.push_back(output_tensor->dimension(i));
	}

	MemoryListEntryId          output_mid = m_pMeta.getMemoryListEntryId(*output_tensor);
	ILoadable::MemoryListEntry output_mle = m_pMeta.getMemoryListEntry(output_mid);

	// Prepare attributes
	int axis = pOp.getAxis().value() ;
	int group = pOp.getGroup().value();
	assert(axis == 1);
	assert(input_tensor_dim[1]%group == 0);

	genmem::NvDlaMemory mem(genmem::memWidth, this->m_pMeta.m_MemoryListEntries.size());

	// 1. generate a input cube
	int C = input_tensor_dim[1];
	int H = input_tensor_dim[2];
	int W = input_tensor_dim[3];
	int line_stride = genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, W, H ,C);
	int surf_stride = genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE, W, H ,C);
	int planar_stride = -1;

	genmem::Cube input_cube(genmem::Cube::TYPE::CUBE, W, H ,C, line_stride, surf_stride, planar_stride);
	input_cube.address_entry = mem.request_memory_with_mid(input_cube.used_entry_number(), input_mid);

	// 2. create a output cube
	C = output_tensor_dim[1], H = output_tensor_dim[2], W = output_tensor_dim[3];
	line_stride = genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, W, H ,C);
	surf_stride = genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE,  W, H ,C);
	planar_stride = -1;

	genmem::Cube output_cube(genmem::Cube::TYPE::CUBE,  W, H ,C, line_stride, surf_stride, planar_stride);
	assert(output_cube.used_entry_number()*genmem::memWidth == output_mle.size);
	output_cube.address_entry = mem.request_memory_with_mid(output_cube.used_entry_number(), output_mid);

	// 3. shuffle
	std::vector<genmem::Register> regs;
	genmem::Shuffle::shuffle(input_cube, output_cube, mem, regs, 0, group);

	auto nvdla_ops = genmem::genmemRegisterToDalOp(m_pMeta, regs, mem);
	for(auto* op : nvdla_ops) issueDlaOp(op, NULL, m_pMeta.m_pPrevOp);

	assert(mem.genmem_addrress_entry_to_mid(output_cube.address_entry) == output_mid);
}
