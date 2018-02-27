#include <bm_kernel.h>
#include <stddef.h>
#include <bm_intrinsics.h>

void *bmkernel_context_prepare();
void tl_test_load()
{
	int ic, oc, kh, kw;
	tensor_lmem *tl_weight;
	shape_t weight_shape;
	kh = 5;
	kw = 5;
	oc = 1;
	ic = 1;
	weight_shape = shape_t4(ic, oc, kh, kw);
	tl_weight = tl_alloc(weight_shape, DATA_FMT_F32, CTRL_NULL);
	tl_load(tl_weight, 0, CTRL_NULL);
	tl_store(tl_weight, 0, CTRL_NULL);
}

int main()
{
	void * bmkernel_handle = bmkernel_context_prepare();
	kernel_enter(bmkernel_handle);
	tl_test_load();
	kernel_submit();
	kernel_exit();
	return 0;
}
