#ifndef SUPPORT_PREPROCESSOR_H_INCLUDED 
#define SUPPORT_PREPROCESSOR_H_INCLUDED 

#define PP_COUNT(...) PP_GET_NTH_ARG_(__VA_ARGS__, \
 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, \
 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, \
 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, \
 100,  99,  98,  97,  96,  95,  94,  93,  92,  91, \
  90,  89,  88,  87,  86,  85,  84,  83,  82,  81, \
  80,  79,  78,  77,  76,  75,  74,  73,  72,  71, \
  70,  69,  68,  67,  66,  65,  64,  63,  62,  61, \
  60,  59,  58,  57,  56,  55,  54,  53,  52,  51, \
  50,  49,  48,  47,  46,  45,  44,  43,  42,  41, \
  40,  39,  38,  37,  36,  35,  34,  33,  32,  31, \
  30,  29,  28,  27,  26,  25,  24,  23,  22,  21, \
  20,  19,  18,  17,  16,  15,  14,  13,  12,  11, \
  10,   9,   8,   7,   6,   5,   4,   3,   2,   1  \
)
#define PP_GET_NTH_ARG_( \
   _1,   _2,   _3,   _4,   _5,   _6,   _7,   _8,   _9,  _10, \
  _11,  _12,  _13,  _14,  _15,  _16,  _17,  _18,  _19,  _20, \
  _21,  _22,  _23,  _24,  _25,  _26,  _27,  _28,  _29,  _30, \
  _31,  _32,  _33,  _34,  _35,  _36,  _37,  _38,  _39,  _40, \
  _41,  _42,  _43,  _44,  _45,  _46,  _47,  _48,  _49,  _50, \
  _51,  _52,  _53,  _54,  _55,  _56,  _57,  _58,  _59,  _60, \
  _61,  _62,  _63,  _64,  _65,  _66,  _67,  _68,  _69,  _70, \
  _71,  _72,  _73,  _74,  _75,  _76,  _77,  _78,  _79,  _80, \
  _81,  _82,  _83,  _84,  _85,  _86,  _87,  _88,  _89,  _90, \
  _91,  _92,  _93,  _94,  _95,  _96,  _97,  _98,  _99, _100, \
 _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, \
 _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, \
 _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, \
    N, ...) N

#define PP_CONCAT(x, y) PP_CONCAT_(x, y)
#define PP_CONCAT_(x, y) PP_CONCAT__(x, y)
#define PP_CONCAT__(x, y) x##y

#define PP_STRINGIFY(x) PP_STRINGIFY_(x)
#define PP_STRINGIFY_(x) PP_STRINGIFY__(x)
#define PP_STRINGIFY__(x) #x

#define PP_UNWRAP(list) PP_UNWRAP_(list)
#define PP_UNWRAP_(list) PP_UNWRAP__ list
#define PP_UNWRAP__(...) __VA_ARGS__

