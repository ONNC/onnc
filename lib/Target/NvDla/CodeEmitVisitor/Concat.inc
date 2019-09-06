PP_DEFINE_VISIT(Concat, pOp)
{
        // Prepare attributes
        int axis = pOp.getAxis().value();
        int input_tensor_num = pOp.getNumOfInputs();
	assert(axis != 0);

        // Prepare input
        std::vector<int> input_mid;
	std::vector<ILoadable::MemoryListEntry> input_mle;
        std::vector<std::vector<int32_t>> input_tensor_dim;
        for(int i = 0; i < input_tensor_num; i++){
                const Tensor* input_tensor = pOp.getInput(i);
                std::vector<int32_t> single_tensor_dim;
		assert(input_tensor->getNumOfDimensions() == 4);
                for (int j = 0; j < input_tensor->getNumOfDimensions(); j++){
                        single_tensor_dim.push_back(input_tensor->dimension(j));
                }
                input_tensor_dim.push_back(single_tensor_dim);

                assert(input_tensor_dim[i][0] == 1);

                input_mid.push_back(m_pMeta.getMemoryListEntryId(*input_tensor));
                input_mle.push_back(m_pMeta.getMemoryListEntry(input_mid[i]));
        }
        
        // Prepare output
        const Tensor* output_tensor = pOp.getOutput(0);
        std::vector<int32_t> output_tensor_dim;
	assert(output_tensor->getNumOfDimensions() == 4);
        for (int i = 0; i < output_tensor->getNumOfDimensions(); i++){
                output_tensor_dim.push_back( output_tensor->dimension(i));
        }
        assert(output_tensor_dim[0] == 1);

        MemoryListEntryId          output_mid = m_pMeta.getMemoryListEntryId(*output_tensor);
        ILoadable::MemoryListEntry output_mle = m_pMeta.getMemoryListEntry(output_mid);

        genmem::NvDlaMemory mem(genmem::memWidth, this->m_pMeta.m_MemoryListEntries.size());

        // 1. generate a input cube
        std::vector<int> line_stride;
        std::vector<int> surf_stride;
        std::vector<int> planar_stride;
        std::vector<int> W;
        std::vector<int> H;
        std::vector<int> C;
        std::vector<genmem::Cube> input_cubes;

        for (int i = 0; i < input_tensor_num; i++){
                C.push_back(input_tensor_dim[i][1]);
                H.push_back(input_tensor_dim[i][2]);
                W.push_back(input_tensor_dim[i][3]);

                line_stride.push_back(genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, W[i], H[i] ,C[i]));
                surf_stride.push_back(genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE, W[i], H[i] ,C[i]));
                planar_stride.push_back( -1 );

                genmem::Cube input_cube(genmem::Cube::TYPE::CUBE, W[i], H[i], C[i], line_stride[i], surf_stride[i], planar_stride[i]);
                input_cube.address_entry = mem.request_memory_with_mid(input_cube.used_entry_number(), input_mid[i]);
                input_cubes.push_back(input_cube);
        }

        // 2. create a output cube
        int line_stride_output = genmem::Cube::get_compact_line_stride(genmem::Cube::TYPE::CUBE, output_tensor_dim[3], output_tensor_dim[2], output_tensor_dim[1]);
        int surf_stride_output = genmem::Cube::get_compact_surface_stride(genmem::Cube::TYPE::CUBE, output_tensor_dim[3], output_tensor_dim[2], output_tensor_dim[1]);

        genmem::Cube output_cube(genmem::Cube::TYPE::CUBE, output_tensor_dim[3], output_tensor_dim[2], output_tensor_dim[1], line_stride_output, surf_stride_output, -1);
	assert(output_cube.used_entry_number()*genmem::memWidth == output_mle.size);
        output_cube.address_entry = mem.request_memory_with_mid(output_cube.used_entry_number(), output_mid);

        // 3. concat
        std::vector<genmem::Register> regs;
	axis -= 1;
        genmem::Concat::concat(input_cubes, output_cube, mem, regs, axis, input_tensor_num);
        auto nvdla_ops = genmem::genmemRegisterToDalOp(m_pMeta, regs, mem);
				for(auto* op : nvdla_ops) issueDlaOp(op, NULL, m_pMeta.m_pPrevOp);
				

        assert( mem.genmem_addrress_entry_to_mid(output_cube.address_entry) == output_mid);
};
