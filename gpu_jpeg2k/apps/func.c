/*
 * func.c
 *
 *  Created on: Feb 3, 2012
 *      Author: miloszc
 */

#include <stdio.h>
#include "func.h"
#include "../misc/memory_management.cuh"
#include "../include/memory.h"
#include "list.h"

#define DATA_ALIGMENT 256

void _cuda_d_free(void *data, void *ctx)
{
	ctx_m_t *ctx_ = (ctx_m_t *)((ctx_t *)ctx)->dev;

	t_node *tmp = find((t_node **)&(ctx_->head), (uint64_t)data);
//	printf("f %x %lld %lld\n", (uint64_t)data, tmp->size, actual_size((t_node *)ctx_->head));
	remove_node((t_node **)&(ctx_->head), tmp);

//	cuda_d_free(data);
}

void _cuda_h_free(void *data, void *ctx) {
	ctx_m_t *ctx_ = (ctx_m_t *)((ctx_t *)ctx)->host;
//	printf("f %x %lld\n", (uint64_t)data, actual_size((t_node *)ctx_->head));

	t_node *tmp = find((t_node **)&(ctx_->head), (uint64_t)data);
	remove_node((t_node **)&(ctx_->head), tmp);

//	cuda_h_free(data);
}

void *_cuda_h_allocate_mem(size_t mem_size, void *ctx)
{
	void *data = NULL;
//	cuda_h_allocate_mem(&data, mem_size);

	size_t aligned_size = mem_size + (DATA_ALIGMENT - (mem_size & (DATA_ALIGMENT - 1)));
	ctx_m_t *ctx_ = (ctx_m_t *)((ctx_t *)ctx)->host;
	data = (void *)(((uint8_t *)(ctx_->mem->p)) + ctx_->mem->alloc_size);
	ctx_->mem->alloc_size += aligned_size;
	if(ctx_->mem->alloc_size > ctx_->mem->size) {
		perror("Insufficient memory space\n");
		return NULL;
	}
	insert((t_node **)&(ctx_->head), create((uint64_t)data, aligned_size));

	size_t size = actual_size((t_node *)ctx_->head);
	if(ctx_->max < size) {
		ctx_->max = size;
	}

//	printf("a %x %lld %lld\n", (uint64_t)data, mem_size, actual_size((t_node *)ctx_->head));

	return data;
}

void *_cuda_d_allocate_mem(size_t mem_size, void *ctx)
{
	void *data = NULL;
//	cuda_d_allocate_mem(&data, mem_size);

	size_t aligned_size = mem_size + (DATA_ALIGMENT - (mem_size & (DATA_ALIGMENT - 1)));
//	printf("%lld	%lld\n", mem_size, aligned_size);
	ctx_m_t *ctx_ = (ctx_m_t *)((ctx_t *)ctx)->dev;
	data = (void *)(((uint8_t *)(ctx_->mem->p)) + ctx_->mem->alloc_size);
	ctx_->mem->alloc_size += aligned_size;
	if(ctx_->mem->alloc_size > ctx_->mem->size) {
		perror("Insufficient memory space\n");
		return NULL;
	}
//	printf("data %lld z %lld\n", (uint64_t)data, ((uint64_t)data & (DATA_ALIGMENT - 1)));
	insert((t_node **)&(ctx_->head), create((uint64_t)data, aligned_size));

	size_t size = actual_size((t_node *)ctx_->head);
	if(ctx_->max < size) {
		ctx_->max = size;
	}

//	printf("a %x %lld %lld\n", (uint64_t)data, aligned_size, actual_size((t_node *)ctx_->head));

	return data;
}