#define PP_LIST_FOR_EACH(macro, ...) PP_LIST_FOR_EACH_(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_(macro, ...) PP_LIST_FOR_EACH__(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH__(macro, ...) PP_CONCAT(PP_LIST_FOR_EACH_, PP_COUNT(__VA_ARGS__))(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_1(macro, x) macro(x)
#define PP_LIST_FOR_EACH_2(macro, x, ...) macro(x) PP_LIST_FOR_EACH_1(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_3(macro, x, ...) macro(x) PP_LIST_FOR_EACH_2(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_4(macro, x, ...) macro(x) PP_LIST_FOR_EACH_3(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_5(macro, x, ...) macro(x) PP_LIST_FOR_EACH_4(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_6(macro, x, ...) macro(x) PP_LIST_FOR_EACH_5(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_7(macro, x, ...) macro(x) PP_LIST_FOR_EACH_6(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_8(macro, x, ...) macro(x) PP_LIST_FOR_EACH_7(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_9(macro, x, ...) macro(x) PP_LIST_FOR_EACH_8(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_10(macro, x, ...) macro(x) PP_LIST_FOR_EACH_9(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_11(macro, x, ...) macro(x) PP_LIST_FOR_EACH_10(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_12(macro, x, ...) macro(x) PP_LIST_FOR_EACH_11(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_13(macro, x, ...) macro(x) PP_LIST_FOR_EACH_12(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_14(macro, x, ...) macro(x) PP_LIST_FOR_EACH_13(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_15(macro, x, ...) macro(x) PP_LIST_FOR_EACH_14(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_16(macro, x, ...) macro(x) PP_LIST_FOR_EACH_15(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_17(macro, x, ...) macro(x) PP_LIST_FOR_EACH_16(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_18(macro, x, ...) macro(x) PP_LIST_FOR_EACH_17(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_19(macro, x, ...) macro(x) PP_LIST_FOR_EACH_18(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_20(macro, x, ...) macro(x) PP_LIST_FOR_EACH_19(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_21(macro, x, ...) macro(x) PP_LIST_FOR_EACH_20(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_22(macro, x, ...) macro(x) PP_LIST_FOR_EACH_21(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_23(macro, x, ...) macro(x) PP_LIST_FOR_EACH_22(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_24(macro, x, ...) macro(x) PP_LIST_FOR_EACH_23(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_25(macro, x, ...) macro(x) PP_LIST_FOR_EACH_24(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_26(macro, x, ...) macro(x) PP_LIST_FOR_EACH_25(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_27(macro, x, ...) macro(x) PP_LIST_FOR_EACH_26(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_28(macro, x, ...) macro(x) PP_LIST_FOR_EACH_27(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_29(macro, x, ...) macro(x) PP_LIST_FOR_EACH_28(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_30(macro, x, ...) macro(x) PP_LIST_FOR_EACH_29(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_31(macro, x, ...) macro(x) PP_LIST_FOR_EACH_30(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_32(macro, x, ...) macro(x) PP_LIST_FOR_EACH_31(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_33(macro, x, ...) macro(x) PP_LIST_FOR_EACH_32(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_34(macro, x, ...) macro(x) PP_LIST_FOR_EACH_33(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_35(macro, x, ...) macro(x) PP_LIST_FOR_EACH_34(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_36(macro, x, ...) macro(x) PP_LIST_FOR_EACH_35(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_37(macro, x, ...) macro(x) PP_LIST_FOR_EACH_36(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_38(macro, x, ...) macro(x) PP_LIST_FOR_EACH_37(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_39(macro, x, ...) macro(x) PP_LIST_FOR_EACH_38(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_40(macro, x, ...) macro(x) PP_LIST_FOR_EACH_39(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_41(macro, x, ...) macro(x) PP_LIST_FOR_EACH_40(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_42(macro, x, ...) macro(x) PP_LIST_FOR_EACH_41(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_43(macro, x, ...) macro(x) PP_LIST_FOR_EACH_42(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_44(macro, x, ...) macro(x) PP_LIST_FOR_EACH_43(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_45(macro, x, ...) macro(x) PP_LIST_FOR_EACH_44(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_46(macro, x, ...) macro(x) PP_LIST_FOR_EACH_45(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_47(macro, x, ...) macro(x) PP_LIST_FOR_EACH_46(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_48(macro, x, ...) macro(x) PP_LIST_FOR_EACH_47(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_49(macro, x, ...) macro(x) PP_LIST_FOR_EACH_48(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_50(macro, x, ...) macro(x) PP_LIST_FOR_EACH_49(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_51(macro, x, ...) macro(x) PP_LIST_FOR_EACH_50(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_52(macro, x, ...) macro(x) PP_LIST_FOR_EACH_51(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_53(macro, x, ...) macro(x) PP_LIST_FOR_EACH_52(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_54(macro, x, ...) macro(x) PP_LIST_FOR_EACH_53(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_55(macro, x, ...) macro(x) PP_LIST_FOR_EACH_54(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_56(macro, x, ...) macro(x) PP_LIST_FOR_EACH_55(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_57(macro, x, ...) macro(x) PP_LIST_FOR_EACH_56(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_58(macro, x, ...) macro(x) PP_LIST_FOR_EACH_57(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_59(macro, x, ...) macro(x) PP_LIST_FOR_EACH_58(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_60(macro, x, ...) macro(x) PP_LIST_FOR_EACH_59(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_61(macro, x, ...) macro(x) PP_LIST_FOR_EACH_60(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_62(macro, x, ...) macro(x) PP_LIST_FOR_EACH_61(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_63(macro, x, ...) macro(x) PP_LIST_FOR_EACH_62(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_64(macro, x, ...) macro(x) PP_LIST_FOR_EACH_63(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_65(macro, x, ...) macro(x) PP_LIST_FOR_EACH_64(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_66(macro, x, ...) macro(x) PP_LIST_FOR_EACH_65(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_67(macro, x, ...) macro(x) PP_LIST_FOR_EACH_66(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_68(macro, x, ...) macro(x) PP_LIST_FOR_EACH_67(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_69(macro, x, ...) macro(x) PP_LIST_FOR_EACH_68(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_70(macro, x, ...) macro(x) PP_LIST_FOR_EACH_69(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_71(macro, x, ...) macro(x) PP_LIST_FOR_EACH_70(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_72(macro, x, ...) macro(x) PP_LIST_FOR_EACH_71(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_73(macro, x, ...) macro(x) PP_LIST_FOR_EACH_72(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_74(macro, x, ...) macro(x) PP_LIST_FOR_EACH_73(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_75(macro, x, ...) macro(x) PP_LIST_FOR_EACH_74(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_76(macro, x, ...) macro(x) PP_LIST_FOR_EACH_75(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_77(macro, x, ...) macro(x) PP_LIST_FOR_EACH_76(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_78(macro, x, ...) macro(x) PP_LIST_FOR_EACH_77(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_79(macro, x, ...) macro(x) PP_LIST_FOR_EACH_78(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_80(macro, x, ...) macro(x) PP_LIST_FOR_EACH_79(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_81(macro, x, ...) macro(x) PP_LIST_FOR_EACH_80(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_82(macro, x, ...) macro(x) PP_LIST_FOR_EACH_81(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_83(macro, x, ...) macro(x) PP_LIST_FOR_EACH_82(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_84(macro, x, ...) macro(x) PP_LIST_FOR_EACH_83(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_85(macro, x, ...) macro(x) PP_LIST_FOR_EACH_84(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_86(macro, x, ...) macro(x) PP_LIST_FOR_EACH_85(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_87(macro, x, ...) macro(x) PP_LIST_FOR_EACH_86(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_88(macro, x, ...) macro(x) PP_LIST_FOR_EACH_87(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_89(macro, x, ...) macro(x) PP_LIST_FOR_EACH_88(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_90(macro, x, ...) macro(x) PP_LIST_FOR_EACH_89(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_91(macro, x, ...) macro(x) PP_LIST_FOR_EACH_90(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_92(macro, x, ...) macro(x) PP_LIST_FOR_EACH_91(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_93(macro, x, ...) macro(x) PP_LIST_FOR_EACH_92(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_94(macro, x, ...) macro(x) PP_LIST_FOR_EACH_93(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_95(macro, x, ...) macro(x) PP_LIST_FOR_EACH_94(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_96(macro, x, ...) macro(x) PP_LIST_FOR_EACH_95(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_97(macro, x, ...) macro(x) PP_LIST_FOR_EACH_96(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_98(macro, x, ...) macro(x) PP_LIST_FOR_EACH_97(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_99(macro, x, ...) macro(x) PP_LIST_FOR_EACH_98(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_100(macro, x, ...) macro(x) PP_LIST_FOR_EACH_99(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_101(macro, x, ...) macro(x) PP_LIST_FOR_EACH_100(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_102(macro, x, ...) macro(x) PP_LIST_FOR_EACH_101(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_103(macro, x, ...) macro(x) PP_LIST_FOR_EACH_102(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_104(macro, x, ...) macro(x) PP_LIST_FOR_EACH_103(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_105(macro, x, ...) macro(x) PP_LIST_FOR_EACH_104(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_106(macro, x, ...) macro(x) PP_LIST_FOR_EACH_105(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_107(macro, x, ...) macro(x) PP_LIST_FOR_EACH_106(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_108(macro, x, ...) macro(x) PP_LIST_FOR_EACH_107(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_109(macro, x, ...) macro(x) PP_LIST_FOR_EACH_108(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_110(macro, x, ...) macro(x) PP_LIST_FOR_EACH_109(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_111(macro, x, ...) macro(x) PP_LIST_FOR_EACH_110(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_112(macro, x, ...) macro(x) PP_LIST_FOR_EACH_111(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_113(macro, x, ...) macro(x) PP_LIST_FOR_EACH_112(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_114(macro, x, ...) macro(x) PP_LIST_FOR_EACH_113(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_115(macro, x, ...) macro(x) PP_LIST_FOR_EACH_114(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_116(macro, x, ...) macro(x) PP_LIST_FOR_EACH_115(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_117(macro, x, ...) macro(x) PP_LIST_FOR_EACH_116(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_118(macro, x, ...) macro(x) PP_LIST_FOR_EACH_117(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_119(macro, x, ...) macro(x) PP_LIST_FOR_EACH_118(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_120(macro, x, ...) macro(x) PP_LIST_FOR_EACH_119(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_121(macro, x, ...) macro(x) PP_LIST_FOR_EACH_120(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_122(macro, x, ...) macro(x) PP_LIST_FOR_EACH_121(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_123(macro, x, ...) macro(x) PP_LIST_FOR_EACH_122(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_124(macro, x, ...) macro(x) PP_LIST_FOR_EACH_123(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_125(macro, x, ...) macro(x) PP_LIST_FOR_EACH_124(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_126(macro, x, ...) macro(x) PP_LIST_FOR_EACH_125(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_127(macro, x, ...) macro(x) PP_LIST_FOR_EACH_126(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_128(macro, x, ...) macro(x) PP_LIST_FOR_EACH_127(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_129(macro, x, ...) macro(x) PP_LIST_FOR_EACH_128(macro, __VA_ARGS__)
#define PP_LIST_FOR_EACH_130(macro, x, ...) macro(x) PP_LIST_FOR_EACH_129(macro, __VA_ARGS__)

#endif
