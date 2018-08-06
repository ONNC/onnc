dnl
dnl @synopsis CHECK_CANONICAL_SYSTEM
dnl
dnl @summary set up default value of `host`, `build` and `target`
dnl
dnl Luba Tang <lubatang@gmail.com>

AC_DEFUN([CHECK_CANONICAL_SYSTEM],
[dnl

AC_CANONICAL_HOST

AC_CACHE_CHECK([type of operating system we're going to host on],
  [onnc_cv_host_os_type],
  [case $host in
  *-*-aix*)
    onnc_cv_host_os_type="AIX"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-irix*)
    onnc_cv_host_os_type="IRIX"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-cygwin*)
    onnc_cv_host_os_type="Cygwin"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-darwin*)
    onnc_cv_host_os_type="Darwin"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-minix*)
    onnc_cv_host_os_type="Minix"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-freebsd*)
    onnc_cv_host_os_type="FreeBSD"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-openbsd*)
    onnc_cv_host_os_type="OpenBSD"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-netbsd*)
    onnc_cv_host_os_type="NetBSD"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-dragonfly*)
    onnc_cv_host_os_type="DragonFly"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-hpux*)
    onnc_cv_host_os_type="HP-UX"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-interix*)
    onnc_cv_host_os_type="Interix"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-linux*)
    onnc_cv_host_os_type="Linux"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-solaris*)
    onnc_cv_host_os_type="SunOS"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-auroraux*)
    onnc_cv_host_os_type="AuroraUX"
    onnc_cv_host_platform_type="Unix" ;;
  *-*-win32*)
    onnc_cv_host_os_type="Win32"
    onnc_cv_host_platform_type="Win32" ;;
  *-*-mingw*)
    onnc_cv_host_os_type="MingW"
    onnc_cv_host_platform_type="Win32" ;;
  *-*-haiku*)
    onnc_cv_host_os_type="Haiku"
    onnc_cv_host_platform_type="Unix" ;;
  *-unknown-eabi*)
    onnc_cv_host_os_type="Freestanding"
    onnc_cv_host_platform_type="Unix" ;;
  *-unknown-elf*)
    onnc_cv_host_os_type="Freestanding"
    onnc_cv_host_platform_type="Unix" ;;
  *)
    onnc_cv_host_os_type="Unknown"
    onnc_cv_host_platform_type="Unknown" ;;
  esac])

dnl Set the "OS" Makefile variable based on the platform type so the
dnl makefile can configure itself to specific build hosts
case $onnc_cv_host_os_type in
FreeBSD|OpenBSD|NetBSD|DragonFly)
  AC_DEFINE([ONNC_ON_BSD],[1],[Define if this is common BSD platform])
;;
Linux)
  AC_DEFINE([ONNC_ON_LINUX],[1],[Define if this is Linux platform])
;;
Darwin)
  AC_DEFINE([ONNC_ON_APPLE],[1],[Define if this is Apple platform])
;;
Unknown)
  AC_MSG_ERROR([Operating system is unknown, configure can't continue])
;;
esac

dnl Set the "ONNC_ON_*" variables based on llvm_cv_llvm_cv_platform_type
dnl This is used by lib/Support to determine the basic kind of implementation
dnl to use.
case $onnc_cv_host_platform_type in
Unix)
  AC_DEFINE([ONNC_ON_UNIX],[1],[Define if this is Unixish platform])
  AC_SUBST(ONNC_ON_PLATFORM,[ONNC_ON_UNIX])
;;
Win32)
  AC_DEFINE([ONNC_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(ONNC_ON_PLATFORM,[ONNC_ON_WIN32])
;;
esac

AC_SUBST(HOST_OS,$onnc_cv_host_os_type)

AC_CANONICAL_TARGET
AC_CACHE_CHECK([type of operating system we're going to target],
  [onnc_cv_target_os_type],
  [case $target in
  *-*-aix*)
    onnc_cv_target_os_type="AIX" ;;
  *-*-irix*)
    onnc_cv_target_os_type="IRIX" ;;
  *-*-cygwin*)
    onnc_cv_target_os_type="Cygwin" ;;
  *-*-darwin*)
  onnc_cv_target_os_type="Darwin" ;;
  *-*-minix*)
    onnc_cv_target_os_type="Minix" ;;
  *-*-freebsd* | *-*-kfreebsd-gnu)
    onnc_cv_target_os_type="FreeBSD" ;;
  *-*-openbsd*)
    onnc_cv_target_os_type="OpenBSD" ;;
  *-*-netbsd*)
    onnc_cv_target_os_type="NetBSD" ;;
  *-*-dragonfly*)
    onnc_cv_target_os_type="DragonFly" ;;
  *-*-hpux*)
    onnc_cv_target_os_type="HP-UX" ;;
  *-*-interix*)
    onnc_cv_target_os_type="Interix" ;;
  *-*-linux*)
    onnc_cv_target_os_type="Linux" ;;
  *-*-gnu*)
    onnc_cv_target_os_type="GNU" ;;
  *-*-solaris*)
    onnc_cv_target_os_type="SunOS" ;;
  *-*-auroraux*)
    onnc_cv_target_os_type="AuroraUX" ;;
  *-*-win32*)
    onnc_cv_target_os_type="Win32" ;;
  *-*-mingw*)
    onnc_cv_target_os_type="MingW" ;;
  *-*-haiku*)
    onnc_cv_target_os_type="Haiku" ;;
  *-*-rtems*)
    onnc_cv_target_os_type="RTEMS" ;;
  *-*-nacl*)
    onnc_cv_target_os_type="NativeClient" ;;
  *-unknown-eabi*)
    onnc_cv_target_os_type="Freestanding" ;;
  *)
    onnc_cv_target_os_type="Unknown" ;;
  esac])

if test "$onnc_cv_target_os_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target operating system])
fi

AC_SUBST(TARGET_OS,$onnc_cv_target_os_type)

AC_CACHE_CHECK([target architecture],
  [onnc_cv_target_arch_type],
  [case $target in
  i?86-*)                 onnc_cv_target_arch_type="x86" ;;
  amd64-* | x86_64-*)     onnc_cv_target_arch_type="x86_64" ;;
  sparc*-*)               onnc_cv_target_arch_type="Sparc" ;;
  powerpc*-*)             onnc_cv_target_arch_type="PowerPC" ;;
  arm*-*)                 onnc_cv_target_arch_type="ARM" ;;
  aarch64*-*)             onnc_cv_target_arch_type="AArch64" ;;
  mips-* | mips64-*)      onnc_cv_target_arch_type="Mips" ;;
  mipsel-* | mips64el-*)  onnc_cv_target_arch_type="Mips" ;;
  xcore-*)                onnc_cv_target_arch_type="XCore" ;;
  msp430-*)               onnc_cv_target_arch_type="MSP430" ;;
  hexagon-*)              onnc_cv_target_arch_type="Hexagon" ;;
  mblaze-*)               onnc_cv_target_arch_type="MBlaze" ;;
  nvptx-*)                onnc_cv_target_arch_type="NVPTX" ;;
  *)                      onnc_cv_target_arch_type="Unknown" ;;
  esac])

if test "$onnc_cv_target_arch_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target archicture])
fi

AC_SUBST(TARGET_ARCH,$onnc_cv_target_arch_type)

dnl Set the "ONNC_DEFAULT_TARGET_QUADRUPLE" variable based on $target.
dnl This is used to determine the default target quadruple and emulation
dnl to use.
AC_DEFINE_UNQUOTED([ONNC_DEFAULT_TARGET_QUADRUPLE],
          ["$target"],
          [default target quadruple])
])
