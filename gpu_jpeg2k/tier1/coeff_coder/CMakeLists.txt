#"--ptxas-options=-dlcm=cg"

SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/gpu_coeff_coder.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/gpu_coeff_coder2.cu
${CMAKE_CURRENT_SOURCE_DIR}/coeff_coder_pcrd.cu
PARENT_SCOPE 
)

cuda_add_library(gpu_coeff_coder
	gpu_coeff_coder.cpp 
	gpu_coeff_coder2.cu
	coeff_coder_pcrd.cu
)

TARGET_LINK_LIBRARIES(gpu_coeff_coder 
	misc
)
