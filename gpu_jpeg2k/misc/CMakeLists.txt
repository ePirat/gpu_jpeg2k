set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS})

cuda_add_library(misc SHARED
cuda_errors.cu
memory_management.cu
)