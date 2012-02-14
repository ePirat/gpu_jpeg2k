CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../CMake" "${PROJECT_SOURCE_DIR}/../CMake/cuda" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(CUDA)
FIND_PACKAGE(FreeImage)

SET(CMAKE_CXX_FLAGS "-Wall -O3")
SET(CMAKE_CC_FLAGS "-Wall -O3")
SET(CUDA_VERBOSE_BUILD ON)

include_directories(${FREEIMAGE_INCLUDE_PATH})

find_library(GPUJPEG2K_LIBRARY
		NAMES 
			gpu_jpeg2k
		PATHS
			${PROJECT_SOURCE_DIR}/../build
)

cuda_add_executable(enc_main
../print_info/print_info.c
common.c
func.c
list.c
enc_main.c)

TARGET_LINK_LIBRARIES(enc_main 
${FREEIMAGE_LIBRARY} 
m
${GPUJPEG2K_LIBRARY})

cuda_add_executable(dec_main
../print_info/print_info.c
common.c
func.c
list.c
dec_main.c)

TARGET_LINK_LIBRARIES(dec_main 
${FREEIMAGE_LIBRARY} 
m
${GPUJPEG2K_LIBRARY})
