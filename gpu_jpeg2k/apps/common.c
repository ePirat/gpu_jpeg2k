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
 * common.c
 *
 *  Created on: Feb 14, 2012
 *      Author: miloszc
 */

#include <stddef.h>
#include <stdlib.h>
#include "common.h"

// 512 MB = 536870912
#define DEV_ALLOC_SIZE 536870912
#define HOST_ALLOC_SIZE 8*104857600

static void init_dev_mem(mem_t *mem) {
	printf("Static allocation of %d bytes of GPU memory\n", DEV_ALLOC_SIZE);
	cuda_d_allocate_mem(&(mem->p), DEV_ALLOC_SIZE);
	cuda_d_memset(mem->p, 0, DEV_ALLOC_SIZE);
	mem->size = DEV_ALLOC_SIZE;
	mem->alloc_size = 0;
}

static void init_host_mem(mem_t *mem) {
	printf("Static allocation of %d bytes of CPU memory\n", HOST_ALLOC_SIZE);
	cuda_h_allocate_mem(&(mem->p), HOST_ALLOC_SIZE);
	memset(mem->p, 0, HOST_ALLOC_SIZE);
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
