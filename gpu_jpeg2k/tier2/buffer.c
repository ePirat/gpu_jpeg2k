/*
 * buffer.c
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */

#include "buffer.h"

void init_dec_buffer(Chunk *img_data, type_buffer *src_buff) {
	mem_mg_t *mem_mg = src_buff->mem_mg;

	src_buff->data = (uint8_t *)mem_mg->alloc->host(img_data->length, mem_mg->ctx);
	src_buff->size = img_data->length;

	cuda_memcpy_hth(img_data->data, src_buff->data, img_data->length);

	src_buff->start = src_buff->data;
	src_buff->end = src_buff->data + src_buff->size;
	src_buff->bp = src_buff->data;
	src_buff->bits_count = 0;
	src_buff->byte = 0;
}
