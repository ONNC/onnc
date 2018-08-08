cmake_minimum_required(VERSION 3.5)
project(onnc)
enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ONNC_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
file(READ ${ONNC_ROOT_DIR}/VERSION VERSION)
set(PACKAGE_VERSION ${VERSION})
set(PACKAGE "onnc")
set(PACKAGE_BUGREPORT "maillist.onnc.ai")
set(PACKAGE_NAME "ONNC")
set(PACKAGE_STRING "ONNC ${VERSION}")
set(PACKAGE_TARNAME "onnc")
set(PACKAGE_URL "onnc.ai")

####################
#  Options
include(option)
option_enum(NAME CMAKE_BUILD_TYPE HELP "Choose the type of build" VALUE Debug Release Regression Optimized)
option(ENABLE_PTHREAD "use pthreads" ON)
set(HAVE_PTHREADS ${ENABLE_PTHREAD})
option(ENABLE_UNITTEST "enable unittest" ON)
option(ENABLE_CLOCK_GETTIME "enable clock_gettime()" OFF)
option(ENABLE_GETTIMEOFDAY "enable gettimeofday()" ON)

####################
#  Languages
set(STDC_HEADERS 1)
set(CMAKE_CXX_STANDARD 14)
set(HAVE_CXX14 TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3"          CACHE STRING "Flags used by the CXX compiler during DEBUG builds.")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG"     CACHE STRING "Flags used by the CXX compiler during RELEASE builds.")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g3" CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds.")
option(ENABLE_WERROR "Compile with -Werror enabled (default is OFF)" OFF)
if (ENABLE_WERROR)
    add_compile_options(-Werror)
endif()
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-variadic-macros WNO_VARIADIC_MACROS)
set(ANDROID_CPPFLAGS -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address)
add_compile_options(${ANDROID_CPPFLAGS})

####################
#  Check
include(check)

####################
#  Targets
file(STRINGS ${ONNC_ROOT_DIR}/include/onnc/Config/Config.h.cmake.in CONFIG_IN_CONTENT)
string(REGEX MATCHALL "ENABLE_[A-Za-z0-9_]*_TARGET" CONFIG_IN_TARGETS ${CONFIG_IN_CONTENT})
foreach(EACH_TARGET IN LISTS CONFIG_IN_TARGETS)
    # Get target name
    string(LENGTH ${EACH_TARGET} EACH_LEN)
    math(EXPR SUBSTR_LEN "${EACH_LEN} - 14")
    string(SUBSTRING ${EACH_TARGET} 7 ${SUBSTR_LEN} EACH_TARGET_NAME)
    # Set option
    option(${EACH_TARGET} "Enable ${EACH_TARGET_NAME}" ON)
endforeach(EACH_TARGET IN CONFIG_IN_TARGETS)

####################
#  External
# TODO:
include(external)

set(ONNC_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(SKYPAT_INSTALL_DIR ${ONNC_BINARY_DIR} CACHE PATH "skypat install path")
set(SKYPAT_INCLUDE_DIRS
    ${SKYPAT_INSTALL_DIR}/include)
set(SKYPAT_LIBRARIES
    ${SKYPAT_INSTALL_DIR}/lib/libskypat.a)

set(ONNX_INSTALL_DIR ${ONNC_BINARY_DIR} CACHE PATH "onnx install path")
set(ONNX_INCLUDE_DIRS
    ${ONNX_INSTALL_DIR}/include)
set(ONNX_LIBRARIES
    ${ONNX_INSTALL_DIR}/lib/libonnx.a
    ${ONNX_INSTALL_DIR}/lib/libonnx_proto.a)
    
####################
# OUTPUT
configure_file(include/onnc/Support/DataTypes.h.cmake.in include/onnc/Support/DataTypes.h)
configure_file(include/onnc/Config/Config.h.cmake.in include/onnc/Config/Config.h)
#configure_file(include/onnc/Config/Platforms.def.in include/onnc/Config/Platforms.def @ONLY)
#configure_file(include/onnc/Config/Backends.def.in include/onnc/Config/Backends.def @ONLY)

####################
# Other
#add_custom_target(code_gen)
#add_subdirectory(lib)
#add_subdirectory(tools)
#add_subdirectory(include)
