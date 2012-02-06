/*
 * image_raw.h
 *
 *  Created on: Feb 6, 2012
 *      Author: miloszc
 */

#ifndef IMAGE_RAW_H_
#define IMAGE_RAW_H_

#include "image_types.h"
#include "../gpu_jpeg2k.h"

void save_raw(type_image *img, Chunk **blocks, Chunk **order);

#endif /* IMAGE_RAW_H_ */
