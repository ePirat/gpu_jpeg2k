/*
 * write_codestream.h
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */

#ifndef WRITE_CODESTREAM_H_
#define WRITE_CODESTREAM_H_

#include "../types/image_types.h"
#include "../include/gpu_jpeg2k.h"

void write_codestream(type_image *img, Chunk **blocks, Chunk **order);

#endif /* WRITE_CODESTREAM_H_ */
