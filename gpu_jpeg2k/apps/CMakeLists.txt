CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../CMake" "${PROJECT_SOURCE_DIR}/../CMake/cuda" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(CUDA)
FIND_PACKAGE(FreeImage)

#Uncomment next line to enable debugging (for use with cuda-gdb)
#SET(CUDA_NVCC_FLAGS -g -G --compiler-options -fpermissive)
SET(CMAKE_CXX_FLAGS "-O3 -g -fno-builtin")
SET(CMAKE_CC_FLAGS "-O3 -g -fno-builtin")
SET(CUDA_NVCC_FLAGS -O3 -g -G --compiler-options -fpermissive -arch sm_20 --maxrregcount 64 --ptxas-options=-v)
SET(CUDA_VERBOSE_BUILD ON)

include_directories(${FREEIMAGE_INCLUDE_PATH})

find_library(GPUJPEG2K_LIBRARY
		NAMES 
			gpu_jpeg2k
		PATHS
			${PROJECT_SOURCE_DIR}/../build
)

message(STATUS )

cuda_add_executable(enc_main
../print_info/print_info.c
func.cu
enc_main.c)

TARGET_LINK_LIBRARIES(enc_main 
${FREEIMAGE_LIBRARY} 
m
${GPUJPEG2K_LIBRARY})

cuda_add_executable(dec_main
../print_info/print_info.c
func.cu
dec_main.c)

TARGET_LINK_LIBRARIES(dec_main 
${FREEIMAGE_LIBRARY} 
m
${GPUJPEG2K_LIBRARY})
