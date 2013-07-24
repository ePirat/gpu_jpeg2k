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
