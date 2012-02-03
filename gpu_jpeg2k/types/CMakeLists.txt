set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS})
set(CMAKE_CC_FLAGS ${CMAKE_CC_FLAGS})

add_library(types
buffered_stream.c
image_hyper.c
image_bsq.c
image_bip.c
image_bil.c
image_ordinary.c
image.c
)
