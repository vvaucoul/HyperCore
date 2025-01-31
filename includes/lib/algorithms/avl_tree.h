/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_tree.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:53:06 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:38:08 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stddef.h>

/**
 * @brief Node structure for AVL tree
 *
 * Represents a single node in the AVL tree, containing the key-value pair,
 * height information, and pointers to left and right children.
 */
typedef struct AVLNode {
	void *key;			   /**< Pointer to the key */
	void *value;		   /**< Pointer to the stored value */
	int height;			   /**< Height of the node for balancing */
	struct AVLNode *left;  /**< Pointer to left child */
	struct AVLNode *right; /**< Pointer to right child */
} AVLNode;

/**
 * @brief AVL tree container structure
 *
 * Main structure for the AVL tree, containing the root node, size information,
 * and the comparison function used for ordering.
 */
typedef struct {
	AVLNode *root;									 /**< Root node of the tree */
	size_t size;									 /**< Number of nodes in the tree */
	size_t key_size;								 /**< Size of the key type in bytes */
	int (*compare_func)(const void *, const void *); /**< Key comparison function */
} AVLTree;

/**
 * @brief Initialize a new AVL tree
 *
 * @param tree Pointer to the tree structure to initialize
 * @param key_size Size of the key type in bytes
 * @param compare_func Function to compare two keys
 * @return int 0 on success, non-zero on failure
 */
int avl_tree_init(AVLTree *tree, size_t key_size, int (*compare_func)(const void *, const void *));

/**
 * @brief Insert a new key-value pair into the tree
 *
 * @param tree Target AVL tree
 * @param key Pointer to the key
 * @param value Pointer to the value
 * @param value_size Size of the value in bytes
 * @return int 0 on success, non-zero on failure
 */
int avl_tree_insert(AVLTree *tree, const void *key, const void *value, size_t value_size);

/**
 * @brief Remove a node with the specified key
 *
 * @param tree Target AVL tree
 * @param key Key to remove
 * @return int 0 on success, non-zero if key not found
 */
int avl_tree_remove(AVLTree *tree, const void *key);

/**
 * @brief Find a value by its key
 *
 * @param tree Target AVL tree
 * @param key Key to search for
 * @return void* Pointer to the found value, NULL if not found
 */
void *avl_tree_find(AVLTree *tree, const void *key);

/**
 * @brief Get the current size of the tree
 *
 * @param tree Target AVL tree
 * @return size_t Number of nodes in the tree
 */
size_t avl_tree_size(AVLTree *tree);

/**
 * @brief Check if the tree is empty
 *
 * @param tree Target AVL tree
 * @return int 1 if empty, 0 if not empty
 */
int avl_tree_empty(AVLTree *tree);

/**
 * @brief Remove all nodes from the tree
 *
 * @param tree Target AVL tree
 */
void avl_tree_clear(AVLTree *tree);

/**
 * @brief Destroy the tree and free all resources
 *
 * @param tree Target AVL tree
 */
void avl_tree_destroy(AVLTree *tree);

/**
 * @brief Get the minimum key value in the tree
 *
 * @param tree Target AVL tree
 * @return void* Pointer to the minimum value, NULL if tree is empty
 */
void *avl_tree_min(AVLTree *tree);

/**
 * @brief Get the maximum key value in the tree
 *
 * @param tree Target AVL tree
 * @return void* Pointer to the maximum value, NULL if tree is empty
 */
void *avl_tree_max(AVLTree *tree);

/**
 * @brief Verify the AVL tree properties
 *
 * Checks if the tree maintains AVL properties (balance factor)
 * and binary search tree properties.
 *
 * @param tree Target AVL tree
 * @return int 1 if valid, 0 if invalid
 */
int avl_tree_verify(AVLTree *tree);

#endif // AVL_TREE_H
