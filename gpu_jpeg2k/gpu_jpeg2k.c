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

#include "include/gpu_jpeg2k.h"
#include "tier2/markers.h"
#include "types/image.h"
#include "types/image_raw.h"
#include "print_info/print_info.h"

static void init_coding_params(type_image *img, Config *config) {
	mem_mg_t *mem_mg = img->mem_mg;
	img->coding_param = (type_coding_param *)mem_mg->alloc->host(sizeof(type_coding_param), mem_mg->ctx);
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

static void deinit_coding_params(type_image *img) {
	mem_mg_t *mem_mg = img->mem_mg;
	mem_mg->dealloc->host(img->coding_param, mem_mg->ctx);
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
	img->num_dlvls = config->num_dlvls;

	img->mem_mg = config->mem_mg;

	img->use_mct = config->use_mct;
	img->cblk_exp_w = config->cblk_exp_w;
	img->cblk_exp_h = config->cblk_exp_h;
	img->use_part2_mct = 0;
	img->mct_compression_method = 0;
	img->mct_klt_iterations = 0;
	img->mct_klt_err = 0.0f;
	img->mct_klt_border_eigenvalue = 0.0f;
	img->coding_style = CODING_STYLE;
	img->prog_order = LY_RES_COMP_POS_PROG;
	img->num_layers = NUM_LAYERS;

	img->codestream = NULL;

	init_coding_params(img, config);
	init_tiles(&img);
	init_img_data(img, img_data);
}

static void deinit_img(type_image *img) {
	mem_mg_t *mem_mg = img->mem_mg;
	deinit_tiles(img);
	deinit_coding_params(img);
	mem_mg->dealloc->host(img, mem_mg->ctx);
}

void encode(void **img_data, Config *config, Chunk **blocks, Chunk **order) {
	mem_mg_t *mem_mg = config->mem_mg;
	type_image *img = (type_image *)mem_mg->alloc->host(sizeof(type_image), mem_mg->ctx);

//	long int start_init = start_measure();
	init_img(img, img_data, config);
//	printf("Init: %d\n", stop_measure(start_init));
	
//	long int start_mct = start_measure();
	enc_mct(img);
//	printf("MCT: %d\n", stop_measure(start_mct));

	int i = 0;
	// Do processing for all tiles
	for(i = 0; i < img->num_tiles; ++i)
	{
		type_tile *tile = &(img->tile[i]);

//		long int start_fwt = start_measure();
		// Do forward wavelet transform
		fwt(tile);
//		printf("FWT: %d\n", stop_measure(start_fwt));

//		long int start_q = start_measure();
		// Divide subbands to codeblocks, than quantize data
		quantize_tile(tile);
//		printf("Q: %d\n", stop_measure(start_q));

//		long int start_ebcot = start_measure();
		// EBCOT + PCRD
		encode_tile(tile);
//		printf("EBCOT: %d\n", stop_measure(start_ebcot));
	}
//	long int start_w = start_measure();
	// Write Codestream
	write_codestream(img, blocks, order);
//	printf("W: %d\n", stop_measure(start_w));

//	long int start_deinit = start_measure();
	deinit_img(img);
//	printf("Deinit: %d\n", stop_measure(start_deinit));
//	free(img);
}

static void fill_config(type_image *img, Config *config) {
	config->img_w = img->width;
	config->img_h = img->height;
	config->num_comp = img->num_components;
	config->num_dlvls = img->num_dlvls;
	config->sign = img->sign;
	config->wavelet_type = img->wavelet_type;
	config->tile_w = img->tile_w;
	config->tile_h = img->tile_h;
}

static void alloc_img_data(type_image *img) {
	mem_mg_t *mem_mg = img->mem_mg;
	int t, c;

	for (t = 0; t < img->num_tiles; ++t) {
		type_tile *tile = &(img->tile[t]);
		for (c = 0; c < img->num_components; ++c) {
			type_tile_comp *tile_comp = &(tile->tile_comp[c]);
			tile_comp->img_data_d = (type_data *)mem_mg->alloc->dev(sizeof(type_data) * tile_comp->width * tile_comp->height, mem_mg->ctx);
		}
	}
}

void decode(Chunk *img_data, Config *config, Chunk **blocks, Chunk **order) {
	mem_mg_t *mem_mg = config->mem_mg;
	type_buffer *src_buff = (type_buffer *)mem_mg->alloc->host(sizeof(type_buffer), mem_mg->ctx);
	src_buff->mem_mg = mem_mg;

//	long int start_init = start_measure();
	init_dec_buffer(img_data, src_buff);
//	printf("start_init: %d\n", stop_measure(start_init));

	type_image *img = (type_image *)mem_mg->alloc->host(sizeof(type_image), mem_mg->ctx);
	img->mem_mg = mem_mg;
//	long int dec_cod = start_measure();
	decode_codestream(src_buff, img);
	alloc_img_data(img);
//	printf("dec_cod: %d\n", stop_measure(dec_cod));

	int i = 0;
	// Do decoding for all tiles
	for(i = 0; i < img->num_tiles; ++i)	{
		type_tile *tile = &(img->tile[i]);
//		long int dec_tile = start_measure();
		/* Decode data */
		decode_tile(tile);
//		printf("dec_tile: %d\n", stop_measure(dec_tile));
//		long int deq_tile = start_measure();
		/* Dequantize data */
		dequantize_tile(tile);
//		printf("deq_tile: %d\n", stop_measure(deq_tile));
//		long int iwt_tile = start_measure();
		/* Do inverse wavelet transform */
		iwt(tile);
//		printf("iwt_tile: %d\n", stop_measure(iwt_tile));
	}
//	long int _mct = start_measure();
	dec_mct(img);
//	printf("dec_mct: %d\n", stop_measure(_mct));

//	long int save_img = start_measure();
	fill_config(img, config);
//	save_image(img);
	save_raw(img, blocks, order);
//	printf("save_img: %d\n", stop_measure(save_img));
//	free(img);
}
