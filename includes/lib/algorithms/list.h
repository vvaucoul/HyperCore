/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:32:34 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:34:01 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
 * @brief Node structure for doubly-linked list
 */
typedef struct ListNode {
	void *data;
	struct ListNode *prev;
	struct ListNode *next;
} ListNode;

/**
 * @brief Doubly-linked list implementation
 *
 * @param head First node of the list
 * @param tail Last node of the list
 * @param size Current number of elements
 */
typedef struct {
	ListNode *head;
	ListNode *tail;
	size_t size;
} List;

/**
 * @brief Initialize a new empty list
 */
int list_init(List *list);

/**
 * @brief Add element to the end of the list
 */
int list_push_back(List *list, const void *element, size_t element_size);

/**
 * @brief Add element to the front of the list
 */
int list_push_front(List *list, const void *element, size_t element_size);

/**
 * @brief Remove last element
 */
int list_pop_back(List *list);

/**
 * @brief Remove first element
 */
int list_pop_front(List *list);

/**
 * @brief Get element at specified position
 */
void *list_get(List *list, size_t index);

/**
 * @brief Insert element at specified position
 */
int list_insert(List *list, size_t index, const void *element, size_t element_size);

/**
 * @brief Remove element at specified position
 */
int list_erase(List *list, size_t index);

/**
 * @brief Get first element
 */
void *list_front(List *list);

/**
 * @brief Get last element
 */
void *list_back(List *list);

/**
 * @brief Check if list is empty
 */
int list_empty(List *list);

/**
 * @brief Get current size of the list
 */
size_t list_size(List *list);

/**
 * @brief Remove all elements
 */
void list_clear(List *list);

/**
 * @brief Destroy list and free memory
 */
void list_destroy(List *list);

#endif // LIST_H
