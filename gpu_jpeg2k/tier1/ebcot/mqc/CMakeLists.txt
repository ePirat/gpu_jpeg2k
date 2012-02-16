set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS})

SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/timer.c
${CMAKE_CURRENT_SOURCE_DIR}/mqc_data.c
${CMAKE_CURRENT_SOURCE_DIR}/mqc_opj_helper.c
${CMAKE_CURRENT_SOURCE_DIR}/mqc_configuration.cu
${CMAKE_CURRENT_SOURCE_DIR}/mqc_develop.cu
${CMAKE_CURRENT_SOURCE_DIR}/mqc_wrapper.cpp 
PARENT_SCOPE 
)

cuda_add_library(mqc
	timer.c 
	mqc_data.c
	mqc_opj_helper.c
	mqc_configuration.cu
	mqc_develop.cu
	mqc_wrapper.cpp
	)

TARGET_LINK_LIBRARIES(mqc 
	misc
	#openjpeg
)