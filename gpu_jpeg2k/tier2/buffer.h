/*
 * buffer.h
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include "../include/gpu_jpeg2k.h"
#include "../types/buffered_stream.h"

void init_dec_buffer(Chunk *img_data, type_buffer *src_buff);

#endif /* BUFFER_H_ */
