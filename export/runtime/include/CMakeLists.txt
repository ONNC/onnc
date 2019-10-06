project(onnc-runtime-example LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MKLDNN)
	message("Compiling MKL-DNN Version Operators")
	add_definitions(-DMKLDNN)
endif()
add_library(operators
	operators.c  
)
target_link_libraries(operators
	m
)
