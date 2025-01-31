/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rb_tree.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:40:30 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 21:06:36 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RB_TREE_H
#define RB_TREE_H

#include <stddef.h>

typedef enum {
	RB_BLACK,
	RB_RED
} RBColor;

/**
 * @brief Node structure for Red-Black Tree
 */
typedef struct RBNode {
	void *key;
	void *value;
	RBColor color;
	struct RBNode *parent;
	struct RBNode *left;
	struct RBNode *right;
} RBNode;

/**
 * @brief Red-Black Tree implementation
 *
 * @param root Root node of the tree
 * @param size Current number of nodes
 * @param key_size Size of key type
 * @param compare_func Function to compare keys
 */
typedef struct {
	RBNode *root;
	size_t size;
	size_t key_size;
	int (*compare_func)(const void *, const void *);
} RBTree;

/**
 * @brief Initialize a new Red-Black Tree
 *
 * @param tree Pointer to the tree to initialize
 * @param key_size Size of key type in bytes
 * @param compare_func Function to compare keys
 * @return int 0 on success, -1 on failure
 */
int rb_tree_init(RBTree *tree, size_t key_size, int (*compare_func)(const void *, const void *));

/**
 * @brief Insert key-value pair into the tree
 */
int rb_tree_insert(RBTree *tree, const void *key, const void *value, size_t value_size);

/**
 * @brief Find value associated with key
 */
void *rb_tree_find(RBTree *tree, const void *key);

/**
 * @brief Remove key-value pair from tree
 */
int rb_tree_remove(RBTree *tree, const void *key);

/**
 * @brief Check if key exists in tree
 */
int rb_tree_contains(RBTree *tree, const void *key);

/**
 * @brief Get number of nodes in tree
 */
size_t rb_tree_size(RBTree *tree);

/**
 * @brief Check if tree is empty
 */
int rb_tree_empty(RBTree *tree);

/**
 * @brief Remove all nodes from tree
 */
void rb_tree_clear(RBTree *tree);

/**
 * @brief Destroy tree and free memory
 */
void rb_tree_destroy(RBTree *tree);

/**
 * @brief Get minimum key in tree
 */
void *rb_tree_min(RBTree *tree);

/**
 * @brief Get maximum key in tree
 */
void *rb_tree_max(RBTree *tree);

/**
 * @brief Verify Red-Black Tree properties
 */
int rb_tree_verify(RBTree *tree);

#endif // RB_TREE_H
