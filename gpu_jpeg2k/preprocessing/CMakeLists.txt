set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS})

SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/preprocess_gpu.cu
${CMAKE_CURRENT_SOURCE_DIR}/mct.c
PARENT_SCOPE 
)

cuda_add_library(preprocessing
preprocess_gpu.cu
mct.c
)

target_link_libraries(preprocessing
	klt
	dwt
)