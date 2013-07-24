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
 * memory.h
 *
 *  Created on: Feb 2, 2012
 *      Author: miloszc
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>

/** Memory allocator */
typedef struct {
	void *(*host)(size_t size, void *ctx);
	void *(*dev)(size_t size, void *ctx);
} alloc_t;

/** Memory deallocator */
typedef struct {
	void (*host)(void *ptr, void *ctx);
	void (*dev)(void *ptr, void *ctx);
} dealloc_t;

/** Context */
typedef struct {
	void *host;
	void *dev;
}ctx_t;

/** Memory manager */
typedef struct {
	alloc_t *alloc;
	dealloc_t *dealloc;
	ctx_t *ctx;
} mem_mg_t;

#endif /* MEMORY_H_ */
