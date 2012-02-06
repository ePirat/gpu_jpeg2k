SET(JPEG2K_SRCS 
${JPEG2K_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/codestream.c
${CMAKE_CURRENT_SOURCE_DIR}/codestream_mct.c
${CMAKE_CURRENT_SOURCE_DIR}/tag_tree_encode.c
${CMAKE_CURRENT_SOURCE_DIR}/write_codestream.c
${CMAKE_CURRENT_SOURCE_DIR}/buffer.c
PARENT_SCOPE 
)

cuda_add_library(tier2
codestream.c
codestream_mct.c
tag_tree_encode.c
write_codestream.c
buffer.c
)

TARGET_LINK_LIBRARIES(tier2
m
misc
types
tier1
)
