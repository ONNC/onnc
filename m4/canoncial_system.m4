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
  [skymizer_cv_host_os_type],
  [case $host in
  *-*-aix*)
    skymizer_cv_host_os_type="AIX"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-irix*)
    skymizer_cv_host_os_type="IRIX"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-cygwin*)
    skymizer_cv_host_os_type="Cygwin"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-darwin*)
    skymizer_cv_host_os_type="Darwin"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-minix*)
    skymizer_cv_host_os_type="Minix"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-freebsd*)
    skymizer_cv_host_os_type="FreeBSD"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-openbsd*)
    skymizer_cv_host_os_type="OpenBSD"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-netbsd*)
    skymizer_cv_host_os_type="NetBSD"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-dragonfly*)
    skymizer_cv_host_os_type="DragonFly"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-hpux*)
    skymizer_cv_host_os_type="HP-UX"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-interix*)
    skymizer_cv_host_os_type="Interix"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-linux*)
    skymizer_cv_host_os_type="Linux"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-solaris*)
    skymizer_cv_host_os_type="SunOS"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-auroraux*)
    skymizer_cv_host_os_type="AuroraUX"
    skymizer_cv_host_platform_type="Unix" ;;
  *-*-win32*)
    skymizer_cv_host_os_type="Win32"
    skymizer_cv_host_platform_type="Win32" ;;
  *-*-mingw*)
    skymizer_cv_host_os_type="MingW"
    skymizer_cv_host_platform_type="Win32" ;;
  *-*-haiku*)
    skymizer_cv_host_os_type="Haiku"
    skymizer_cv_host_platform_type="Unix" ;;
  *-unknown-eabi*)
    skymizer_cv_host_os_type="Freestanding"
    skymizer_cv_host_platform_type="Unix" ;;
  *-unknown-elf*)
    skymizer_cv_host_os_type="Freestanding"
    skymizer_cv_host_platform_type="Unix" ;;
  *)
    skymizer_cv_host_os_type="Unknown"
    skymizer_cv_host_platform_type="Unknown" ;;
  esac])

dnl Set the "OS" Makefile variable based on the platform type so the
dnl makefile can configure itself to specific build hosts
case $skymizer_cv_host_os_type in
FreeBSD|OpenBSD|NetBSD|DragonFly)
  AC_DEFINE([SKYMIZER_ON_BSD],[1],[Define if this is common BSD platform])
;;
Linux)
  AC_DEFINE([SKYMIZER_ON_LINUX],[1],[Define if this is Linux platform])
;;
Darwin)
  AC_DEFINE([SKYMIZER_ON_APPLE],[1],[Define if this is Apple platform])
;;
Unknown)
  AC_MSG_ERROR([Operating system is unknown, configure can't continue])
;;
esac

dnl Set the "SKYMIZER_ON_*" variables based on llvm_cv_llvm_cv_platform_type
dnl This is used by lib/Support to determine the basic kind of implementation
dnl to use.
case $skymizer_cv_host_platform_type in
Unix)
  AC_DEFINE([SKYMIZER_ON_UNIX],[1],[Define if this is Unixish platform])
  AC_SUBST(SKYMIZER_ON_PLATFORM,[SKYMIZER_ON_UNIX])
;;
Win32)
  AC_DEFINE([SKYMIZER_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(SKYMIZER_ON_PLATFORM,[SKYMIZER_ON_WIN32])
;;
esac

AC_SUBST(HOST_OS,$skymizer_cv_host_os_type)

AC_CANONICAL_TARGET
AC_CACHE_CHECK([type of operating system we're going to target],
  [skymizer_cv_target_os_type],
  [case $target in
  *-*-aix*)
    skymizer_cv_target_os_type="AIX" ;;
  *-*-irix*)
    skymizer_cv_target_os_type="IRIX" ;;
  *-*-cygwin*)
    skymizer_cv_target_os_type="Cygwin" ;;
  *-*-darwin*)
  skymizer_cv_target_os_type="Darwin" ;;
  *-*-minix*)
    skymizer_cv_target_os_type="Minix" ;;
  *-*-freebsd* | *-*-kfreebsd-gnu)
    skymizer_cv_target_os_type="FreeBSD" ;;
  *-*-openbsd*)
    skymizer_cv_target_os_type="OpenBSD" ;;
  *-*-netbsd*)
    skymizer_cv_target_os_type="NetBSD" ;;
  *-*-dragonfly*)
    skymizer_cv_target_os_type="DragonFly" ;;
  *-*-hpux*)
    skymizer_cv_target_os_type="HP-UX" ;;
  *-*-interix*)
    skymizer_cv_target_os_type="Interix" ;;
  *-*-linux*)
    skymizer_cv_target_os_type="Linux" ;;
  *-*-gnu*)
    skymizer_cv_target_os_type="GNU" ;;
  *-*-solaris*)
    skymizer_cv_target_os_type="SunOS" ;;
  *-*-auroraux*)
    skymizer_cv_target_os_type="AuroraUX" ;;
  *-*-win32*)
    skymizer_cv_target_os_type="Win32" ;;
  *-*-mingw*)
    skymizer_cv_target_os_type="MingW" ;;
  *-*-haiku*)
    skymizer_cv_target_os_type="Haiku" ;;
  *-*-rtems*)
    skymizer_cv_target_os_type="RTEMS" ;;
  *-*-nacl*)
    skymizer_cv_target_os_type="NativeClient" ;;
  *-unknown-eabi*)
    skymizer_cv_target_os_type="Freestanding" ;;
  *)
    skymizer_cv_target_os_type="Unknown" ;;
  esac])

if test "$skymizer_cv_target_os_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target operating system])
fi

AC_SUBST(TARGET_OS,$skymizer_cv_target_os_type)

AC_CACHE_CHECK([target architecture],
  [skymizer_cv_target_arch_type],
  [case $target in
  i?86-*)                 skymizer_cv_target_arch_type="x86" ;;
  amd64-* | x86_64-*)     skymizer_cv_target_arch_type="x86_64" ;;
  sparc*-*)               skymizer_cv_target_arch_type="Sparc" ;;
  powerpc*-*)             skymizer_cv_target_arch_type="PowerPC" ;;
  arm*-*)                 skymizer_cv_target_arch_type="ARM" ;;
  aarch64*-*)             skymizer_cv_target_arch_type="AArch64" ;;
  mips-* | mips64-*)      skymizer_cv_target_arch_type="Mips" ;;
  mipsel-* | mips64el-*)  skymizer_cv_target_arch_type="Mips" ;;
  xcore-*)                skymizer_cv_target_arch_type="XCore" ;;
  msp430-*)               skymizer_cv_target_arch_type="MSP430" ;;
  hexagon-*)              skymizer_cv_target_arch_type="Hexagon" ;;
  mblaze-*)               skymizer_cv_target_arch_type="MBlaze" ;;
  nvptx-*)                skymizer_cv_target_arch_type="NVPTX" ;;
  *)                      skymizer_cv_target_arch_type="Unknown" ;;
  esac])

if test "$skymizer_cv_target_arch_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target archicture])
fi

AC_SUBST(TARGET_ARCH,$skymizer_cv_target_arch_type)

dnl Set the "SKYMIZER_DEFAULT_TARGET_TRIPLE" variable based on $target.
dnl This is used to determine the default target triple and emulation
dnl to use.
AC_DEFINE_UNQUOTED([SKYMIZER_DEFAULT_TARGET_TRIPLE],
                   $target,
                   [default target triple])

])
