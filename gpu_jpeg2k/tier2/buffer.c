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
