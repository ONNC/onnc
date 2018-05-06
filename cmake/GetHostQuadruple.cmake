# Returns the host quadruple.
# Invokes config.guess

set(config_guess ${CMAKE_CURRENT_LIST_DIR}/config.guess)
function( get_host_quadruple var )
  if( MSVC )
    if( CMAKE_CL_64 )
      set( value "x86_64-pc-win32" )
    else()
      set( value "i686-pc-win32" )
    endif()
  elseif( MINGW AND NOT MSYS )
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      set( value "x86_64-w64-mingw32" )
    else()
      set( value "i686-pc-mingw32" )
    endif()
  else( MSVC )
    execute_process(COMMAND sh ${config_guess}
      RESULT_VARIABLE TT_RV
      OUTPUT_VARIABLE TT_OUT
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if( NOT TT_RV EQUAL 0 )
      message(FATAL_ERROR "Failed to execute ${config_guess}")
    endif( NOT TT_RV EQUAL 0 )
    set( value ${TT_OUT} )
  endif( MSVC )
  set( ${var} ${value} PARENT_SCOPE )
endfunction( get_host_quadruple var )
