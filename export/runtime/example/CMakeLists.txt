project(onnc-runtime-example LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
if(MKLDNN)
	set(MKLDNN_LIBRARY_PATH /usr/local/lib64)
	link_directories(${MKLDNN_LIBRARY_PATH})
endif()

add_executable(inference
  client-app.c  
  client-lib.c
  onnc-runtime-service.c
  onnc-runtime-core.c
)
target_link_libraries(inference
	operators
)
if(MKLDNN)
	target_link_libraries(inference
		mkldnn
	)
endif()
