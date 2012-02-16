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
