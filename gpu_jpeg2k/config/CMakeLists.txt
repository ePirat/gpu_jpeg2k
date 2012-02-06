SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/parameters.c
${CMAKE_CURRENT_SOURCE_DIR}/ini.c
${CMAKE_CURRENT_SOURCE_DIR}/arguments.c
${CMAKE_CURRENT_SOURCE_DIR}/init_device.c
PARENT_SCOPE 
)

add_library(config
parameters.c
ini.c
arguments.c
init_device.c)
