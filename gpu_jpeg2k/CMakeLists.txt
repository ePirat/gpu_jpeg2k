CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" "${PROJECT_SOURCE_DIR}/CMake/cuda" ${CMAKE_MODULE_PATH})
FIND_PACKAGE(CUDA)
FIND_PACKAGE(FreeImage)

#Uncomment next line to enable debugging (for use with cuda-gdb)
#SET(CUDA_NVCC_FLAGS -g -G --compiler-options -fpermissive)
SET(CMAKE_CXX_FLAGS "-O3 -g -fno-builtin")
SET(CMAKE_CC_FLAGS "-O3 -g -fno-builtin")
SET(CUDA_NVCC_FLAGS -O3 -g -G --compiler-options -fpermissive -arch sm_20 --maxrregcount 64 --ptxas-options=-v)
SET(CUDA_VERBOSE_BUILD ON)

include_directories(${FREEIMAGE_INCLUDE_PATH})

add_subdirectory(tier1/coeff_coder)
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

cuda_add_library(gpu_jpeg2k SHARED
print_info/print_info.c
gpu_jpeg2k.c
)

TARGET_LINK_LIBRARIES(gpu_jpeg2k 
${FREEIMAGE_LIBRARY} 
m
gpu_coeff_coder 
config
dwt
klt
preprocessing
misc
tier1
tier2
types)

cuda_add_executable(enc_main
print_info/print_info.c
func.cu
enc_main.c)

TARGET_LINK_LIBRARIES(enc_main 
${FREEIMAGE_LIBRARY} 
m
gpu_jpeg2k)

cuda_add_executable(dec_main
print_info/print_info.c
func.cu
dec_main.c)

TARGET_LINK_LIBRARIES(dec_main 
${FREEIMAGE_LIBRARY} 
m
gpu_jpeg2k)
