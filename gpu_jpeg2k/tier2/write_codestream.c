/*
 * write_codestream.c
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */
#include <stdlib.h>
#include <stdio.h>

#include "write_codestream.h"
#include "../gpu_jpeg2k.h"

Chunk **write_codestream(type_image *img) {
	mem_mg_t *mem_mg = img->mem_mg;
	type_buffer *buffer = (type_buffer *)mem_mg->alloc->host(sizeof(type_buffer), mem_mg->ctx);

	buffer->mem_mg = mem_mg;
	init_buffer(buffer);
	encode_codestream(buffer, img);

	// Currently one chunk
	Chunk **chunks = (Chunk **)mem_mg->alloc->host(sizeof(Chunk *), mem_mg->ctx);
	chunks[0] = (Chunk *)mem_mg->alloc->host(sizeof(Chunk), mem_mg->ctx);
	chunks[0]->data = buffer->data;
	chunks[0]->length = buffer->bytes_count;
	// TODO
//	mem_mg->dealloc->host(buffer);
	return chunks;
}
