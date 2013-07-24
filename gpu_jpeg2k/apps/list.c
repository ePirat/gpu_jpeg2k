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
 * list.c
 *
 *  Created on: Feb 13, 2012
 *      Author: miloszc
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "list.h"
#include "func.h"

t_node *create(uint64_t id, size_t size) {
	t_node *elem = (t_node *) malloc(sizeof(t_node));
	elem->next = NULL;
	elem->id = id;
	elem->size = size;
	return elem;
}

void insert(t_node **head, t_node *tmp) {
	if(head == NULL) {
		*head = tmp;
		return;
	}
	tmp->next = *head;
	*head = tmp;
}

t_node *find(t_node **head, uint64_t id) {
	if(*head == NULL)
		return NULL;

	t_node *curr = *head;
	while((curr != NULL) && (curr->id != id)) {
		curr = curr->next;
	}
	if(curr == NULL) {
		perror("Could not find element with id: %lld\n", id);
	}
	return curr;
}

void remove_node(t_node **head, t_node *elem) {
	if((*head == NULL) || (elem == NULL))
		return;

	if(*head == elem) {
		*head = elem->next;
		free(elem);
		return;
	}

	t_node *curr = *head;
	while ((curr != NULL) && (curr->next != elem)) {
		curr = curr->next;
	}

	curr->next = elem->next;
	free(elem);
}

size_t actual_size(t_node *head) {
	if(head == NULL)
		return 0;

	size_t curr_size = 0;
	t_node *curr = head;
	while(curr != NULL) {
		curr_size += curr->size;
		curr = curr->next;
	}
	return curr_size;
}
