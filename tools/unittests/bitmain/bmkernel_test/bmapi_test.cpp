#include <bm_kernel.h>
#include <stddef.h>
#include <bmkernel_api.h>

void *bmkernel_context_prepare();
void bmapi_test()
{
  bmnet_conv_forward_bmkernel(
      0,    // gaddr_t             ga_ifmap,
      0,    // gaddr_t             ga_ofmap,
      0,    // gaddr_t             ga_weight,
      0,    // gaddr_t             ga_bias,
      0,    // gaddr_t             ga_bn_mean,
      0,    // gaddr_t             ga_bn_variance,
      0,    // gaddr_t             ga_scale,
      0,    // gaddr_t             ga_scale_bias,
      4,    // int                 input_n,
      3,    // int                 input_c,
      50,    // int                 input_h,
      50,    // int                 input_w,
      1,    // int                 groups,
      5,    // int                 output_c,
      5,    // u16                 kh,
      5,    // u16                 kw,
      1,    // u16                 dilation_h,
      1,    // u16                 dilation_w,
      0,    // u8                  pad_h,
      0,    // u8                  pad_w,
      1,    // u8                  stride_h,
      1,    // u8                  stride_w,
      false, //int                 result_add,
      0,    // int                 do_bias,
      0,    // int                 do_bn,
      0,    // int                 do_scale,
      0,    // int                 do_scale_bias,
      0,    // int                 do_activation,
      0,    // float               bn_scale,
      0,    // float               bn_eps,
      0,    // int                 activation_method,
      0,    // float               activation_arg[],
      0,    // gaddr_t             activation_ga_slope,
      0    //  bool                activation_channel_shared
      );
}

int main()
{
	void * bmkernel_handle = bmkernel_context_prepare();
	kernel_enter(bmkernel_handle);
	bmapi_test();
	kernel_submit();
	kernel_exit();
	return 0;
}
