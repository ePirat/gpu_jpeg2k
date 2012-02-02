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

static void init_img(type_image *img, Config *config) {
	img->width = config->img_w;
	img->height = config->img_h;

	img->tile_w = config->tile_w;
	img->tile_h = config->tile_h;

	img->num_components = config->num_comp;
	img->depth = config->depth;
	img->sign = config->sign;
	img->num_range_bits = img->depth / img->num_components;

	img->wavelet_type = config->wavelet_type;
	img->num_dlvls = config->num_dlvls;
	img->use_mct = config->use_mct;

	img->use_part2_mct = 0;
	img->mct_compression_method = 0;
	img->coding_style = CODING_STYLE;
	img->prog_order = LY_RES_COMP_POS_PROG;
	img->num_layers = NUM_LAYERS;

	init_coding_params(img, config);
	init_tiles(&img);
}

void encode(void **img_data, Config *config, Chunk **blocks, Chunk **order) {
	type_image *img = (type_image *)allocator(sizeof(type_image));

	init_img(img, config);
}
