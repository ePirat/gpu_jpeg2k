/*
 * func.c
 *
 *  Created on: Feb 3, 2012
 *      Author: miloszc
 */

extern "C" {
#include "func.h"
#include "misc/memory_management.cuh"
}

void _cuda_d_free(void *data, void *ctx)
{
	cuda_d_free(data);
}

void _cuda_h_free(void *data, void *ctx) {
	cuda_h_free(data);
}

void *_cuda_h_allocate_mem(size_t mem_size, void *ctx)
{
	void *data = NULL;
	cuda_h_allocate_mem(&data, mem_size);
	return data;
}

void *_cuda_d_allocate_mem(size_t mem_size, void *ctx)
{
	void *data = NULL;
	cuda_d_allocate_mem(&data, mem_size);
	return data;
}
