/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:53:04 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 19:57:57 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/algorithms/avl_tree.h"
#include <stdlib.h>
#include <string.h>

static int height(AVLNode *node) {
	return node ? node->height : 0;
}

static int balance_factor(AVLNode *node) {
	return node ? height(node->left) - height(node->right) : 0;
}

static void update_height(AVLNode *node) {
	if (!node) return;
	int left_height	 = height(node->left);
	int right_height = height(node->right);
	node->height	 = (left_height > right_height ? left_height : right_height) + 1;
}

static AVLNode *rotate_right(AVLNode *y) {
	AVLNode *x	= y->left;
	AVLNode *T2 = x->right;

	x->right = y;
	y->left	 = T2;

	update_height(y);
	update_height(x);

	return x;
}

static AVLNode *rotate_left(AVLNode *x) {
	AVLNode *y	= x->right;
	AVLNode *T2 = y->left;

	y->left	 = x;
	x->right = T2;

	update_height(x);
	update_height(y);

	return y;
}

static AVLNode *create_node(const void *key, const void *value, size_t key_size, size_t value_size) {
	AVLNode *node = malloc(sizeof(AVLNode));
	if (!node) return NULL;

	node->key	= malloc(key_size);
	node->value = malloc(value_size);
	if (!node->key || !node->value) {
		free(node->key);
		free(node->value);
		free(node);
		return NULL;
	}

	memcpy(node->key, key, key_size);
	memcpy(node->value, value, value_size);
	node->height = 1;
	node->left = node->right = NULL;
	return node;
}

static AVLNode *balance_node(AVLNode *node) {
	if (!node) return NULL;

	update_height(node);
	int balance = balance_factor(node);

	// Left Heavy
	if (balance > 1) {
		if (balance_factor(node->left) < 0)
			node->left = rotate_left(node->left);
		return rotate_right(node);
	}

	// Right Heavy
	if (balance < -1) {
		if (balance_factor(node->right) > 0)
			node->right = rotate_right(node->right);
		return rotate_left(node);
	}

	return node;
}

static AVLNode *insert_recursive(AVLNode *node, const void *key, const void *value, size_t key_size, size_t value_size, int (*compare_func)(const void *, const void *)) {
	if (!node)
		return create_node(key, value, key_size, value_size);

	int cmp = compare_func(key, node->key);
	if (cmp < 0)
		node->left = insert_recursive(node->left, key, value, key_size, value_size, compare_func);
	else if (cmp > 0)
		node->right = insert_recursive(node->right, key, value, key_size, value_size, compare_func);
	else {
		memcpy(node->value, value, value_size);
		return node;
	}

	return balance_node(node);
}

int avl_tree_init(AVLTree *tree, size_t key_size, int (*compare_func)(const void *, const void *)) {
	if (!tree || !compare_func) return -1;
	tree->root		   = NULL;
	tree->size		   = 0;
	tree->key_size	   = key_size;
	tree->compare_func = compare_func;
	return 0;
}

int avl_tree_insert(AVLTree *tree, const void *key, const void *value, size_t value_size) {
	if (!tree || !key || !value) return -1;

	tree->root = insert_recursive(tree->root, key, value, tree->key_size, value_size, tree->compare_func);
	if (!tree->root) return -1;
	tree->size++;
	return 0;
}

void *avl_tree_find(AVLTree *tree, const void *key) {
	if (!tree || !key) return NULL;

	AVLNode *current = tree->root;
	while (current) {
		int cmp = tree->compare_func(key, current->key);
		if (cmp == 0)
			return current->value;
		current = (cmp < 0) ? current->left : current->right;
	}
	return NULL;
}

static void free_node(AVLNode *node) {
	if (!node) return;
	free(node->key);
	free(node->value);
	free(node);
}

static void clear_recursive(AVLNode *node) {
	if (!node) return;
	clear_recursive(node->left);
	clear_recursive(node->right);
	free_node(node);
}

void avl_tree_clear(AVLTree *tree) {
	if (!tree) return;
	clear_recursive(tree->root);
	tree->root = NULL;
	tree->size = 0;
}

void avl_tree_destroy(AVLTree *tree) {
	avl_tree_clear(tree);
}

size_t avl_tree_size(AVLTree *tree) {
	return tree ? tree->size : 0;
}

int avl_tree_empty(AVLTree *tree) {
	return tree ? tree->size == 0 : 1;
}

void *avl_tree_min(AVLTree *tree) {
	if (!tree || !tree->root) return NULL;
	AVLNode *current = tree->root;
	while (current->left)
		current = current->left;
	return current->key;
}

void *avl_tree_max(AVLTree *tree) {
	if (!tree || !tree->root) return NULL;
	AVLNode *current = tree->root;
	while (current->right)
		current = current->right;
	return current->key;
}

static int verify_recursive(AVLNode *node) {
	if (!node) return 1;

	int balance = balance_factor(node);
	if (balance < -1 || balance > 1)
		return 0;

	return verify_recursive(node->left) && verify_recursive(node->right);
}

int avl_tree_verify(AVLTree *tree) {
	return tree ? verify_recursive(tree->root) : 0;
}
