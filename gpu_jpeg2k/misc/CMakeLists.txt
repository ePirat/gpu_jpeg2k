SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/cuda_errors.cu
${CMAKE_CURRENT_SOURCE_DIR}/memory_management.cu
PARENT_SCOPE 
)

cuda_add_library(misc
cuda_errors.cu
memory_management.cu
)