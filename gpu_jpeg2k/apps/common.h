/*
 * common.h
 *
 *  Created on: Feb 14, 2012
 *      Author: miloszc
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "func.h"
#include "../include/memory.h"

void init_ctx(ctx_t *ctx);
void deinit_dev_mem(mem_t *mem);
void deinit_host_mem(mem_t *mem);

#endif /* COMMON_H_ */
