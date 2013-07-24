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
 * list.h
 *
 *  Created on: Feb 13, 2012
 *      Author: miloszc
 */

typedef struct t_node t_node;

struct t_node {
	t_node *next;
	size_t size;
	uint64_t id;
};

t_node *create(uint64_t id, size_t size);
void insert(t_node **head, t_node *tmp);
t_node *find(t_node **head, uint64_t id);
void remove_node(t_node **head, t_node *elem);
size_t actual_size(t_node *head);
