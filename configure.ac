#
#                     The ONNC Project
#
# See LICENSE.TXT for details.
##############################################################################

#  Autoconf/Automake Initialization

dnl AC_INIT(PKG_NAME, BUILD_VERSION, BUG_REPORTER)
AC_INIT([ONNC],[dev],[bugs@onnc.org])
AC_PREREQ(2.59)

AC_CONFIG_SRCDIR([LICENSE.TXT])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

####################
#  Check environments - `host`, `target` and `build`
CHECK_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([1.9 dist-bzip2 foreign nostdinc subdir-objects])

####################
#  Languages
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext])
AC_CXX_STL_HASH
AC_PROG_RANLIB
AC_PROG_LEX
AC_PROG_YACC
AM_CONDITIONAL(GCC, test "$GCC" = yes)   # let the Makefile know if we're gcc
LT_INIT

####################
# Check for compiler warnings
CHECK_WERROR_FLAGS
CXX_FLAG_CHECK([-Wno-variadic-macros])

####################
# Check for libraries
SETUP_CONFIG_HEADER
AC_CONFIG_HEADER(include/onnc/Config/Config.h)
AC_CONFIG_HEADER(include/onnc/Support/DataTypes.h)

####################
# Check for types
AC_CHECK_TYPES([int8_t, int16_t, int32_t, int64_t, intptr_t])
AC_CHECK_TYPES([uint8_t, uint16_t, uint32_t, uint64_t, uintptr_t])
AC_CHECK_TYPES([long double, long long, long long int, unsigned long long int])
AC_CHECK_TYPES([off_t, size_t, ssize_t])
AC_TYPE_LONG_DOUBLE_WIDER

####################
# Check for headers
AC_CHECK_HEADERS([sys/time.h])

####################
# Check for functions
AC_SEARCH_LIBS([clock_gettime], [rt], [
    AC_DEFINE(HAVE_CLOCK_GETTIME, 1,[Have clock_gettime])
])
AC_CHECK_FUNCS([gettimeofday])

####################
# Check for options
CHECK_PTHREAD
CHECK_SKYPAT
CHECK_BMKERNEL
CHECK_ZLIB
CHECK_ENABLE_OPTION([debug], [no], [ENABLE_DEBUG])
CHECK_ENABLE_OPTION([unittest], [no], [ENABLE_UNITTEST])
CHECK_ENABLE_OPTION([regression], [no], [ENABLE_REGRESSION])
CHECK_ENABLE_OPTION([optimize], [no], [ENABLE_OPTIMIZE])
CHECK_ENABLE_OPTION([clock_gettime], [no], [ENABLE_CLOCK_GETTIME])
CHECK_ENABLE_OPTION([gettimeofday], [yes], [ENABLE_GETTIMEOFDAY])

####################
# Check for variables and targets
CHECK_ONNC
ENUM_ONNC_TARGETS

####################
# OUTPUT
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([include/onnc/Makefile])
AC_CONFIG_FILES([include/onnc/Config/Platforms.def])
AC_CONFIG_FILES([include/onnc/Config/Backends.def])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([tools/Makefile])
AC_CONFIG_FILES([tools/unittests/Makefile])
AC_CONFIG_FILES([tools/unittests/bitmain/Makefile])
AC_CONFIG_FILES([tools/unittests/bitmain/bmkernel_test/Makefile])
AC_CONFIG_FILES([tools/onnc/Makefile])

AC_OUTPUT
