import jinja2


def get_max_len(items, f):
    max_v = 0
    for i in items:
        max_v = max(len(f(i)), max_v)
    return max_v


class bm_param_type:
    def __init__(self, name):
        type_dict = {
            "gaddr_t": "uint64",
            "laddr_t": "uint64",
            "int": "int32",
            "u64": "uint64",
            "u32": "uint32",
            "u16": "uint32",
            "u8": "uint32"
        }
        if name in type_dict:
            self.__proto_name = type_dict[name]
        else:
            self.__proto_name = name
        self.name = name
        self.is_vector = False
        self.is_const = False

    def get_proto_name(self):
        if self.is_vector:
            s = "repeated {}".format(self.__proto_name)
        else:
            s = "optional {}".format(self.__proto_name)
        return s

    def cpp_name(self):
        s = self.name
        if self.is_vector:
            s = "{}*".format(s)
        if self.is_const:
            s = "const {}".format(s)
        if self.name == 'ActivationMethod':
            s = "bmnet::bm1880::Inst::{}".format(self.name)
        return s

    def proto_cast(self, ns):
        if self.name in ['ActivationMethod']:
            return "({}::Inst::{})".format(ns, self.name)
        return ""

    def cpp_cast(self):
        if self.name in ['ActivationMethod']:
            return "(ActivationMethod)"
        return ""


class bm_param:
    def __init__(self, name):
        self.name = name

    def set_type(self, name):
        self.type = bm_param_type(name)
        return self

    def set_c_array(self, index):
        self.type.is_vector = True
        self.type.is_std_vector = False
        if type(index) is int:
            self.type.vector_fixed_size = True
            self.type.vector_size = index
        else:
            self.type.vector_fixed_size = False
            self.type.vector_index = index
            self.type.vector_size = index
        return self

    def set_vector(self):
        self.type.is_vector = True
        self.type.is_std_vector = True
        return self

    def set_const(self):
        self.type.is_const = True
        return self

    def add_comment(self, c):
        self.comment_ = c
        return self

    def get_arg(self):
        s = "{} {}".format(self.type.cpp_name(), self.name)
        return s


class bm_inst:
    def __init__(self, name, short_name):
        self.name = name
        self.short_name = short_name
        self.__params = []

    def add_ctx(self):
        self.need_ctx = True
        return self

    def add_param(self, *arg):
        p = bm_param(*arg)
        self.__params.append(p)
        return p

    def get_proto_name(self, i):
        f = "{{:{}}} {{:{}}} = {{}};".format(self.name_len_max,
                                             self.short_name_len_max)
        s = f.format(self.name, self.short_name, i)
        return s

    def get_prarms(self):
        name_max = get_max_len(self.__params, lambda p: p.name)
        for p in self.__params:
            p.name_max = name_max
        type_max = get_max_len(self.__params,
                               lambda p: p.type.get_proto_name())
        for p in self.__params:
            p.type_max = type_max
        return self.__params


class bm_inst_group:
    def __init__(self):
        self.__insts = []
        self.__name_check = set()
        self.__short_name_check = set()

    def __add_inst_base(self, inst):
        assert not ' ' in inst.name
        assert not inst.name in self.__name_check
        self.__name_check.add(inst.name)
        assert not inst.short_name in self.__short_name_check
        self.__short_name_check.add(inst.short_name)
        self.__insts.append(inst)
        return inst

    def add_inst(self, *arg):
        if len(arg) == 1 and type(arg[0]) == bm_inst:
            return self.__add_inst_base(arg[0])
        return self.__add_inst_base(bm_inst(*arg))

    def get_insts(self):
        name_max = get_max_len(self.__insts, lambda i: i.name)
        for i in self.__insts:
            i.name_len_max = name_max
        len_max = get_max_len(self.__insts, lambda i: i.short_name)
        for i in self.__insts:
            i.short_name_len_max = len_max
        return self.__insts


