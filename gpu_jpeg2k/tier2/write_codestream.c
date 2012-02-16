/*
 * write_codestream.c
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */
#include <stdlib.h>
#include <stdio.h>

#include "write_codestream.h"
#include "../include/gpu_jpeg2k.h"

void write_codestream(type_image *img, Chunk **blocks, Chunk **order) {
	mem_mg_t *mem_mg = img->mem_mg;
	type_buffer *buffer = (type_buffer *)mem_mg->alloc->host(sizeof(type_buffer), mem_mg->ctx);

	buffer->mem_mg = mem_mg;
	init_buffer(buffer);
	encode_codestream(buffer, img);

	// Currently one chunk
	*blocks = (Chunk *)mem_mg->alloc->host(sizeof(Chunk), mem_mg->ctx);
	(*blocks)->data = buffer->data;
	(*blocks)->length = buffer->bytes_count;
	*order = *blocks;
	// TODO
	mem_mg->dealloc->host(buffer, mem_mg->ctx);
}
