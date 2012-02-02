/*
 * main.c
 *
 *  Created on: Feb 2, 2012
 *      Author: miloszc
 */

#include <FreeImage.h>
#include "gpu_jpeg2k.h"
#include "misc/cuda_errors.h"
#include <cuda.h>
#include <cuda_runtime_api.h>

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
   printf("\n*** ");
   if(fif != FIF_UNKNOWN) {
     printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
   }
   printf("%s", message);
   printf(" ***\n");
}

void cuda_d_free(void *data, void *ctx)
{
	cudaFree(data);
	checkCUDAError("cuda_d_free");
}

void cuda_h_free(void *data, void *ctx) {
	cudaFreeHost(data);
	checkCUDAError("cuda_h_free");
}

void *cuda_h_allocate_mem(size_t mem_size, void *ctx)
{
	void *data;
	cudaHostAlloc(&data, mem_size, cudaHostAllocMapped);
	checkCUDAError("cuda_h_allocate_mem");
	return data;
}

void *cuda_d_allocate_mem(uint64_t mem_size, void *ctx)
{
	void *data;
	cudaMalloc(&data, mem_size);
	checkCUDAError("cuda_d_allocate_mem");
	return data;
}

void init_config(Config *config, FIBITMAP *dib) {
	config->img_h = FreeImage_GetHeight(dib);
	config->img_w = FreeImage_GetWidth(dib);
	config->depth = FreeImage_GetBPP(dib);
	config->num_comp = get_num_comp(dib);
	config->tile_w = -1U;
	config->tile_h = -1U;

	/* TODO: check if origin data was signed? */
	config->sign = get_img_type(dib);
	config->num_dlvls = 4;

	config->wavelet_type = DWT_97;
	config->use_mct = 1;
	config->target_size = 0;

	mem_mg_t *mem_mg = (mem_mg_t *)malloc(sizeof(mem_mg_t));
	mem_mg->alloc = (alloc_t *)malloc(sizeof(alloc_t));
	mem_mg->alloc->host = cuda_h_allocate_mem;
	mem_mg->alloc->dev = cuda_d_allocate_mem;
	mem_mg->dealloc = (dealloc_t *)malloc(sizeof(dealloc_t));
	mem_mg->dealloc->host = cuda_h_free;
	mem_mg->dealloc->dev = cuda_d_free;

	config->mem_mg = mem_mg;
}

void read_img(const char *in_file, Config *config) {
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(in_file, 0);
	FIBITMAP* dib = FreeImage_Load(formato, in_file, 0);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	if(image_type == FIT_BITMAP)
	{
//		printf("BITMAP\n");

		dib = FreeImage_ConvertTo24Bits(dib);

		if(FreeImage_HasPixels(dib) == FALSE)
		{
			printf("Do not have pixel data!\n");
			exit(0);
		}

		init_image(dib, container, param);

		int scan_width = FreeImage_GetPitch(dib);
		int mem_size = container->height * scan_width;

		BYTE *bits = malloc(mem_size * sizeof(BYTE));

		// convert the bitmap to raw bits (top-left pixel first)
		FreeImage_ConvertToRawBits(bits, dib, scan_width, FreeImage_GetBPP(dib), /*FI_RGBA_RED_MASK*/FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK/*FI_RGBA_BLUE_MASK*/, TRUE);
//		FreeImage_ConvertToRawBits(bits, dib, scan_width, FreeImage_GetBPP(dib), FI_RGBA_GREEN_MASK/*FI_RGBA_BLUE_MASK*/, FI_RGBA_BLUE_MASK, /*FI_RGBA_RED_MASK*/FI_RGBA_RED_MASK, TRUE);

		FreeImage_Unload(dib);

		long int start_copy;
		start_copy = start_measure();

		//copying data DIRECTLY as tiles to device
		int x, y, c, i;
		type_tile *tile;
		type_tile_comp *tile_comp;
		for(i = 0; i < container->num_tiles; i++) {
			tile = &(container->tile[i]);
			for(c = 0; c < container->num_components; c++) {
				tile_comp = &(tile->tile_comp[c]);
				for(y = 0; y < tile_comp->height; y++) {
					for(x = 0; x < tile_comp->width; x++) {
						tile_comp->img_data[x + y*tile_comp->width] =
						(type_data)bits[(tile->tly + y) * scan_width + (tile->tlx + x) * container->num_components + c];
//						if(c == 0)
//							printf("%6d,", bits[(tile->tly + y) * scan_width + (tile->tlx + x) * container->num_components + c] - 128);
					}
//					if(c == 0)
//						printf("\n");
				}
				cuda_memcpy_htd(tile_comp->img_data, tile_comp->img_data_d, tile_comp->width * tile_comp->height * sizeof(type_data));
				cuda_h_free(tile_comp->img_data);
//				free(tile_comp->img_data);
			}
		}
	}
}

int main(int argc, char *argv[]) {

	encode();

	return 0;
}
