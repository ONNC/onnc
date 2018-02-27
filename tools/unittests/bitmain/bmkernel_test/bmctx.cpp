#include <bm_kernel.h>
#include <host_util.h>
#include <ios>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

#define CMD_BUF_NAME "cmdbuf.bin"
static void send_cmd_buf(void *user_data, const void *cmd_buf, u32 len)
{
	std::cout << __func__ << std::endl;
	std::cout << "save to " << CMD_BUF_NAME << std::endl;
	std::fstream output(CMD_BUF_NAME, std::ios::out | std::ios::trunc | std::ios::binary);
	output.write((char *)cmd_buf, len);
}

static void emit_cmd_buf(void *user_data, void *cmd_buf, u32 len)
{
	send_cmd_buf(user_data, cmd_buf, len);
	/* free implicitly */
	free(cmd_buf);
}

static void free_cmd_buf(void *user_data, void *cmd_buf)
{
	/* free without emit */
	emit_cmd_buf(user_data, cmd_buf, 0);
}

static void* alloc_cmd_buf(void *user_data, u32 size)
{
	void* cmd_buf = malloc(size);
	return cmd_buf;
}

static void host_sync()
{
}

static void emit_debug_info(void *user_data, char const *info, int node_id, u64 fw_addr, bool is_float)
{
}

static void *g_bmkernel_handle_;
void *bmkernel_context_prepare()
{
	bmkernel_info_t bmkernel_info_;
	hw_info_t hw_info_;

	memcpy(&hw_info_, get_hw_info(),sizeof(hw_info_t));
	std::cout << "default hw_info:"<< std::endl;
	std::cout << "  chip_version " << hw_info_.chip_version << std::endl
		<< "  nodechip_shift " << hw_info_.nodechip_shift << std::endl
		<< "  npu_shift " << hw_info_.npu_shift << std::endl
		<< "  eu_shift " << hw_info_.eu_shift << std::endl
		<< "  local_mem_shift " << hw_info_.local_mem_shift << std::endl
		<< "  local_mem_banks " << hw_info_.local_mem_banks << std::endl
		<< "  global_mem_size 0x" << std::hex << hw_info_.global_mem_size << std::endl
		<< "  nodechip_num " << hw_info_.nodechip_num << std::endl
		<< "  npu_num " << hw_info_.npu_num << std::endl
		<< "  eu_num " << hw_info_.eu_num << std::endl
		<< "  local_mem_size 0x" << std::hex << hw_info_.local_mem_size << std::endl;

	bmkernel_info_.chip_version = hw_info_.chip_version;
	bmkernel_info_.eu_shift = hw_info_.eu_shift;
	bmkernel_info_.npu_shift = hw_info_.npu_shift;
	bmkernel_info_.node_shift = hw_info_.nodechip_shift;
	bmkernel_info_.local_mem_shift = hw_info_.local_mem_shift;
	bmkernel_info_.local_mem_banks = hw_info_.local_mem_banks;
	bmkernel_info_.cmd_buf_size = 0x10000000;
	bmkernel_info_.emit_on_update = false;
	bmkernel_info_.alloc_cmd_buf = alloc_cmd_buf;
	bmkernel_info_.emit_cmd_buf = emit_cmd_buf;
	bmkernel_info_.free_cmd_buf = free_cmd_buf;
	bmkernel_info_.sync = host_sync;
	bmkernel_info_.debug = emit_debug_info;

	bmkernel_register(&bmkernel_info_, BMKERNEL_NODE_ID_HOST, (void *)0, &g_bmkernel_handle_);
	std::cout << "bmkernel_register done" << std::endl;
	return g_bmkernel_handle_;
}

