add_libonnc_src(
  DefaultAttributes.cpp
  Lower.cpp
  LowerRegistry.cpp
  AbsLower.cpp
  AcosLower.cpp
  AddLower.cpp
  AffineLower.cpp
  AndLower.cpp
  ArgMaxLower.cpp
  ArgMinLower.cpp
  AsinLower.cpp
  AtanLower.cpp
  # TODO: ATenLower.cpp
  AveragePoolLower.cpp
  BatchNormalizationLower.cpp
  CastLower.cpp
  CeilLower.cpp
  ClipLower.cpp
  ConcatLower.cpp
  # FIXME: ConstantLower.cpp
  # TODO: ConstantFillLower.cpp
  ConvLower.cpp
  # TODO: ConvTransposeLower.cpp
  CosLower.cpp
  # TODO: CropLower.cpp
  # TODO: DepthToSpaceLower.cpp
  DivLower.cpp
  DropoutLower.cpp
  # TODO: EluLower.cpp
  EqualLower.cpp
  ExpLower.cpp
  # TODO: ExpandLower.cpp
  FlattenLower.cpp
  FloorLower.cpp
  # TODO: GatherLower.cpp
  GemmLower.cpp
  # TODO: GivenTensorFillLower.cpp
  GlobalAveragePoolLower.cpp
  GlobalLpPoolLower.cpp
  GlobalMaxPoolLower.cpp
  GreaterLower.cpp
  # TODO: GruLower.cpp
  # TODO: GruUnitLower.cpp
  HardmaxLower.cpp
  HardSigmoidLower.cpp
  IdentityLower.cpp
  # TODO: IfLower.cpp
  ImageScalerLower.cpp
  InstanceNormalizationLower.cpp
  LeakyReluLower.cpp
  LessLower.cpp
  LogLower.cpp
  LogSoftmaxLower.cpp
  # TODO: LoopLower.cpp
  LpNormalizationLower.cpp
  LpPoolLower.cpp
  LRNLower.cpp
  # TODO: LSTMLower.cpp
  MatMulLower.cpp
  MaxLower.cpp
  MaxPoolLower.cpp
  # TODO: MaxRoiPoolLower.cpp
  MeanLower.cpp
  # TODO: MeanVarianceNormalizationLower.cpp
  MinLower.cpp
  MulLower.cpp
  MultinomialLower.cpp
  NegLower.cpp
  NotLower.cpp
  OrLower.cpp
  PadLower.cpp
  ParametricSoftplusLower.cpp
  PowLower.cpp
  PReluLower.cpp
  RandomNormalLower.cpp
  RandomNormalLikeLower.cpp
  RandomUniformLower.cpp
  RandomUniformLikeLower.cpp
  ReciprocalLower.cpp
  ReduceL1Lower.cpp
  ReduceL2Lower.cpp
  ReduceLogSumLower.cpp
  ReduceLogSumExpLower.cpp
  ReduceMaxLower.cpp
  ReduceMeanLower.cpp
  ReduceMinLower.cpp
  ReduceProdLower.cpp
  ReduceSumLower.cpp
  ReduceSumSquareLower.cpp
  ReluLower.cpp
  ReshapeLower.cpp
  # TODO: RnnLower.cpp
  ScaleLower.cpp
  ScaledTanhLower.cpp
  # TODO: ScanLower.cpp
  SeluLower.cpp
  ShapeLower.cpp
  SigmoidLower.cpp
  SinLower.cpp
  SizeLower.cpp
  SliceLower.cpp
  SoftmaxLower.cpp
  # TODO: SoftplusLower.cpp
  SoftsignLower.cpp
  SpaceToDepthLower.cpp
  SplitLower.cpp
  SqrtLower.cpp
  SqueezeLower.cpp
  SubLower.cpp
  SumLower.cpp
  TanLower.cpp
  TanhLower.cpp
  ThresholdedReluLower.cpp
  TileLower.cpp
  TopKLower.cpp
  TransposeLower.cpp
  UnsqueezeLower.cpp
  UpsampleLower.cpp
  XorLower.cpp
)
