/*
 * func.h
 *
 *  Created on: Feb 3, 2012
 *      Author: miloszc
 */

#ifndef FUNC_H_
#define FUNC_H_

typedef struct {
	void *p;
	size_t alloc_size;
	size_t size;
} mem_t;

typedef struct {
	void *head;
	size_t max;
	mem_t *mem;
} ctx_m_t;

void _cuda_d_free(void *data, void *ctx);

void _cuda_h_free(void *data, void *ctx);

void *_cuda_h_allocate_mem(size_t mem_size, void *ctx);

void *_cuda_d_allocate_mem(size_t mem_size, void *ctx);

#endif /* FUNC_H_ */
