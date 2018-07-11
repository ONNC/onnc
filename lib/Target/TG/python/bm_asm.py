import jinja2


def get_max_len(items, f):
    max_v = 0
    for i in items:
        max_v = max(len(f(i)), max_v)
    return max_v


class param_type:
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
        return s

    def proto_cast(self, ns):
        if self.name in ['ActivationMethod']:
            return "({}::Inst::{})".format(ns, self.name)
        return ""

    def cpp_cast(self):
        if self.name in ['ActivationMethod']:
            return "(ActivationMethod)"
        return ""


class param:
    def __init__(self, name):
        self.name = name

    def set_type(self, name):
        self.type = param_type(name)
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


class inst:
    def __init__(self, name, short_name):
        self.name = name
        self.short_name = short_name
        self.__params = []

    def add_ctx(self):
        self.need_ctx = True
        return self

    def add_param(self, *arg):
        p = param(*arg)
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


class inst_group:
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
        if len(arg) == 1 and type(arg[0]) == inst:
            return self.__add_inst_base(arg[0])
        return self.__add_inst_base(inst(*arg))

    def get_insts(self):
        name_max = get_max_len(self.__insts, lambda i: i.name)
        for i in self.__insts:
            i.name_len_max = name_max
        len_max = get_max_len(self.__insts, lambda i: i.short_name)
        for i in self.__insts:
            i.short_name_len_max = len_max
        return self.__insts
