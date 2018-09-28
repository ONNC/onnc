add_libonnc_src(
    BookONNXGraphs.cpp
    BuildInitializers.cpp
    BuildInputOperators.cpp
    BuildOutputOperators.cpp
    DeadNodeElimination.cpp
    GraphBuildingPass.cpp
    RemoveTrainingNodes.cpp
    TensorSel.cpp
    TensorSel/DefaultAttributes.cpp
    TensorSel/Lower.cpp
    TensorSel/LowerRegistry.cpp
	TensorSel/AbsLower.cpp
	TensorSel/AcosLower.cpp
	TensorSel/AddLower.cpp
	TensorSel/AffineLower.cpp
	TensorSel/AndLower.cpp
	TensorSel/ArgMaxLower.cpp
	TensorSel/ArgMinLower.cpp
	TensorSel/AsinLower.cpp
	TensorSel/AtanLower.cpp
	# TODO: TensorSel/ATenLower.cpp
	TensorSel/AveragePoolLower.cpp
	TensorSel/BatchNormalizationLower.cpp
	# TODO: TensorSel/CastLower.cpp
	TensorSel/CeilLower.cpp
	TensorSel/ClipLower.cpp
	TensorSel/ConcatLower.cpp
	# FIXME: TensorSel/ConstantLower.cpp
	# TODO: TensorSel/ConstantFillLower.cpp
	TensorSel/ConvLower.cpp
	# TODO: TensorSel/ConvTransposeLower.cpp
	TensorSel/CosLower.cpp
	# TODO: TensorSel/CropLower.cpp
	# TODO: TensorSel/DepthToSpaceLower.cpp
	TensorSel/DivLower.cpp
	TensorSel/DropoutLower.cpp
	# TODO: TensorSel/EluLower.cpp
	TensorSel/EqualLower.cpp
	TensorSel/ExpLower.cpp
	# TODO: TensorSel/ExpandLower.cpp
	TensorSel/FlattenLower.cpp
	TensorSel/FloorLower.cpp
	# TODO: TensorSel/GatherLower.cpp
	TensorSel/GemmLower.cpp
	# TODO: TensorSel/GivenTensorFillLower.cpp
	TensorSel/GlobalAveragePoolLower.cpp
	TensorSel/GlobalLpPoolLower.cpp
	TensorSel/GlobalMaxPoolLower.cpp
	TensorSel/GreaterLower.cpp
	# TODO: TensorSel/GruLower.cpp
	# TODO: TensorSel/GruUnitLower.cpp
	TensorSel/HardmaxLower.cpp
	TensorSel/HardSigmoidLower.cpp
	TensorSel/IdentityLower.cpp
	# TODO: TensorSel/IfLower.cpp
	TensorSel/ImageScalerLower.cpp
	TensorSel/InstanceNormalizationLower.cpp
	TensorSel/LeakyReluLower.cpp
	TensorSel/LessLower.cpp
	TensorSel/LogLower.cpp
	TensorSel/LogSoftmaxLower.cpp
	# TODO: TensorSel/LoopLower.cpp
	TensorSel/LpNormalizationLower.cpp
	TensorSel/LpPoolLower.cpp
	TensorSel/LRNLower.cpp
	# TODO: TensorSel/LSTMLower.cpp
	TensorSel/MatMulLower.cpp
	TensorSel/MaxLower.cpp
	TensorSel/MaxPoolLower.cpp
	# TODO: TensorSel/MaxRoiPoolLower.cpp
	TensorSel/MeanLower.cpp
	# TODO: TensorSel/MeanVarianceNormalizationLower.cpp
	TensorSel/MinLower.cpp
	TensorSel/MulLower.cpp
	TensorSel/MultinomialLower.cpp
	TensorSel/NegLower.cpp
	TensorSel/NotLower.cpp
	TensorSel/OrLower.cpp
	TensorSel/PadLower.cpp
	TensorSel/ParametricSoftplusLower.cpp
	TensorSel/PowLower.cpp
	TensorSel/PReluLower.cpp
	TensorSel/RandomNormalLower.cpp
	TensorSel/RandomNormalLikeLower.cpp
	TensorSel/RandomUniformLower.cpp
	TensorSel/RandomUniformLikeLower.cpp
	TensorSel/ReciprocalLower.cpp
	TensorSel/ReduceL1Lower.cpp
	TensorSel/ReduceL2Lower.cpp
	TensorSel/ReduceLogSumLower.cpp
	TensorSel/ReduceLogSumExpLower.cpp
	TensorSel/ReduceMaxLower.cpp
	TensorSel/ReduceMeanLower.cpp
	TensorSel/ReduceMinLower.cpp
	TensorSel/ReduceProdLower.cpp
	TensorSel/ReduceSumLower.cpp
	TensorSel/ReduceSumSquareLower.cpp
	TensorSel/ReluLower.cpp
	TensorSel/ReshapeLower.cpp
	# TODO: TensorSel/RnnLower.cpp
	TensorSel/ScaleLower.cpp
	TensorSel/ScaledTanhLower.cpp
	# TODO: TensorSel/ScanLower.cpp
	TensorSel/SeluLower.cpp
	TensorSel/ShapeLower.cpp
	TensorSel/SigmoidLower.cpp
	TensorSel/SinLower.cpp
	TensorSel/SizeLower.cpp
	TensorSel/SliceLower.cpp
	TensorSel/SoftmaxLower.cpp
	# TODO: TensorSel/SoftplusLower.cpp
	TensorSel/SoftsignLower.cpp
	TensorSel/SpaceToDepthLower.cpp
	TensorSel/SplitLower.cpp
	TensorSel/SqrtLower.cpp
	TensorSel/SqueezeLower.cpp
	TensorSel/SubLower.cpp
	TensorSel/SumLower.cpp
	TensorSel/TanLower.cpp
	TensorSel/TanhLower.cpp
	TensorSel/ThresholdedReluLower.cpp
	TensorSel/TileLower.cpp
	TensorSel/TopKLower.cpp
	TensorSel/TransposeLower.cpp
	TensorSel/UnsqueezeLower.cpp
	TensorSel/UpsampleLower.cpp
	TensorSel/XorLower.cpp
)
