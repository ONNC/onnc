include(check_helper)

####################
#  Check canonical system
if (UNIX)
  set(ONNC_ON_UNIX 1)
elseif(WIN32)
  set(ONNC_ON_WIN32 1)
elseif(APPLE)
  set(ONNC_ON_APPLE 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(ONNC_ON_LINUX 1)
endif()

####################
#  Check quardruple
if( MSVC )
  if( CMAKE_CL_64 )
    set(ONNC_DEFAULT_TARGET_QUADRUPLE "x86_64-pc-win32")
  else()
    set(ONNC_DEFAULT_TARGET_QUADRUPLE "i686-pc-win32")
  endif()
elseif( MINGW AND NOT MSYS )
  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(ONNC_DEFAULT_TARGET_QUADRUPLE "x86_64-w64-mingw32")
  else()
    set(ONNC_DEFAULT_TARGET_QUADRUPLE "i686-pc-mingw32")
  endif()
else( MSVC )
  execute_process(COMMAND sh ${CMAKE_CURRENT_LIST_DIR}/config.guess
    RESULT_VARIABLE TT_RV
    OUTPUT_VARIABLE TT_OUT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if( NOT TT_RV EQUAL 0 )
    message(FATAL_ERROR "Failed to execute ${CMAKE_CURRENT_LIST_DIR}/config.guess")
  endif( NOT TT_RV EQUAL 0 )
  set(ONNC_DEFAULT_TARGET_QUADRUPLE ${TT_OUT})
endif( MSVC )

####################
#  Check STL hash
message(STATUS "Looking for unordered/hash map/set headers")
foreach(NAMESPACE_I std std::tr1)
    foreach(LOCATION_I unordered_map tr1/unordered_map)
        try_compile_code(
            HEAD "#include <${LOCATION_I}>" 
            CODE "${NAMESPACE_I}::unordered_map<int, int> t; return t.find(5) == t.end();" 
            RESULT SUCCEED)
        if (SUCCEED)
            set(LOCATION "${LOCATION_I}")
            break()
        endif()
    endforeach()
    if (SUCCEED)
        set(NAMESPACE "${NAMESPACE_I}")
        set(CLASSNAME "unordered_map")
        break()
    endif()
endforeach()
if (NOT SUCCEED)
    foreach(NAMESPACE_I __gnu_cxx "" std stdext)
        foreach(LOCATION_I ext/hash_map hash_map)
            try_compile_code(
                HEAD "#include <${LOCATION_I}>" 
                CODE "${NAMESPACE_I}::hash_map<int, int> t;" 
                RESULT SUCCEED)
            if (SUCCEED)
                set(LOCATION "${LOCATION_I}")
                break()
            endif()
        endforeach()
        if (SUCCEED)
            set(NAMESPACE "${NAMESPACE_I}")
            set(CLASSNAME "hash_map")
            break()
        endif()
    endforeach()
endif()
if (SUCCEED)
    set(HASH_NAMESPACE "${NAMESPACE}" CACHE INTERNAL "")
    set(HAVE_HASH_MAP 1 CACHE INTERNAL "")
    set(HASH_MAP_H "${LOCATION}" CACHE INTERNAL "")
    set(HASH_MAP_CLASS "${CLASSNAME}" CACHE INTERNAL "")
    string(REPLACE "map" "set" CLASSNAME ${CLASSNAME})
    string(REPLACE "map" "set" LOCATION ${LOCATION})
    set(HAVE_HASH_SET 1 CACHE INTERNAL "")
    set(HASH_SET_H "${LOCATION}" CACHE INTERNAL "")
    set(HASH_SET_CLASS "${CLASSNAME}" CACHE INTERNAL "")
endif ()

####################
#  Check types
check_types(int8_t int16_t int32_t int64_t intptr_t)
check_types(uint8_t uint16_t uint32_t uint64_t uintptr_t)
check_types(double "long double" "long long" "long long int" "unsigned long long int")
check_types(off_t size_t ssize_t)
if(LONG_DOUBLE GREATER DOUBLE)
    set(HAVE_LONG_DOUBLE_WIDER TRUE)
endif()

####################
# Check libraries
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
    unistd.h
)

####################
# Check for functions
include(CheckSymbolExists)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
check_symbol_exists(clock_gettime sys/time.h HAVE_CLOCK_GETTIME)