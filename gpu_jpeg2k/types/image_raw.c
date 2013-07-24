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
 * image_raw.c
 *
 *  Created on: Feb 6, 2012
 *      Author: miloszc
 */

#include "image_raw.h"
#include "../print_info/print_info.h"

void save_raw(type_image *img, Chunk **blocks, Chunk **order) {
	mem_mg_t *mem_mg = img->mem_mg;
	uint8_t **comp_data = (uint8_t **)mem_mg->alloc->host(sizeof(uint8_t *) * img->num_components, mem_mg->ctx);

	*blocks = (Chunk *)mem_mg->alloc->host(sizeof(Chunk) * img->num_components, mem_mg->ctx);
	int i = 0, c = 0, x = 0, y = 0;
	for (i = 0; i < img->num_tiles; ++i) {
		type_tile *tile = &(img->tile[i]);
		for (c = 0; c < img->num_components; ++c) {
			type_tile_comp *tile_comp = &(tile->tile_comp[c]);
			tile_comp->img_data = (type_data *)mem_mg->alloc->host(tile_comp->width * tile_comp->height * sizeof(type_data), mem_mg->ctx);
			cuda_memcpy_dth(tile_comp->img_data_d, tile_comp->img_data, tile_comp->width * tile_comp->height * sizeof(type_data));
			comp_data[i] = (uint8_t *)mem_mg->alloc->host(tile_comp->width * tile_comp->height * sizeof(uint8_t), mem_mg->ctx);
			for (x = 0; x < tile_comp->width; ++x) {
				for (y = 0; y < tile_comp->height; ++y) {
					comp_data[i][y * tile_comp->width + x] = (uint8_t) tile_comp->img_data[y * tile_comp->width + x];
				}
			}
			mem_mg->dealloc->host(tile_comp->img_data, mem_mg->ctx);
			mem_mg->dealloc->dev(tile_comp->img_data_d, mem_mg->ctx);
			(*blocks)[c].length = tile_comp->width * tile_comp->height * sizeof(uint8_t);
			(*blocks)[c].data = (void *)(comp_data[i]);
		}
	}
//	mem_mg->dealloc->host(comp_data, mem_mg->ctx);
	*order = *blocks;
}
