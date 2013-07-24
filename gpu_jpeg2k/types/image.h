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
void deinit_tiles(type_image *img);
void set_coding_parameters(type_image *img, type_parameters *param);
int read_image(type_image *img, type_parameters *param);
void save_image(type_image *img);

#endif /* IMAGE_H_ */
