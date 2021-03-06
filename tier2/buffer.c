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
 * buffer.c
 *
 *  Created on: Dec 1, 2011
 *      Author: miloszc
 */

#include <stdlib.h>
#include <malloc.h>

#include "buffer.h"

void init_dec_buffer(FILE *fsrc, type_buffer *src_buff) {
	fseek(fsrc, 0, SEEK_END);
	long file_length = ftell(fsrc);
	fseek(fsrc, 0, SEEK_SET);

	src_buff->data = (uint8_t *) malloc(file_length + 1);
	src_buff->size = file_length;

    if (src_buff->data == NULL) {
        printf("Failed allocating memory");
        exit(1);
    }
    
	size_t r = fread(src_buff->data, file_length, 1, fsrc);
    if (r != 1) {
        printf("Error reading!\n");
        exit(1);
    }
    

	src_buff->start = src_buff->data;
	src_buff->end = src_buff->data + src_buff->size;
	src_buff->bp = src_buff->data;
	src_buff->bits_count = 0;
	src_buff->byte = 0;
}