def get_tg():
    tg = bm_inst_group()
    cur_inst = tg.add_inst("bmnet_pooling_fixed_forward_bmkernel",
                           "pooling").add_ctx()
    cur_inst.add_param("ifmap_gaddr").set_type("gaddr_t")
    cur_inst.add_param("ofmap_gaddr").set_type("gaddr_t")
    cur_inst.add_param("index_gaddr").set_type("gaddr_t")
    cur_inst.add_param("o_findex_gaddr").set_type("gaddr_t")
    cur_inst.add_param("n").set_type("int")
    cur_inst.add_param("c").set_type("int")
    cur_inst.add_param("h").set_type("int")
    cur_inst.add_param("w").set_type("int")
    cur_inst.add_param("kh").set_type("int")
    cur_inst.add_param("kw").set_type("int")
    cur_inst.add_param("pad_top").set_type("int")
    cur_inst.add_param("pad_bot").set_type("int")
    cur_inst.add_param("pad_left").set_type("int")
    cur_inst.add_param("pad_right").set_type("int")
    cur_inst.add_param("stride_h").set_type("int")
    cur_inst.add_param("stride_w").set_type("int")
    cur_inst.add_param("is_avg_pooling").set_type("int")
    cur_inst.add_param("avg_const").set_type("float").add_comment(
        "default(passing 0.0f) is 1/kh*kw")
    cur_inst.add_param("do_relu").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        1).set_const()

    cur_inst = tg.add_inst("bmnet_conv_fixed_forward_bmkernel",
                           "conv").add_ctx()
    cur_inst.add_param("ga_ifmap").set_type("gaddr_t")
    cur_inst.add_param("ga_ofmap").set_type("gaddr_t")
    cur_inst.add_param("ga_weight").set_type("gaddr_t")
    cur_inst.add_param("ga_bias").set_type("gaddr_t")
    cur_inst.add_param("ga_bn_mean").set_type("gaddr_t")
    cur_inst.add_param("ga_bn_variance").set_type("gaddr_t")
    cur_inst.add_param("ga_scale").set_type("gaddr_t")
    cur_inst.add_param("ga_scale_bias").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("groups").set_type("int")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("kh").set_type("u16")
    cur_inst.add_param("kw").set_type("u16")
    cur_inst.add_param("dilation_h").set_type("u16")
    cur_inst.add_param("dilation_w").set_type("u16")
    cur_inst.add_param("pad_h").set_type("u8")
    cur_inst.add_param("pad_w").set_type("u8")
    cur_inst.add_param("stride_h").set_type("u8")
    cur_inst.add_param("stride_w").set_type("u8")
    cur_inst.add_param("result_add").set_type("int")
    cur_inst.add_param("do_bias").set_type("int")
    cur_inst.add_param("do_bn").set_type("int")
    cur_inst.add_param("do_scale").set_type("int")
    cur_inst.add_param("do_scale_bias").set_type("int")
    cur_inst.add_param("do_activation").set_type("int")
    cur_inst.add_param("bn_scale").set_type("float")
    cur_inst.add_param("bn_eps").set_type("float")
    cur_inst.add_param("activation_method").set_type("int")
    cur_inst.add_param("activation_arg").set_type("float").set_c_array(1)
    cur_inst.add_param("activation_ga_slope").set_type("gaddr_t")
    cur_inst.add_param("activation_channel_shared").set_type("bool")
    cur_inst.add_param("activation_gt_scale").set_type("int")
    cur_inst.add_param("activation_gt_rshift").set_type("int")
    cur_inst.add_param("activation_le_scale").set_type("int").add_comment(
        "slope, TODO")
    cur_inst.add_param("activation_le_rshift").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("bn_right_shift_width").set_type("int")
    cur_inst.add_param("scalar_right_shift_width").set_type("int")
    cur_inst.add_param("use_winograd").set_type("bool")

    cur_inst = tg.add_inst("bmnet_conv_parallel_fixed_forward_bmkernel",
                           "conv_p").add_ctx()
    cur_inst.add_param("ga_ifmap").set_type("gaddr_t")
    cur_inst.add_param("ga_ofmap").set_type("gaddr_t")
    cur_inst.add_param("ga_weight").set_type("gaddr_t")
    cur_inst.add_param("ga_bias").set_type("gaddr_t")
    cur_inst.add_param("ga_bn_mean").set_type("gaddr_t")
    cur_inst.add_param("ga_bn_variance").set_type("gaddr_t")
    cur_inst.add_param("ga_scale").set_type("gaddr_t")
    cur_inst.add_param("ga_scale_bias").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("groups").set_type("int")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("kh").set_type("u16")
    cur_inst.add_param("kw").set_type("u16")
    cur_inst.add_param("dilation_h").set_type("u16")
    cur_inst.add_param("dilation_w").set_type("u16")
    cur_inst.add_param("pad_h").set_type("u8")
    cur_inst.add_param("pad_w").set_type("u8")
    cur_inst.add_param("stride_h").set_type("u8")
    cur_inst.add_param("stride_w").set_type("u8")
    cur_inst.add_param("result_add").set_type("int")
    cur_inst.add_param("do_bias").set_type("int")
    cur_inst.add_param("do_bn").set_type("int")
    cur_inst.add_param("do_scale").set_type("int")
    cur_inst.add_param("do_scale_bias").set_type("int")
    cur_inst.add_param("do_activation").set_type("int")
    cur_inst.add_param("bn_scale").set_type("float")
    cur_inst.add_param("bn_eps").set_type("float")
    cur_inst.add_param("activation_method").set_type("int")
    cur_inst.add_param("activation_arg").set_type("float").set_c_array(1)
    cur_inst.add_param("activation_ga_slope").set_type("gaddr_t")
    cur_inst.add_param("activation_channel_shared").set_type("bool")
    cur_inst.add_param("activation_gt_scale").set_type("int")
    cur_inst.add_param("activation_gt_rshift").set_type("int")
    cur_inst.add_param("activation_le_scale").set_type("int").add_comment(
        "slope, TODO")
    cur_inst.add_param("activation_le_rshift").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("bn_right_shift_width").set_type("int")
    cur_inst.add_param("scale_right_shift_width").set_type("int")
    cur_inst.add_param("use_winograd").set_type("bool")

    cur_inst = tg.add_inst("bmnet_fc_fixed_forward_bmkernel",
                           "fc").add_ctx().add_ctx()
    cur_inst.add_param("bottom_data_gaddr").set_type("gaddr_t")
    cur_inst.add_param("weight_data_gaddr").set_type("gaddr_t")
    cur_inst.add_param("bias_data_gaddr").set_type("gaddr_t")
    cur_inst.add_param("top_data_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_row_num").set_type("int")
    cur_inst.add_param("input_col_num").set_type("int")
    cur_inst.add_param("weight_col_num").set_type("int")
    cur_inst.add_param("have_bias").set_type("int")
    cur_inst.add_param("do_activation").set_type("int")
    cur_inst.add_param("activation_method").set_type("int")
    cur_inst.add_param("activation_ga_slope").set_type("gaddr_t")
    cur_inst.add_param("activation_channel_shared").set_type("int")
    cur_inst.add_param("activation_gt_scale").set_type("int")
    cur_inst.add_param("activation_gt_rshift").set_type("int")
    cur_inst.add_param("activation_le_scale").set_type("int")
    cur_inst.add_param("activation_le_rshift").set_type("int")
    cur_inst.add_param("weight_transpose").set_type("bool")
    cur_inst.add_param("left_shift_width").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")

    cur_inst = tg.add_inst("bmnet_relu_fixed_forward_bmkernel",
                           "relu").add_ctx()
    cur_inst.add_param("bottom_gaddr").set_type("gaddr_t")
    cur_inst.add_param("top_gaddr").set_type("gaddr_t")
    cur_inst.add_param("negative_slope").set_type("float")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")

    cur_inst = tg.add_inst("bmnet_leakyrelu_fixed_forward_bmkernel",
                           "leakyrelu").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("u64")
    cur_inst.add_param("output_gaddr").set_type("u64")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("GT_right_shift_width").set_type("int")
    cur_inst.add_param("LE_right_shift_width").set_type("int")
    cur_inst.add_param("GT_scale").set_type("int")
    cur_inst.add_param("LE_scale").set_type("int")

    cur_inst = tg.add_inst("bmnet_prelu_fixed_forward_bmkernel",
                           "prelu").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("gaddr_t")
    cur_inst.add_param("slope_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    # cur_inst.add_param("arm_reserved_gaddr").set_type("gaddr_t")
    cur_inst.add_param("channel_shared").set_type("int")
    cur_inst.add_param("slope").set_type("int")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("GT_scale").set_type("int")
    cur_inst.add_param("GT_right_shift_width").set_type("int")
    cur_inst.add_param("LE_right_shift_width").set_type("int").add_comment(
        "slope_right_shift_width")

    cur_inst = tg.add_inst("bmnet_batchnorm_fixed_forward_inference_bmkernel",
                           "batchnorm").add_ctx()
    cur_inst.add_param("bottom_gaddr").set_type("gaddr_t")
    cur_inst.add_param("mean_ma_gaddr").set_type("gaddr_t")
    cur_inst.add_param("variance_ma_gaddr").set_type("gaddr_t")
    cur_inst.add_param("top_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")

    cur_inst = tg.add_inst("bmnet_scale_fixed_forward_bmkernel",
                           "scale").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("gaddr_t")
    cur_inst.add_param("scale_gaddr").set_type("gaddr_t")
    cur_inst.add_param("bias_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("scale_dim").set_type("int")
    cur_inst.add_param("inner_dim").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")

    cur_inst = tg.add_inst("bmnet_reshape_fixed_forward_bmkernel",
                           "reshape").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_dim_len").set_type("int")
    cur_inst.add_param("output_dim").set_type("int").set_c_array(
        "output_dim_len")

    cur_inst = tg.add_inst("bmnet_split_fixed_forward_bmkernel",
                           "split").add_ctx()
    cur_inst.add_param("bottom_gaddr").set_type("gaddr_t")
    cur_inst.add_param("top_gaddr").set_type("gaddr_t").set_c_array("top_size")
    cur_inst.add_param("top_size").set_type("int")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")

    cur_inst = tg.add_inst("bmnet_concat_fixed_forward_bmkernel",
                           "concat").add_ctx()
    cur_inst.add_param("input_gaddrs").set_type("gaddr_t").set_c_array(
        "input_num")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_dims").set_type("int").set_c_array("input_num")
    cur_inst.add_param("input_num").set_type("int")
    cur_inst.add_param("concat_axis").set_type("int")
    cur_inst.add_param("output_dim_len").set_type("int")
    cur_inst.add_param("output_dim").set_type("int").set_c_array(
        "output_dim_len")

    cur_inst = tg.add_inst("bmnet_eltwise_fixed_forward_bmkernel",
                           "eltwise").add_ctx()
    cur_inst.add_param("ga_input").set_type("gaddr_t").set_c_array(
        "input_size")
    cur_inst.add_param("ga_output").set_type("gaddr_t")
    cur_inst.add_param("input_size").set_type("int")
    cur_inst.add_param("op").set_type("int")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("do_relu").set_type("bool")
    cur_inst.add_param("relu_slope").set_type("float")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        "input_size").set_const()

    cur_inst = tg.add_inst("bmnet_permute_fixed_forward_bmkernel",
                           "permute").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("output_n").set_type("int")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("output_h").set_type("int")
    cur_inst.add_param("output_w").set_type("int")
    cur_inst.add_param("order_n").set_type("int")
    cur_inst.add_param("order_c").set_type("int")
    cur_inst.add_param("order_h").set_type("int")
    cur_inst.add_param("order_w").set_type("int")
    cur_inst.add_param("need_permute").set_type("bool")

    cur_inst = tg.add_inst("bmnet_priorbox_fixed_forward_bmkernel",
                           "priorbox").add_ctx()
    cur_inst.add_param("weight_data_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("output_h").set_type("int")
    cur_inst.add_param("output_w").set_type("int")

    cur_inst = tg.add_inst("bmnet_lrn_fixed_forward_bmkernel", "lrn").add_ctx()
    cur_inst.add_param("bottom_gaddr").set_type("gaddr_t")
    cur_inst.add_param("top_gaddr").set_type("gaddr_t")
    cur_inst.add_param("sqr_lut_gaddr").set_type("gaddr_t")
    cur_inst.add_param("power_lut_gaddr").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("size").set_type("int")
    cur_inst.add_param("sum_right_shift_width").set_type("int")
    cur_inst.add_param("lrn_right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        2).set_const()

    cur_inst = tg.add_inst("bmnet_upsample_fixed_bmkernel",
                           "upsample").add_ctx()
    cur_inst.add_param("ga_ifmap").set_type("gaddr_t")
    cur_inst.add_param("ga_ofmap").set_type("gaddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("size").set_type("int")

    cur_inst = tg.add_inst("bmnet_normalize_fixed_forward_bmkernel",
                           "normalize").add_ctx()
    cur_inst.add_param("input_gaddr").set_type("gaddr_t")
    cur_inst.add_param("output_gaddr").set_type("gaddr_t")
    cur_inst.add_param("has_scale_blob").set_type("bool")
    cur_inst.add_param("scale_gaddr").set_type("gaddr_t")
    cur_inst.add_param("sqr_lut_gaddr").set_type("gaddr_t")
    cur_inst.add_param("sqrt_lut_gaddr").set_type("gaddr_t")
    cur_inst.add_param("across_spatial").set_type("bool")
    cur_inst.add_param("channel_shared").set_type("bool")
    cur_inst.add_param("scales").set_type("int").set_c_array(0)
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("sum_right_shift_width").set_type("int")
    cur_inst.add_param("norma_right_shift_width").set_type("int")
    cur_inst.add_param("scale_right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        2).set_const()
    return tg


def get_tl():
    tl = bm_inst_group()
    cur_inst = tl.add_inst(
        "bmnet_tl_conv_forward_bmkernel",
        "bm_tl_conv").add_ctx()  # tl_conv has been defined as MACRO QQ
    cur_inst.add_param("la_ifmap").set_type("laddr_t")
    cur_inst.add_param("la_ofmap").set_type("laddr_t")
    cur_inst.add_param("la_weight").set_type("laddr_t")
    cur_inst.add_param("la_bias").set_type("laddr_t")
    cur_inst.add_param("group_weight").set_type("u32").set_c_array("group")
    cur_inst.add_param("group_bias").set_type("u32").set_c_array("group")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("group").set_type("int")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("output_h").set_type("int")
    cur_inst.add_param("output_w").set_type("int")
    cur_inst.add_param("kh").set_type("u32")
    cur_inst.add_param("kw").set_type("u32")
    cur_inst.add_param("dh").set_type("u32")
    cur_inst.add_param("dw").set_type("u32")
    cur_inst.add_param("pad_h_top").set_type("u32")
    cur_inst.add_param("pad_h_bottom").set_type("u32")
    cur_inst.add_param("pad_w_left").set_type("u32")
    cur_inst.add_param("pad_w_right").set_type("u32")
    cur_inst.add_param("stride_h").set_type("u32")
    cur_inst.add_param("stride_w").set_type("u32")
    cur_inst.add_param("result_add").set_type("u32")
    cur_inst.add_param("ctrl").set_type("u32")
    cur_inst.add_param("rshift").set_type("int")
    cur_inst.add_param("do_bias").set_type("bool")
    cur_inst.add_param("use_winograd").set_type("bool")
    cur_inst.add_param("do_relu").set_type("bool")

    cur_inst = tl.add_inst("bmnet_tl_activation_forward_bmkernel",
                           "tl_activation")
    cur_inst.add_param("input_laddr").set_type("laddr_t")
    cur_inst.add_param("output_laddr").set_type("laddr_t")
    cur_inst.add_param("weight_laddr").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("gt_rshift").set_type("int")
    cur_inst.add_param("le_rshift").set_type("int")
    cur_inst.add_param("gt_scale").set_type("int")
    cur_inst.add_param("le_scale").set_type("int")
    cur_inst.add_param("gt_right_shift_width").set_type("int")
    cur_inst.add_param("le_right_shift_width").set_type("int")
    cur_inst.add_param("activation_arg_len").set_type("int")
    cur_inst.add_param("activation_arg").set_type("float").set_c_array(
        "activation_arg_len")
    cur_inst.add_param("channel_shared").set_type("bool")
    cur_inst.add_param("activation_type").set_type(
        "ActivationMethod").set_const()

    cur_inst = tl.add_inst("bmnet_tl_scale_forward_bmkernel", "tl_csale")
    cur_inst.add_param("input_laddr").set_type("laddr_t")
    cur_inst.add_param("output_laddr").set_type("laddr_t")
    cur_inst.add_param("scale_laddr").set_type("laddr_t")
    cur_inst.add_param("bias_laddr").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("scale_dim").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("do_bias").set_type("bool")
    cur_inst.add_param("do_relu").set_type("bool")
    cur_inst.add_param("relu_slope").set_type("float")

    cur_inst = tl.add_inst("bmnet_tl_batchnorm_forward_bmkernel", "tl_bn")
    cur_inst.add_param("bottom_laddr").set_type("laddr_t")
    cur_inst.add_param("top_laddr").set_type("laddr_t")
    cur_inst.add_param("mean_laddr").set_type("laddr_t")
    cur_inst.add_param("variance_laddr").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("do_relu").set_type("bool")

    cur_inst = tl.add_inst("bmnet_tl_eltwise_forward_bmkernel", "tl_eltwise")
    cur_inst.add_param("la_input").set_type("laddr_t").set_c_array(
        "input_size")
    cur_inst.add_param("la_output").set_type("laddr_t")
    cur_inst.add_param("la_working").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("input_size").set_type("int")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("op").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        "input_size").set_const()
    cur_inst.add_param("use_default_coeff").set_type("bool")
    cur_inst.add_param("do_relu").set_type("bool")
    cur_inst.add_param("relu_slope").set_type("float")

    cur_inst = tl.add_inst("bmnet_tl_lrn_forward_bmkernel", "tl_lrn")
    cur_inst.add_param("bottom_laddr").set_type("laddr_t")
    cur_inst.add_param("top_laddr").set_type("laddr_t")
    cur_inst.add_param("sqr_lut_laddr").set_type("laddr_t")
    cur_inst.add_param("power_lut_laddr").set_type("laddr_t")
    cur_inst.add_param("working_laddr").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("size").set_type("int")
    cur_inst.add_param("sum_right_shift_width").set_type("int")
    cur_inst.add_param("lrn_right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int").set_c_array(
        2).set_const()

    cur_inst = tl.add_inst("bmnet_tl_pooling_forward_bmkernel", "tl_pool")
    cur_inst.add_param("ifmap_laddr").set_type("laddr_t")
    cur_inst.add_param("ofmap_laddr").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("output_n").set_type("int")
    cur_inst.add_param("output_c").set_type("int")
    cur_inst.add_param("output_h").set_type("int")
    cur_inst.add_param("output_w").set_type("int")
    cur_inst.add_param("kh").set_type("u32")
    cur_inst.add_param("kw").set_type("u32")
    cur_inst.add_param("stride_h").set_type("u32")
    cur_inst.add_param("stride_w").set_type("u32")
    cur_inst.add_param("pad_h_top").set_type("u32")
    cur_inst.add_param("pad_h_bottom").set_type("u32")
    cur_inst.add_param("pad_w_left").set_type("u32")
    cur_inst.add_param("pad_w_right").set_type("u32")
    cur_inst.add_param("is_avg_pooling").set_type("bool")
    cur_inst.add_param("right_shift_width").set_type("int")
    cur_inst.add_param("threshold_x_quantized").set_type("int")

    cur_inst = tl.add_inst("bmnet_tl_upsample_forward_bmkernel",
                           "tl_upsample").add_ctx()
    cur_inst.add_param("la_ifmap").set_type("laddr_t")
    cur_inst.add_param("la_ofmap").set_type("laddr_t")
    cur_inst.add_param("input_n").set_type("int")
    cur_inst.add_param("input_c").set_type("int")
    cur_inst.add_param("input_h").set_type("int")
    cur_inst.add_param("input_w").set_type("int")
    cur_inst.add_param("output_h").set_type("int")
    cur_inst.add_param("output_w").set_type("int")
    cur_inst.add_param("size").set_type("int")
    return tl


def main():
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument("--api", required=True)
    parser.add_argument("--proto", required=True)
    args = parser.parse_args()

    templateLoader = jinja2.FileSystemLoader(searchpath="./")
    templateEnv = jinja2.Environment(
        loader=templateLoader, trim_blocks=True, lstrip_blocks=True)

    tg = get_tg()
    tl = get_tl()
    tg_tl = bm_inst_group()
    for i in tg.get_insts():
        tg_tl.add_inst(i)
    for i in tl.get_insts():
        tg_tl.add_inst(i)
    template = templateEnv.get_template("bmkernel_api.h.in")
    outputText = template.render(tg=tg, tl=tl, tg_tl=tg_tl)
    with open(args.api, 'w') as f:
        f.write(outputText)

    template = templateEnv.get_template("asm.proto.in")
    outputText = template.render(tg_tl=tg_tl)
    with open(args.proto, 'w') as f:
        f.write(outputText)


if __name__ == "__main__":
    main()
