/*
 * func.c
 *
 *  Created on: Feb 3, 2012
 *      Author: miloszc
 */

extern "C" {
#include "func.h"
#include "../misc/memory_management.cuh"
#include "list.h"
}
#include <stdio.h>

void _cuda_d_free(void *data, void *ctx)
{
	ctx_t *ctx_ = (ctx_t *)ctx;
//	printf("f %x %lld\n", (uint64_t)data, actual_size((t_node *)ctx_->head));

	t_node *tmp = find((t_node **)&(ctx_->head), (uint64_t)data);
	remove_node((t_node **)&(ctx_->head), tmp);

//	cuda_d_free(data);
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
//	cuda_d_allocate_mem(&data, mem_size);

	ctx_t *ctx_ = (ctx_t *)ctx;
	data = (void *)(((uint8_t *)(ctx_->mem->p)) + ctx_->mem->alloc_size);
	ctx_->mem->alloc_size += mem_size;
	if(ctx_->mem->alloc_size > ctx_->mem->size) {
		perror("Insufficient memory space\n");
		return NULL;
	}
	insert((t_node **)&(ctx_->head), create((uint64_t)data, mem_size));

	size_t size = actual_size((t_node *)ctx_->head);
	if(ctx_->max < size) {
		ctx_->max = size;
	}

//	printf("a %x %lld %lld\n", (uint64_t)data, mem_size, actual_size((t_node *)ctx_->head));

	return data;
}
