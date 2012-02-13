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
