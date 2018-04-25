

set(VERSION dev)
set(PACKAGE_VERSION dev)
set(HAVE_CXX14 TRUE)
set(PACKAGE "onnc")
set(PACKAGE_BUGREPORT "bugs@onnc.org")
set(PACKAGE_NAME "ONNC")
set(PACKAGE_STRING "ONNC dev")
set(PACKAGE_TARNAME "onnc")
set(PACKAGE_URL "")

####################
#  Check environments - `host`, `target` and `build`
include(cmake/canoncial_system.cmake)
check_canonical_system()

####################
#  Languages
include(cmake/stl_hash.cmake)
determine_hash_namespace()

####################
# Check for compiler warnings
option(ENABLE_WERROR "Compile with -Werror enabled (default is OFF)" OFF)
if (ENABLE_WERROR)
    add_compile_options(-Werror)
endif()

####################
# Check for libraries
find_package(Threads)
if (Threads_FOUND)
    set(HAVE_PTHREAD 1)
endif ()
find_package(ZLIB 1.2.0.4)
if (ZLIB_FOUND)
    set(HAVE_ZLIB 1)
endif (ZLIB_FOUND)
find_package(Protobuf REQUIRED)

####################
# Check for types
include(cmake/platform_check.cmake)
check_types(int8_t int16_t int32_t int64_t intptr_t)
check_types(uint8_t uint16_t uint32_t uint64_t uintptr_t)
check_types(double "long double" "long long" "long long int" "unsigned long long int")
check_types(off_t size_t ssize_t)
if(LONG_DOUBLE GREATER DOUBLE)
    set(HAVE_LONG_DOUBLE_WIDER TRUE)
endif()

####################
# Check for headers
check_headers(
    dlfcn.h
    inttypes.h
    memory.h
    stdint.h
    stdlib.h
    strings.h
    string.h
    sys/stat.h
    sys/time.h
    sys/types.h
    unistd.h)
set(STDC_HEADERS 1)

include(CheckSymbolExists)
####################
# Check for functions
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
check_symbol_exists(clock_gettime sys/time.h HAVE_CLOCK_GETTIME)
####################
# Check for options
option(ENABLE_PTHREAD "use pthreads" ON)
set(HAVE_PTHREADS ${ENABLE_PTHREAD})
option(ENABLE_UNITTEST "enable unittest" ON)
option(ENABLE_CLOCK_GETTIME "enable clock_gettime()" OFF)
option(ENABLE_GETTIMEOFDAY "enable gettimeofday()" ON)

####################
# Check for variables and targets
include(cmake/onnc_target.cmake)
enum_onnc_targets()

set(ONNC_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/lib)


####################
# OUTPUT
configure_file(include/onnc/Support/DataTypes.h.cmake.in include/onnc/Support/DataTypes.h)
configure_file(include/onnc/Config/Config.h.cmake.in include/onnc/Config/Config.h)
configure_file(include/onnc/Config/Platforms.def.in include/onnc/Config/Platforms.def @ONLY)
configure_file(include/onnc/Config/Backends.def.in include/onnc/Config/Backends.def @ONLY)

####################
# Other
set(ANDROID_CPPFLAGS -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address)
add_compile_options(${ANDROID_CPPFLAGS})
add_subdirectory(lib)
add_subdirectory(tools)
