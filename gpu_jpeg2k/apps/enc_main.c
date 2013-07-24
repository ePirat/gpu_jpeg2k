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
 * main.c
 *
 *  Created on: Feb 2, 2012
 *      Author: miloszc
 */

#include <FreeImage.h>
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include "../include/gpu_jpeg2k.h"
#include "../misc/cuda_errors.h"
#include "../config/parameters.h"
#include "../config/arguments.h"
#include "../config/help.h"
#include "../types/image_types.h"
#include "func.h"

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
static void _FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
   printf("\n*** ");
   if(fif != FIF_UNKNOWN) {
     printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
   }
   printf("%s", message);
   printf(" ***\n");
}

static void init_mem_mg(mem_mg_t *mem_mg) {
	mem_mg->alloc = (alloc_t *)malloc(sizeof(alloc_t));
	mem_mg->alloc->host = _cuda_h_allocate_mem;
	mem_mg->alloc->dev = _cuda_d_allocate_mem;
	mem_mg->dealloc = (dealloc_t *)malloc(sizeof(dealloc_t));
	mem_mg->dealloc->host = _cuda_h_free;
	mem_mg->dealloc->dev = _cuda_d_free;

	ctx_t *ctx = (ctx_t *)malloc(sizeof(ctx_t));
	init_ctx(ctx);
	mem_mg->ctx = (void *)ctx;
}

static void init_img(Config *config, FIBITMAP *dib) {
	// lossy compression
	config->img_h = FreeImage_GetHeight(dib);
	config->img_w = FreeImage_GetWidth(dib);
	config->depth = FreeImage_GetBPP(dib);
	config->sign = get_img_type(dib);
	config->num_comp = get_num_comp(dib);
	config->tile_w = config->img_w;
	config->tile_h = config->img_h;

	mem_mg_t *mem_mg = (mem_mg_t *)malloc(sizeof(mem_mg_t));
	init_mem_mg(mem_mg);
	config->mem_mg = mem_mg;
}

void init_config(Config *config, type_parameters *param) {
	config->num_dlvls = param->param_tile_comp_dlvls;
	config->wavelet_type = param->param_wavelet_type;

	config->cblk_exp_w = param->param_cblk_exp_w;
	config->cblk_exp_h = param->param_cblk_exp_h;

	config->use_mct = param->param_use_mct;
	config->target_size = param->param_target_size;
}

static void **read_img(const char *in_file, Config *config) {
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(in_file, 0);
	FIBITMAP* dib = FreeImage_Load(formato, in_file, 0);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

	FreeImage_SetOutputMessage(_FreeImageErrorHandler);

	type_data **img_data_d = NULL;

	if(image_type == FIT_BITMAP)
	{
//		printf("BITMAP\n");

		FIBITMAP* src = FreeImage_ConvertTo24Bits(dib);

		FreeImage_Unload(dib);

		if(FreeImage_HasPixels(src) == FALSE)
		{
			printf("Do not have pixel data!\n");
			exit(0);
		}

		init_img(config, src);

		int scan_width = FreeImage_GetPitch(src);
		int mem_size = config->img_h * scan_width;

		BYTE *bits = malloc(mem_size * sizeof(BYTE));

		// convert the bitmap to raw bits (top-left pixel first)
		FreeImage_ConvertToRawBits(bits, src, scan_width, FreeImage_GetBPP(src), FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

		FreeImage_Unload(src);

		type_data **img_data_h = (type_data **)malloc(sizeof(type_data *) * config->num_comp);
		mem_mg_t *mem_mg = config->mem_mg;
		int c;
		for(c = 0; c < config->num_comp; ++c) {
			cuda_h_allocate_mem((void **)&img_data_h[c], sizeof(type_data) * config->img_w * config->img_h);
		}
		img_data_d = (type_data **)malloc(sizeof(type_data *) * config->num_comp);
		for(c = 0; c < config->num_comp; ++c) {
			img_data_d[c] = (type_data *)mem_mg->alloc->dev(sizeof(type_data) * config->img_w * config->img_h, mem_mg->ctx);
		}

		//copying data DIRECTLY as tiles to device
		int x, y, i;
		for(c = 0; c < config->num_comp; ++c) {
			for(y = 0; y < config->img_h; ++y) {
				for(x = 0; x < config->img_w; ++x) {
					img_data_h[c][x + y * config->img_w] = (type_data)bits[y * scan_width + x * config->num_comp + (config->num_comp - 1 - c)];
				}
			}
			cuda_memcpy_htd(img_data_h[c], img_data_d[c], config->img_w * config->img_h * sizeof(type_data));
			cuda_h_free(img_data_h[c]);
		}
		free(img_data_h);
	}
	return (void **)img_data_d;
}

static void save_img(const char *out_file, Chunk *img) {
	FILE *fp = fopen(out_file, "wb");

	size_t count = fwrite(img->data, sizeof(uint8_t), img->length, fp);

	if(count != img->length) {
		perror("Could not write output image\n");
		fclose(fp);
		return;
	}

	fclose(fp);
}

int main(int argc, char *argv[]) {
	type_image *img = (type_image *)malloc(sizeof(type_image));
	if((parse_args(argc, argv, img) == ERROR) || (check_args_enc(img) == ERROR))
	{
		fprintf(stderr, "Error occurred while parsing arguments.\n");
		fprintf(stdout, "%s", help);
		return 1;
	}
	type_parameters *param = (type_parameters*)malloc(sizeof(type_parameters));
	if((parse_config(img->conf_file, param) == ERROR) || (check_config(param) == ERROR)) {
		fprintf(stderr, "Error occurred while parsing configuration file.\n");
		fprintf(stdout, "%s", help);
		return 1;
	}

	cuda_set_device_flags();
	cuda_set_device(param->param_device);
	Config *config = (Config *)malloc(sizeof(Config));
	init_config(config, param);
	void **img_data = read_img(img->in_file, config);

	if(img_data == NULL) {
		perror("Could not read input image\n");
		free(config);
		return -1;
	}

	Chunk *blocks, *order;

	long int start_enc;
	start_enc = start_measure();

	encode(img_data, config, &blocks, &order);
	printf("Encoding time: %ld\n", stop_measure(start_enc));

	save_img(img->out_file, blocks);

	mem_mg_t *mem_mg = config->mem_mg;
	mem_mg->dealloc->host(((Chunk *)blocks)->data, mem_mg->ctx);
	mem_mg->dealloc->host(blocks, mem_mg->ctx);

	ctx_t *ctx = ((ctx_t *)mem_mg->ctx);
	deinit_dev_mem(((ctx_m_t *)ctx->dev)->mem);
	deinit_host_mem(((ctx_m_t *)ctx->host)->mem);

//	printf("host memory leaks %lld\n", actual_size(((ctx_m_t *)ctx->host)->head));
//	printf("dev memory leaks %lld\n", actual_size(((ctx_m_t *)ctx->dev)->head));
//	printf("host max used memory %lld\n", ((ctx_m_t *)ctx->host)->max);
//	printf("dev max used memory %lld\n", ((ctx_m_t *)ctx->dev)->max);

	free(config);

	return 0;
}
