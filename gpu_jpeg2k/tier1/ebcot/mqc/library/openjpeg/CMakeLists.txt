set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS})

SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/bio.c 
${CMAKE_CURRENT_SOURCE_DIR}/cio.c 
${CMAKE_CURRENT_SOURCE_DIR}/dwt.c 
${CMAKE_CURRENT_SOURCE_DIR}/event.c 
${CMAKE_CURRENT_SOURCE_DIR}/image.c 
${CMAKE_CURRENT_SOURCE_DIR}/j2k.c 
${CMAKE_CURRENT_SOURCE_DIR}/j2k_lib.c 
${CMAKE_CURRENT_SOURCE_DIR}/jp2.c 
${CMAKE_CURRENT_SOURCE_DIR}/jpt.c 
${CMAKE_CURRENT_SOURCE_DIR}/mct.c
${CMAKE_CURRENT_SOURCE_DIR}/mqc.c 
${CMAKE_CURRENT_SOURCE_DIR}/openjpeg.c 
${CMAKE_CURRENT_SOURCE_DIR}/pi.c 
${CMAKE_CURRENT_SOURCE_DIR}/raw.c 
${CMAKE_CURRENT_SOURCE_DIR}/t1.c 
${CMAKE_CURRENT_SOURCE_DIR}/t2.c 
${CMAKE_CURRENT_SOURCE_DIR}/tcd.c 
${CMAKE_CURRENT_SOURCE_DIR}/tgt.c 
${CMAKE_CURRENT_SOURCE_DIR}/opj_convert.c 
PARENT_SCOPE 
)

cuda_add_library(openjpeg
	bio.c 
	cio.c 
	dwt.c 
	event.c 
	image.c 
	j2k.c 
	j2k_lib.c 
	jp2.c 
	jpt.c 
	mct.c
    mqc.c 
    openjpeg.c 
    pi.c 
    raw.c 
    t1.c 
    t2.c 
    tcd.c 
    tgt.c 
    opj_convert.c
)

TARGET_LINK_LIBRARIES(openjpeg 

)