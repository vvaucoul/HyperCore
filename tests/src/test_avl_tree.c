/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_avl_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:53:07 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 19:58:02 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_init() {
	printf("Testing AVL tree initialization...\n");
	AVLTree tree;
	assert(avl_tree_init(&tree, sizeof(int), compare_int) == 0);
	assert(tree.root == NULL);
	assert(tree.size == 0);
	avl_tree_destroy(&tree);
	printf("Initialization test passed!\n");
}

static void test_insert_and_find() {
	printf("Testing insertions and searches...\n");
	AVLTree tree;
	avl_tree_init(&tree, sizeof(int), compare_int);

	int values[] = {44, 17, 78, 32, 50, 88, 48, 62, 54};
	int count	 = sizeof(values) / sizeof(values[0]);

	for (int i = 0; i < count; i++) {
		assert(avl_tree_insert(&tree, &values[i], &values[i], sizeof(int)) == 0);
		assert(*(int *)avl_tree_find(&tree, &values[i]) == values[i]);
	}

	// Verify tree is balanced
	assert(avl_tree_verify(&tree) == 1);

	// Test non-existent value
	int non_existent = 999;
	assert(avl_tree_find(&tree, &non_existent) == NULL);

	avl_tree_destroy(&tree);
	printf("Insertion and search tests passed!\n");
}

static void test_min_max() {
	printf("Testing min/max operations...\n");
	AVLTree tree;
	avl_tree_init(&tree, sizeof(int), compare_int);

	int values[] = {44, 17, 78, 32, 50, 88, 48, 62, 54};
	int count	 = sizeof(values) / sizeof(values[0]);

	for (int i = 0; i < count; i++) {
		avl_tree_insert(&tree, &values[i], &values[i], sizeof(int));
	}

	assert(*(int *)avl_tree_min(&tree) == 17);
	assert(*(int *)avl_tree_max(&tree) == 88);

	avl_tree_destroy(&tree);
	printf("Min/max operations test passed!\n");
}

int main() {
	printf("Starting AVL Tree tests...\n");

	test_init();
	test_insert_and_find();
	test_min_max();

	printf("All AVL Tree tests passed successfully!\n");
	return 0;
}
