/*
 * dec_main.c
 *
 *  Created on: Feb 6, 2012
 *      Author: miloszc
 */

#include <FreeImage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gpu_jpeg2k.h"
#include "../config/parameters.h"
#include "../config/arguments.h"
#include "../config/help.h"
#include "../types/image_types.h"
#include "func.h"

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

static void init_config(Config *config) {
	memset(config, 0, sizeof(Config));

	mem_mg_t *mem_mg = (mem_mg_t *)malloc(sizeof(mem_mg_t));
	init_mem_mg(mem_mg);
	config->mem_mg = mem_mg;
}

static Chunk *read_img(const char *filename) {
	FILE *fsrc = fopen(filename, "rb");
	if (!fsrc) {
		fprintf(stderr, "Error, failed to open %s for reading\n", filename);
		return NULL;
	}

	fseek(fsrc, 0, SEEK_END);
	long file_length = ftell(fsrc);
	fseek(fsrc, 0, SEEK_SET);

	Chunk *img_data = (Chunk *)malloc(sizeof(Chunk));
	img_data->length = file_length;
	img_data->data = (void *)malloc(img_data->length);

	size_t count = fread(img_data->data, sizeof(uint8_t), img_data->length, fsrc);

	if(count != img_data->length) {
		perror("Could not read input image!\n");
		return NULL;
	}

	fclose(fsrc);

	return img_data;
}

static int save_img(Chunk *blocks, Config *config, const char *filename)
{
	int c, x, y;

	int scan_width = config->img_w * (config->num_comp);
	BYTE *bits = (BYTE*) malloc(config->img_h * scan_width * sizeof(BYTE));

	if(bits == NULL) {
		perror("Could not allocate output data!\n");
	}

	for (c = 0; c < config->num_comp; ++c) {
		for (y = 0; y < config->img_h; ++y) {
			for (x = 0; x < config->img_w; ++x) {
				bits[y * scan_width + x * config->num_comp + (config->num_comp - 1 - c)] = ((BYTE *)(blocks[c].data))[x + y * config->img_w];
			}
		}
	}

	// convert the bitmap to raw bits (top-left pixel first)
	FIBITMAP *dst = FreeImage_ConvertFromRawBits(bits, config->img_w, config->img_h, scan_width, 24, FI_RGBA_RED_MASK,
			FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	if (FreeImage_Save(FIF_BMP, dst, filename, 0)) {
//		printf("saved: %s", filename);
	} else {
		printf("saving FAILED: %s", filename);
	}

	FreeImage_Unload(dst);
	free(bits);

	return 0;
}

int main(int argc, char *argv[]) {
	type_image *img = (type_image *)malloc(sizeof(type_image));
	if((parse_args(argc, argv, img) == ERROR) || (check_args_dec(img) == ERROR))
	{
		fprintf(stderr, "Error occurred while parsing arguments.\n");
		fprintf(stdout, "%s", help);
		return 1;
	}

	cuda_set_device_flags();
	Config *config = (Config *)malloc(sizeof(Config));
	init_config(config);

	Chunk *img_data = read_img(img->in_file);
	Chunk *blocks, *order;

	long int start_dec;
	start_dec = start_measure();

	decode(img_data, config, &blocks, &order);

	printf("Decoding time:%d\n", stop_measure(start_dec));

	save_img(blocks, config, img->out_file);

	ctx_t *ctx = ((ctx_t *)config->mem_mg->ctx);
	deinit_dev_mem(((ctx_m_t *)ctx->dev)->mem);
	deinit_host_mem(((ctx_m_t *)ctx->host)->mem);

	free(config);

	return 0;
}
