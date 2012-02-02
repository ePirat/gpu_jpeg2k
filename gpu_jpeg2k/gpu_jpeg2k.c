/*
 * gpu_jpeg2k.c
 *
 *  Created on: Feb 2, 2012
 *      Author: miloszc
 */

#include "gpu_jpeg2k.h"
#include "tier2/markers.h"
#include "types/image.h"

static void init_coding_params(type_image *img, Config *config) {
	img->coding_param = (type_coding_param *)allocator(sizeof(type_coding_param));
	img->coding_param->target_size = config->target_size;
	img->coding_param->imgarea_tlx = 0;
	img->coding_param->imgarea_tly = 0;
	img->coding_param->imgarea_width = img->width;
	img->coding_param->imgarea_height = img->height;
	img->coding_param->tilegrid_tlx = 0;
	img->coding_param->tilegrid_tly = 0;
	img->coding_param->comp_step_x = 1;
	img->coding_param->comp_step_y = 1;
	img->coding_param->base_step = 1.0 / (float) (1 << (img->num_range_bits - 1));
}

static void init_img_data(type_image *img, void **img_data) {
	mem_mg_t *mem_mg = img->mem_mg;
	int t, c;

	for (t = 0; t < img->num_tiles; ++t) {
		type_tile *tile = &(img->tile[t]);
		for (c = 0; c < img->num_components; ++c) {
			type_tile_comp *tile_comp = &(tile->tile_comp[c]);
			tile_comp->img_data_d = (type_data *)img_data[c];
		}
	}
}

static void init_img(type_image *img, void **img_data, Config *config) {
	img->width = config->img_w;
	img->height = config->img_h;

	img->tile_w = config->tile_w;
	img->tile_h = config->tile_h;

	img->num_components = config->num_comp;
	img->depth = config->depth;
	img->sign = config->sign;
	img->num_range_bits = img->depth / img->num_components;

	img->wavelet_type = config->wavelet_type;
	img->num_dlvls = config->num_dlvls;\

	img->mem_mg = config->mem_mg;

	img->use_mct = config->use_mct;
	img->use_part2_mct = 0;
	img->mct_compression_method = 0;
	img->mct_klt_iterations = 0;
	img->mct_klt_err = 0.0f;
	img->mct_klt_border_eigenvalue = 0.0f;
	img->coding_style = CODING_STYLE;
	img->prog_order = LY_RES_COMP_POS_PROG;
	img->num_layers = NUM_LAYERS;

	init_coding_params(img, config);
	init_tiles(&img);
	init_img_data(img, img_data);
}

void encode(void **img_data, Config *config, Chunk **blocks, Chunk **order) {
	mem_mg_t *mem_mg = config->mem_mg;
	type_image *img = (type_image *)mem_mg->alloc->host(sizeof(type_image), mem_mg->ctx);

	init_img(img, img_data, config);

	mct(img);

	int i = 0;
	// Do processing for all tiles
	for(i = 0; i < img->num_tiles; ++i)
	{
		type_tile *tile = &(img->tile[i]);

		// Do forward wavelet transform
		fwt(tile);

		// Divide subbands to codeblocks, than quantize data
		quantize_tile(tile);

		// EBCOT + PCRD
		encode_tile(tile);
	}
	// Write Codestream
	blocks = write_codestream(img);
//	free(img);
}
