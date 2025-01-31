/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rb_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:46:16 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 21:14:19 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Compare function for integers
static int compare_ints(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

// Compare function for strings
static int compare_strings(const void *a, const void *b) {
	return strcmp((const char *)a, (const char *)b);
}

static void test_init() {
	printf("Testing initialization...\n");
	RBTree tree;
	assert(rb_tree_init(&tree, sizeof(int), compare_ints) == 0);
	assert(tree.root == NULL);
	assert(tree.size == 0);
	assert(tree.key_size == sizeof(int));
	rb_tree_destroy(&tree);
	printf("Initialization test passed!\n");
}

static void test_insert_ints() {
	printf("Testing integer insertions...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	int values[]	= {5, 3, 7, 1, 9, 6, 8, 2, 4};
	int value_count = sizeof(values) / sizeof(values[0]);

	// Insert values
	for (int i = 0; i < value_count; i++) {
		assert(rb_tree_insert(&tree, &values[i], &values[i], sizeof(int)) == 0);
		assert(tree.size == (size_t)(i + 1));
	}

	// Verify insertions
	for (int i = 0; i < value_count; i++) {
		void *result = rb_tree_find(&tree, &values[i]);
		assert(result != NULL);
		assert(*(int *)result == values[i]);
	}

	// Test non-existent value
	int non_existent = 42;
	assert(rb_tree_find(&tree, &non_existent) == NULL);

	rb_tree_destroy(&tree);
	printf("Integer insertion test passed!\n");
}

static void test_string_operations() {
	printf("Testing string operations...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(char) * 20, compare_strings);

	const char *strings[] = {"apple", "banana", "cherry", "date", "elderberry"};
	int values[]		  = {1, 2, 3, 4, 5};
	int count			  = 5;

	// Insert strings
	for (int i = 0; i < count; i++) {
		assert(rb_tree_insert(&tree, strings[i], &values[i], sizeof(int)) == 0);
	}

	// Verify searches
	for (int i = 0; i < count; i++) {
		void *result = rb_tree_find(&tree, strings[i]);
		assert(result != NULL);
		assert(*(int *)result == values[i]);
	}

	// Test non-existent string
	assert(rb_tree_find(&tree, "nonexistent") == NULL);

	rb_tree_destroy(&tree);
	printf("String operations test passed!\n");
}

static void test_duplicates() {
	printf("Testing duplicate handling...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	int key	   = 5;
	int value1 = 10;
	int value2 = 20;

	// Insert initial value
	assert(rb_tree_insert(&tree, &key, &value1, sizeof(int)) == 0);
	assert(tree.size == 1);

	// Insert duplicate key with different value
	assert(rb_tree_insert(&tree, &key, &value2, sizeof(int)) == 0);
	assert(tree.size == 1);

	// Verify the value was updated
	void *result = rb_tree_find(&tree, &key);
	assert(result != NULL);
	assert(*(int *)result == value2);

	rb_tree_destroy(&tree);
	printf("Duplicate handling test passed!\n");
}

static void test_memory_management() {
	printf("Testing memory management...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	// Insert some values
	for (int i = 0; i < 100; i++) {
		assert(rb_tree_insert(&tree, &i, &i, sizeof(int)) == 0);
	}

	// Clear the tree
	rb_tree_clear(&tree);
	assert(tree.size == 0);
	assert(tree.root == NULL);

	// Insert after clearing
	int value = 42;
	assert(rb_tree_insert(&tree, &value, &value, sizeof(int)) == 0);
	assert(tree.size == 1);

	rb_tree_destroy(&tree);
	printf("Memory management test passed!\n");
}

static void test_min_max() {
	printf("Testing min/max operations...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	// Test empty tree
	assert(rb_tree_min(&tree) == NULL);
	assert(rb_tree_max(&tree) == NULL);

	int values[]	= {5, 3, 7, 1, 9, 6, 8, 2, 4};
	int value_count = sizeof(values) / sizeof(values[0]);

	// Insert values
	for (int i = 0; i < value_count; i++) {
		rb_tree_insert(&tree, &values[i], &values[i], sizeof(int));
	}

	// Verify min and max
	assert(*(int *)rb_tree_min(&tree) == 1);
	assert(*(int *)rb_tree_max(&tree) == 9);

	rb_tree_destroy(&tree);
	printf("Min/max operations test passed!\n");
}

static void test_tree_properties() {
	printf("Testing Red-Black Tree properties...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	// Empty tree should be valid
	assert(rb_tree_verify(&tree) == 1);

	// Insert values in a way that forces rotations
	int values[]	= {7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13};
	int value_count = sizeof(values) / sizeof(values[0]);

	for (int i = 0; i < value_count; i++) {
		rb_tree_insert(&tree, &values[i], &values[i], sizeof(int));
		// Tree should maintain properties after each insertion
		assert(rb_tree_verify(&tree) == 1);
	}

	rb_tree_destroy(&tree);
	printf("Tree properties test passed!\n");
}

static void test_edge_cases() {
	printf("Testing edge cases...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);

	// Test empty tree operations
	assert(rb_tree_find(&tree, &(int){1}) == NULL);
	assert(rb_tree_remove(&tree, &(int){1}) == -1); // Assurez-vous que rb_tree_remove retourne -1 pour une clé inexistante

	// Test with minimum and maximum integers
	int min_val = INT_MIN;
	int max_val = INT_MAX;

	assert(rb_tree_insert(&tree, &min_val, &min_val, sizeof(int)) == 0);
	assert(rb_tree_insert(&tree, &max_val, &max_val, sizeof(int)) == 0);

	void *result = rb_tree_find(&tree, &min_val);
	assert(result != NULL && *(int *)result == INT_MIN);

	result = rb_tree_find(&tree, &max_val);
	assert(result != NULL && *(int *)result == INT_MAX);

	// Test suppression des nœuds
	assert(rb_tree_remove(&tree, &min_val) == 0);
	assert(rb_tree_find(&tree, &min_val) == NULL);
	assert(rb_tree_remove(&tree, &max_val) == 0);
	assert(rb_tree_find(&tree, &max_val) == NULL);

	rb_tree_destroy(&tree);
	printf("✓ Edge cases test passed!\n");
}

static void test_performance() {
	printf("Testing performance...\n");
	RBTree tree;
	rb_tree_init(&tree, sizeof(int), compare_ints);
	clock_t start, end;
	double cpu_time_used;
	int dummy_value = 0;

	// Test insertion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		rb_tree_insert(&tree, &i, &dummy_value, sizeof(int));
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 insertions: %f seconds\n", cpu_time_used);

	// Test search performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		rb_tree_find(&tree, &i);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 searches: %f seconds\n", cpu_time_used);

	// Test deletion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		rb_tree_remove(&tree, &i);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 deletions: %f seconds\n", cpu_time_used);

	rb_tree_destroy(&tree);
	printf("✓ Performance test completed\n");
}

int main() {
	printf("=== Starting Red-Black Tree Tests ===\n\n");
	srand(time(NULL));

	test_init();
	test_insert_ints();
	test_string_operations();
	test_duplicates();
	test_edge_cases();
	test_memory_management();
	test_min_max();
	test_tree_properties();
	test_performance();

	printf("\n=== All Red-Black Tree Tests Passed ===\n");
	return 0;
}
