/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_skip_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:57:58 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 20:58:02 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Test structure for complex values
typedef struct TestData {
	char *name;
	int value;
} TestData;

static TestData *create_test_data(const char *name, int value) {
	TestData *data = malloc(sizeof(TestData));
	data->name	   = strdup(name);
	data->value	   = value;
	return data;
}

static void free_test_data(TestData *data) {
	free(data->name);
	free(data);
}

static void test_creation_destruction(void) {
	printf("Testing creation and destruction...\n");
	SkipList *list = skip_list_create();
	assert(list != NULL);
	assert(list->size == 0);
	assert(list->level == 1);
	skip_list_destroy(list);
	printf("✓ Creation/destruction test passed\n");
}

static void test_basic_operations(void) {
	printf("Testing basic operations...\n");
	SkipList *list = skip_list_create();

	// Test with simple integer values
	int value1 = 50;
	int value2 = 100;
	assert(skip_list_insert(list, 5, &value1) == 1);
	assert(skip_list_insert(list, 10, &value2) == 1);
	assert(list->size == 2);

	SkipNode *node = skip_list_search(list, 5);
	assert(node != NULL);
	assert(node->key == 5);
	assert(*(int *)node->value == 50);

	// Test with complex data
	TestData *data = create_test_data("test", 123);
	assert(skip_list_insert(list, 15, data) == 1);

	node = skip_list_search(list, 15);
	assert(node != NULL);
	TestData *found_data = (TestData *)node->value;
	assert(found_data->value == 123);
	assert(strcmp(found_data->name, "test") == 0);

	// Test update
	TestData *new_data = create_test_data("updated", 456);
	assert(skip_list_insert(list, 15, new_data) == 0);
	free_test_data(data); // Free old data

	// Test deletion
	assert(skip_list_delete(list, 5) == 1);
	assert(skip_list_search(list, 5) == NULL);
	assert(list->size == 2);

	// Cleanup
	free_test_data(new_data);
	skip_list_destroy(list);
	printf("✓ Basic operations test passed\n");
}

static void test_edge_cases(void) {
	printf("Testing edge cases...\n");
	SkipList *list = skip_list_create();

	// Test empty list operations
	assert(skip_list_search(list, 1) == NULL);
	assert(skip_list_delete(list, 1) == 0);

	// Test with minimum and maximum integers
	int min_val = 0, max_val = 0;
	assert(skip_list_insert(list, INT_MIN, &min_val) == 1);
	assert(skip_list_insert(list, INT_MAX, &max_val) == 1);
	assert(skip_list_search(list, INT_MIN) != NULL);
	assert(skip_list_search(list, INT_MAX) != NULL);

	skip_list_destroy(list);
	printf("✓ Edge cases test passed\n");
}

static void test_performance(void) {
	printf("Testing performance...\n");
	SkipList *list = skip_list_create();
	clock_t start, end;
	double cpu_time_used;
	int dummy_value = 0;

	// Test insertion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		skip_list_insert(list, i, &dummy_value);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 insertions: %f seconds\n", cpu_time_used);

	// Test search performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		skip_list_search(list, i);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 searches: %f seconds\n", cpu_time_used);

	// Test deletion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		skip_list_delete(list, i);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 deletions: %f seconds\n", cpu_time_used);

	skip_list_destroy(list);
	printf("✓ Performance test completed\n");
}

int main(void) {
	printf("=== Starting Skip List Tests ===\n\n");
	srand(time(NULL));

	test_creation_destruction();
	test_basic_operations();
	test_edge_cases();
	test_performance();

	printf("\n=== All Skip List Tests Passed ===\n");
	return 0;
}
