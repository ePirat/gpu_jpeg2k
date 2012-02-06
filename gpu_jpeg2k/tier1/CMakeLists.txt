SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/quantization.c
${CMAKE_CURRENT_SOURCE_DIR}/quantizer.cu
${CMAKE_CURRENT_SOURCE_DIR}/dequantizer.cu
PARENT_SCOPE 
)

#message(STATUS "${JPEG2K_SRCS}")

cuda_add_library(tier1
quantization.c
quantizer.cu
dequantizer.cu
)
