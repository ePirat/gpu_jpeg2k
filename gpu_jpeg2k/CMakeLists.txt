CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(GPU_JPEG2K)

if(CUDA_ARCH)
	message(STATUS "Using CUDA_ARCH:${CUDA_ARCH}")
else()
	message(STATUS "Empty CUDA_ARCH. Using default sm_20")
	SET(CUDA_ARCH "sm_20")
endif()

string(SUBSTRING ${CUDA_ARCH} 3 2 CUDA_ARCH_NUM)

if(${CUDA_ARCH_NUM} LESS 20)
	SET(MAX_REG_COUNT 64)
else()
	SET(MAX_REG_COUNT 63)
endif()

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" "${PROJECT_SOURCE_DIR}/CMake/cuda" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(CUDA)
FIND_PACKAGE(FreeImage)

SET(CMAKE_CXX_FLAGS "-Wall -O3")
SET(CMAKE_CC_FLAGS "-Wall -O3")
SET(CUDA_NVCC_FLAGS -arch ${CUDA_ARCH} --maxrregcount ${MAX_REG_COUNT})
#SET(CUDA_VERBOSE_BUILD ON)

SET(BUILD_SHARED_LIBS ON)

include_directories(${FREEIMAGE_INCLUDE_PATH})

# Set source files
SET(JPEG2K_SRCS 
print_info/print_info.c
gpu_jpeg2k.c)

#add_subdirectory(tier1/coeff_coder)
add_subdirectory(tier1/ebcot)
add_subdirectory(tier1/ebcot/mqc)
add_subdirectory(tier1/ebcot/mqc/library/openjpeg)
add_subdirectory(config)
add_subdirectory(dwt)
add_subdirectory(preprocessing)
add_subdirectory(misc)
add_subdirectory(tier1)
add_subdirectory(tier2)
add_subdirectory(file_format)
add_subdirectory(file_access)
add_subdirectory(types)
add_subdirectory(klt)

#message(STATUS "${JPEG2K_SRCS}")

cuda_add_library(gpu_jpeg2k
${JPEG2K_SRCS}
)

TARGET_LINK_LIBRARIES(gpu_jpeg2k 
${FREEIMAGE_LIBRARY} 
m)

cuda_add_cublas_to_target(gpu_jpeg2k)
