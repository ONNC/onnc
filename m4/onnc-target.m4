dnl
dnl @synopsis ENUM_ONNC_TARGETS
dnl
dnl @summary enumlate ONNC Targets, set up variables:
dnl   ONNC_TARGET_PLATFORMS
dnl   ONNC_TARGET_BACKENDS
dnl
dnl Luba Tang <luba@skymizer.com>

AC_DEFUN([ENUM_ONNC_TARGETS],
[dnl
  dnl Allow specific targets to be specified for building (or not)
  TARGETS_TO_BUILD="";

  AC_ARG_ENABLE([targets],
    [AS_HELP_STRING([--enable-targets],
              [Build specific host targets: all or target1,target2,... Valid targets are:
       host, vanilla, sophon, x86, x86_64, sparc, powerpc, alpha, aarch64, arm,
       arm64, mips, spu, hexagon, xcore, msp430, systemz, blackfin, ptx, cbe, and cpp (default=all)])],
    [],
    [enableval=all])

  AC_MSG_CHECKING([target backends])
  case "$enableval" in
    all) TARGETS_TO_BUILD="Vanilla X86 Sparc PowerPC Alpha AArch64 ARM Mips Hexagon CellSPU XCore MSP430 SystemZ Blackfin CBackend CppBackend MBlaze PTX Sophon TG"
        ;;
    *)for a_target in `echo $enableval|sed -e 's/,/ /g' ` ; do
        case "$a_target" in
    vanilla)
        TARGETS_TO_BUILD="Vanilla $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_VANILLA_TARGET, 1, [define Vanilla target])
        ;;
    x86)
        TARGETS_TO_BUILD="X86 $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_X86_TARGET, 1, [define x86 target])
        ;;
    x86_64)
        TARGETS_TO_BUILD="X86 $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_X86_TARGET, 1, [define x86_64 target])
        ;;
    sparc)    TARGETS_TO_BUILD="Sparc $TARGETS_TO_BUILD" ;;
    powerpc)  TARGETS_TO_BUILD="PowerPC $TARGETS_TO_BUILD" ;;
    alpha)    TARGETS_TO_BUILD="Alpha $TARGETS_TO_BUILD" ;;
    aarch64)  TARGETS_TO_BUILD="AArch64 $TARGETS_TO_BUILD" ;;
    arm64)
        TARGETS_TO_BUILD="AArch64 $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_AArch64_TARGET, 1, [define AArch64 target])
        ;;
    arm)
        TARGETS_TO_BUILD="ARM $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_ARM_TARGET, 1, [define ARM target])
        ;;
    mips)     TARGETS_TO_BUILD="Mips $TARGETS_TO_BUILD" ;;
    hexagon)  TARGETS_TO_BUILD="Hexagon $TARGETS_TO_BUILD" ;;
    spu)      TARGETS_TO_BUILD="CellSPU $TARGETS_TO_BUILD" ;;
    xcore)    TARGETS_TO_BUILD="XCore $TARGETS_TO_BUILD" ;;
    msp430)   TARGETS_TO_BUILD="MSP430 $TARGETS_TO_BUILD" ;;
    systemz)  TARGETS_TO_BUILD="SystemZ $TARGETS_TO_BUILD" ;;
    blackfin) TARGETS_TO_BUILD="Blackfin $TARGETS_TO_BUILD" ;;
    cbe)      TARGETS_TO_BUILD="CBackend $TARGETS_TO_BUILD" ;;
    cpp)      TARGETS_TO_BUILD="CppBackend $TARGETS_TO_BUILD" ;;
    mblaze)   TARGETS_TO_BUILD="MBlaze $TARGETS_TO_BUILD" ;;
    ptx)      TARGETS_TO_BUILD="PTX $TARGETS_TO_BUILD" ;;
    sophon)
        TARGETS_TO_BUILD="Sophon $TARGETS_TO_BUILD"
        AC_DEFINE(ENABLE_SOPHON_TARGET, 1, [define sohpon target])
        ;;
    *) AC_MSG_ERROR([Unrecognized target $a_target]) ;;
        esac
    done
    ;;
  esac

  AC_SUBST(TARGETS_TO_BUILD,$TARGETS_TO_BUILD)
  AC_MSG_RESULT([$TARGETS_TO_BUILD])

  dnl Build the ONNC_TARGET and ONNC_* macros for Targets.def and the individual
  dnl target feature def files.
  ONNC_TARGET_PLATFORMS=""
  ONNC_TARGET_BACKENDS=""
  for target_to_build in $TARGETS_TO_BUILD; do
    if test -d ${srcdir}/lib/Target/${target_to_build} ; then
      ONNC_TARGET_PLATFORMS="ONNC_PLATFORM($target_to_build) $ONNC_TARGET_PLATFORMS"
    fi
    dnl check *Backend.cpp file
    if test -f ${srcdir}/lib/Target/${target_to_build}/*Backend.cpp ; then
      ONNC_TARGET_BACKENDS="ONNC_BACKEND($target_to_build) $ONNC_TARGET_BACKENDS";
    fi
  done

  AC_SUBST(ONNC_TARGET_PLATFORMS)
  AC_SUBST(ONNC_TARGET_BACKENDS)

  dnl enable variables
  AM_CONDITIONAL([ENABLE_VANILLA_TARGET], [ test "${TARGETS_TO_BUILD/Vanilla}" != "${TARGETS_TO_BUILD}" ])
  AM_CONDITIONAL([ENABLE_X86_TARGET],     [ test "${TARGETS_TO_BUILD/X86}"     != "${TARGETS_TO_BUILD}" ])
  AM_CONDITIONAL([ENABLE_TG_TARGET],      [ test "${TARGETS_TO_BUILD/TG}"      != "${TARGETS_TO_BUILD}" ])
  AM_CONDITIONAL([ENABLE_AArch64_TARGET], [ test "${TARGETS_TO_BUILD/AArch64}" != "${TARGETS_TO_BUILD}" ])
  AM_CONDITIONAL([ENABLE_ARM_TARGET],     [ test "${TARGETS_TO_BUILD/ARM}"     != "${TARGETS_TO_BUILD}" ])
  AM_CONDITIONAL([ENABLE_SOPHON_TARGET],  [ test "${TARGETS_TO_BUILD/Sophon}"  != "${TARGETS_TO_BUILD}" ])

  dnl include target-dependent autoconf function calls.
  AM_COND_IF([ENABLE_X86_TARGET],     [m4_include(m4/targets/x86.m4)])
  AM_COND_IF([ENABLE_TG_TARGET],      [m4_include(m4/targets/sophon.m4)])
])
