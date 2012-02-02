/*
 * memory.h
 *
 *  Created on: Feb 2, 2012
 *      Author: miloszc
 */

#ifndef MEMORY_H_
#define MEMORY_H_

typedef struct {
	void *(*host)(size_t size, void *ctx);
	void *(*dev)(size_t size, void *ctx);
} alloc_t;

typedef struct {
	void (*host)(void *ptr, void *ctx);
	void (*dev)(void *ptr, void *ctx);
} dealloc_t;

typedef struct {
	alloc_t *alloc;
	dealloc_t *dealloc;
	void *ctx;
} mem_mg_t;

#endif /* MEMORY_H_ */
