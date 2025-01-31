/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_list.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:02:22 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 20:58:45 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Maximum number of levels in the skip list
 * Affects the space-time trade-off. Higher values improve search speed but use more memory.
 */
#define MAX_LEVEL 16

/**
 * @brief Probability factor for level promotion
 * Determines how many nodes get promoted to higher levels.
 * 0.5 gives a balanced structure - each level has about half the nodes of the level below.
 */
#define SKIP_LIST_P 0.5

/**
 * @brief Node structure for the skip list
 * Each node contains a key, a value pointer, and an array of forward pointers.
 */
typedef struct SkipNode {
	int key;				   /**< Integer key used for ordering and searching */
	void *value;			   /**< Pointer to the stored value (can be any type) */
	struct SkipNode **forward; /**< Array of forward pointers for each level */
	int level;				   /**< Number of levels in this node */
} SkipNode;

/**
 * @brief Skip list main structure
 * Contains the header node and metadata about the list.
 */
typedef struct SkipList {
	SkipNode *header; /**< Header node (sentinel) */
	int level;		  /**< Current maximum level in the skip list */
	int size;		  /**< Number of elements in the skip list */
} SkipList;

/**
 * @brief Creates a new empty skip list
 *
 * Allocates and initializes a new skip list with a header node.
 * The header node is created with MAX_LEVEL forward pointers.
 *
 * @return SkipList* Pointer to the new skip list, or NULL if allocation fails
 */
SkipList *skip_list_create(void);

/**
 * @brief Destroys a skip list and frees all associated memory
 *
 * Frees all nodes, their forward pointer arrays, and the list structure.
 * Note: Does not free the values pointed to by node->value.
 * The caller is responsible for freeing any dynamically allocated values.
 *
 * @param list Pointer to the skip list to destroy
 */
void skip_list_destroy(SkipList *list);

/**
 * @brief Inserts a new key-value pair or updates an existing key
 *
 * If the key already exists, updates the value and returns 0.
 * If the key is new, creates a new node with a random level and returns 1.
 *
 * @param list The skip list to insert into
 * @param key The key to insert/update
 * @param value Pointer to the value to store
 * @return int 1 if a new node was created, 0 if value was updated
 */
int skip_list_insert(SkipList *list, int key, void *value);

/**
 * @brief Deletes a node with the specified key
 *
 * Removes the node if found and adjusts all necessary forward pointers.
 * Note: Does not free the value pointer, caller must handle value cleanup.
 *
 * @param list The skip list to delete from
 * @param key The key to delete
 * @return int 1 if the key was found and deleted, 0 otherwise
 */
int skip_list_delete(SkipList *list, int key);

/**
 * @brief Searches for a node with the specified key
 *
 * Uses the skip list structure to perform an efficient search,
 * with expected O(log n) time complexity.
 *
 * @param list The skip list to search in
 * @param key The key to search for
 * @return SkipNode* Pointer to the found node, or NULL if not found
 */
SkipNode *skip_list_search(SkipList *list, int key);

/**
 * @brief Prints the skip list structure for debugging
 *
 * Displays each level of the skip list, showing the keys in each level
 * and their connections. Useful for visualizing the skip list structure.
 *
 * @param list The skip list to print
 */
void skip_list_print(SkipList *list);

#endif // SKIP_LIST_H
