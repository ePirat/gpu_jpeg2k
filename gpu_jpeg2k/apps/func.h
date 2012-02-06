/*
 * func.h
 *
 *  Created on: Feb 3, 2012
 *      Author: miloszc
 */

#ifndef FUNC_H_
#define FUNC_H_

void _cuda_d_free(void *data, void *ctx);

void _cuda_h_free(void *data, void *ctx);

void *_cuda_h_allocate_mem(size_t mem_size, void *ctx);

void *_cuda_d_allocate_mem(size_t mem_size, void *ctx);

#endif /* FUNC_H_ */
