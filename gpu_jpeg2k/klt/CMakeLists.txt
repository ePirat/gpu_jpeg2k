SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/klt.cu
${CMAKE_CURRENT_SOURCE_DIR}/gs.cu
${CMAKE_CURRENT_SOURCE_DIR}/analysis.cu
${CMAKE_CURRENT_SOURCE_DIR}/adjust.cu
${CMAKE_CURRENT_SOURCE_DIR}/mct_transform.cu
${CMAKE_CURRENT_SOURCE_DIR}/blocks.c
${CMAKE_CURRENT_SOURCE_DIR}/reduce.cu
${CMAKE_CURRENT_SOURCE_DIR}/mean_adjust_data.cu
${CMAKE_CURRENT_SOURCE_DIR}/shift.cu
${CMAKE_CURRENT_SOURCE_DIR}/calc_cov.cu
${CMAKE_CURRENT_SOURCE_DIR}/calc_cov_mat.cu
PARENT_SCOPE 
)

cuda_add_library(klt
	klt.cu
	gs.cu
	analysis.cu
	adjust.cu
	mct_transform.cu
	blocks.c
	reduce.cu
	mean_adjust_data.cu
	shift.cu
	calc_cov.cu
	calc_cov_mat.cu
)

target_link_libraries(klt
	m
)

cuda_add_cublas_to_target(klt)
