/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:32:32 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:34:01 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/algorithms/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int list_init(List *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return 0;
}

static ListNode *create_node(const void *element, size_t element_size) {
	ListNode *node = malloc(sizeof(ListNode));
	if (!node) {
		fprintf(stderr, "Error: Failed to allocate memory for list node\n");
		return NULL;
	}

	node->data = malloc(element_size);
	if (!node->data) {
		fprintf(stderr, "Error: Failed to allocate memory for node data\n");
		free(node);
		return NULL;
	}

	memcpy(node->data, element, element_size);
	node->prev = NULL;
	node->next = NULL;
	return node;
}

int list_push_back(List *list, const void *element, size_t element_size) {
	ListNode *node = create_node(element, element_size);
	if (!node) return -1;

	if (list->tail) {
		node->prev		 = list->tail;
		list->tail->next = node;
		list->tail		 = node;
	} else {
		list->head = list->tail = node;
	}

	list->size++;
	return 0;
}

int list_push_front(List *list, const void *element, size_t element_size) {
	ListNode *node = create_node(element, element_size);
	if (!node) return -1;

	if (list->head) {
		node->next		 = list->head;
		list->head->prev = node;
		list->head		 = node;
	} else {
		list->head = list->tail = node;
	}

	list->size++;
	return 0;
}

int list_pop_back(List *list) {
	if (!list->tail) return -1;

	ListNode *node = list->tail;
	list->tail	   = node->prev;

	if (list->tail) {
		list->tail->next = NULL;
	} else {
		list->head = NULL;
	}

	free(node->data);
	free(node);
	list->size--;
	return 0;
}

int list_pop_front(List *list) {
	if (!list->head) return -1;

	ListNode *node = list->head;
	list->head	   = node->next;

	if (list->head) {
		list->head->prev = NULL;
	} else {
		list->tail = NULL;
	}

	free(node->data);
	free(node);
	list->size--;
	return 0;
}

void *list_get(List *list, size_t index) {
	if (index >= list->size) {
		fprintf(stderr, "Error: Index out of bounds [%zu] (size: %zu)\n", index, list->size);
		return NULL;
	}

	ListNode *current = list->head;
	for (size_t i = 0; i < index; i++) {
		current = current->next;
	}

	return current->data;
}

int list_insert(List *list, size_t index, const void *element, size_t element_size) {
	if (index > list->size) {
		fprintf(stderr, "Error: Index out of bounds for insertion\n");
		return -1;
	}

	if (index == 0) return list_push_front(list, element, element_size);
	if (index == list->size) return list_push_back(list, element, element_size);

	ListNode *node = create_node(element, element_size);
	if (!node) return -1;

	ListNode *current = list->head;
	for (size_t i = 0; i < index; i++) {
		current = current->next;
	}

	node->prev			= current->prev;
	node->next			= current;
	current->prev->next = node;
	current->prev		= node;

	list->size++;
	return 0;
}

int list_erase(List *list, size_t index) {
	if (index >= list->size) {
		fprintf(stderr, "Error: Index out of bounds for erasure\n");
		return -1;
	}

	if (index == 0) return list_pop_front(list);
	if (index == list->size - 1) return list_pop_back(list);

	ListNode *current = list->head;
	for (size_t i = 0; i < index; i++) {
		current = current->next;
	}

	current->prev->next = current->next;
	current->next->prev = current->prev;

	free(current->data);
	free(current);
	list->size--;
	return 0;
}

void *list_front(List *list) {
	if (!list->head) {
		fprintf(stderr, "Error: Cannot access front of empty list\n");
		return NULL;
	}
	return list->head->data;
}

void *list_back(List *list) {
	if (!list->tail) {
		fprintf(stderr, "Error: Cannot access back of empty list\n");
		return NULL;
	}
	return list->tail->data;
}

int list_empty(List *list) {
	return list->size == 0;
}

size_t list_size(List *list) {
	return list->size;
}

void list_clear(List *list) {
	while (!list_empty(list)) {
		list_pop_front(list);
	}
}

void list_destroy(List *list) {
	list_clear(list);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
