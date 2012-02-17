/*
 * mqc_wrapper.h
 *
 *  Created on: Dec 9, 2011
 *      Author: miloszc
 */

#ifndef MQC_WRAPPER_H_
#define MQC_WRAPPER_H_

#include "../gpu_coder.h"
#include "../gpu_coeff_coder2.cuh"
#include "../../../include/memory.h"

unsigned char* mqc_gpu_encode(EntropyCodingTaskInfo *infos, CodeBlockAdditionalInfo* h_infos, int codeBlocks,
		unsigned char *d_outbuf, int maxOutLength, mem_mg_t *mem_mg);

#endif /* MQC_WRAPPER_H_ */
