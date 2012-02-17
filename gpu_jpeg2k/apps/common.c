/*
 * common.c
 *
 *  Created on: Feb 14, 2012
 *      Author: miloszc
 */

#include <stddef.h>
#include <stdlib.h>
#include "common.h"

// 512 MB = 536870912
#define DEV_ALLOC_SIZE 1342177280
#define HOST_ALLOC_SIZE 104857600

static void init_dev_mem(mem_t *mem) {
	cuda_d_allocate_mem(&(mem->p), DEV_ALLOC_SIZE);
	mem->size = DEV_ALLOC_SIZE;
	mem->alloc_size = 0;
}

static void init_host_mem(mem_t *mem) {
	cuda_h_allocate_mem(&(mem->p), HOST_ALLOC_SIZE);
	mem->size = HOST_ALLOC_SIZE;
	mem->alloc_size = 0;
}

void deinit_dev_mem(mem_t *mem) {
	cuda_d_free(mem->p);
}

void deinit_host_mem(mem_t *mem) {
	cuda_h_free(mem->p);
}

void init_ctx(ctx_t *ctx) {
	// Host Ctx
	ctx_m_t *ctx_h = (ctx_m_t *)malloc(sizeof(ctx_m_t));
	ctx_h->max = 0;
	ctx_h->head = NULL;

	mem_t *mem_h = (mem_t *)malloc(sizeof(mem_t));
	init_host_mem(mem_h);
	ctx_h->mem = mem_h;

	ctx->host = (void *)ctx_h;

	// Dev Ctx
	ctx_m_t *ctx_d = (ctx_m_t *)malloc(sizeof(ctx_m_t));
	ctx_d->max = 0;
	ctx_d->head = NULL;

	mem_t *mem_d = (mem_t *)malloc(sizeof(mem_t));
	init_dev_mem(mem_d);
	ctx_d->mem = mem_d;

	ctx->dev = (void *)ctx_d;
}
