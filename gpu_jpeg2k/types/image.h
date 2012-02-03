/**
 * @file image.h
 *
 * @author Milosz Ciznicki
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "image_types.h"
#include "../config/parameters.h"

void init_tiles(type_image **_img);
void set_coding_parameters(type_image *img, type_parameters *param);
int read_image(type_image *img, type_parameters *param);
void save_image(type_image *img);

#endif /* IMAGE_H_ */
