/* 
Copyright 2009-2013 Poznan Supercomputing and Networking Center

Authors:
Milosz Ciznicki miloszc@man.poznan.pl

GPU JPEG2K is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GPU JPEG2K is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with GPU JPEG2K. If not, see <http://www.gnu.org/licenses/>.
*/
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
