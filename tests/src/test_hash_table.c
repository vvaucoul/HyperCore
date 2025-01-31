/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_hash_table.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:50:51 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:53:06 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <hypercore.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Simple hash function for strings
static unsigned int string_hash(const void *key) {
	const char *str	  = *(const char **)key;
	unsigned int hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

// String comparison function
static int string_compare(const void *a, const void *b) {
	const char *str1 = *(const char **)a;
	const char *str2 = *(const char **)b;
	return strcmp(str1, str2);
}

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
	HashTable table;
	bool success = hash_table_init(&table, 16, sizeof(char *), string_hash, string_compare);
	assert(success && "Hash table initialization failed");
	assert(table.size == 0);
	assert(table.capacity == 16);
	hash_table_destroy(&table);
	printf("✓ Creation/destruction test passed\n");
}

static void test_basic_operations(void) {
	printf("Testing basic operations...\n");
	HashTable table;
	hash_table_init(&table, 16, sizeof(char *), string_hash, string_compare);

	// Test with simple string values
	const char *key1   = "key1";
	const char *value1 = "value1";
	assert(hash_table_insert(&table, &key1, &value1, sizeof(char *)));

	// Test with complex data
	TestData *data	 = create_test_data("test", 123);
	const char *key2 = "key2";
	assert(hash_table_insert(&table, &key2, &data, sizeof(TestData *)));

	// Verify insertions
	void *found = hash_table_find(&table, &key1);
	assert(found != NULL);
	assert(strcmp(*(const char **)found, "value1") == 0);

	found = hash_table_find(&table, &key2);
	assert(found != NULL);
	TestData *found_data = *(TestData **)found;
	assert(found_data->value == 123);
	assert(strcmp(found_data->name, "test") == 0);

	// Test removal
	assert(hash_table_remove(&table, &key1));
	assert(hash_table_find(&table, &key1) == NULL);

	// Cleanup
	free_test_data(data);
	hash_table_destroy(&table);
	printf("✓ Basic operations test passed\n");
}

static void test_edge_cases(void) {
	printf("Testing edge cases...\n");
	HashTable table;
	hash_table_init(&table, 1, sizeof(char *), string_hash, string_compare); // Small initial size

	// Test empty table operations
	const char *key = "nonexistent";
	assert(hash_table_find(&table, &key) == NULL);
	assert(!hash_table_remove(&table, &key));

	// Test inserting many items to force resizing
	char keys[100][20];
	const char *value = "test";

	for (int i = 0; i < 100; i++) {
		sprintf(keys[i], "key%d", i);
		const char *current_key = keys[i];
		assert(hash_table_insert(&table, &current_key, &value, sizeof(char *)));
	}

	// Verify all insertions
	for (int i = 0; i < 100; i++) {
		const char *current_key = keys[i];
		assert(hash_table_find(&table, &current_key) != NULL);
	}

	hash_table_destroy(&table);
	printf("✓ Edge cases test passed\n");
}

static void test_collisions(void) {
	printf("Testing collision handling...\n");
	HashTable table;
	hash_table_init(&table, 4, sizeof(char *), string_hash, string_compare); // Small table to force collisions

	// Insert items that will likely collide
	const char *keys[]	 = {"abc", "cba", "bac", "acb", "cab", "bca"};
	const char *values[] = {"val1", "val2", "val3", "val4", "val5", "val6"};

	for (int i = 0; i < 6; i++) {
		assert(hash_table_insert(&table, &keys[i], &values[i], sizeof(char *)));
	}

	// Verify all values can be retrieved correctly
	for (int i = 0; i < 6; i++) {
		void *found = hash_table_find(&table, &keys[i]);
		assert(found != NULL);
		assert(strcmp(*(const char **)found, values[i]) == 0);
	}

	hash_table_destroy(&table);
	printf("✓ Collision handling test passed\n");
}

static void test_performance(void) {
	printf("Testing performance...\n");
	HashTable table;
	hash_table_init(&table, 1024, sizeof(char *), string_hash, string_compare);
	clock_t start, end;
	double cpu_time_used;
	char keys[100000][20];
	const char *value = "test_value";

	// Prepare keys
	for (int i = 0; i < 100000; i++) {
		sprintf(keys[i], "key%d", i);
	}

	// Test insertion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		const char *current_key = keys[i];
		hash_table_insert(&table, &current_key, &value, sizeof(char *));
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 insertions: %f seconds\n", cpu_time_used);

	// Test lookup performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		const char *current_key = keys[i];
		hash_table_find(&table, &current_key);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 lookups: %f seconds\n", cpu_time_used);

	// Test deletion performance
	start = clock();
	for (int i = 0; i < 100000; i++) {
		const char *current_key = keys[i];
		hash_table_remove(&table, &current_key);
	}
	end			  = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time for 100,000 deletions: %f seconds\n", cpu_time_used);

	hash_table_destroy(&table);
	printf("✓ Performance test completed\n");
}

int main(void) {
	printf("=== Starting Hash Table Tests ===\n\n");

	test_creation_destruction();
	test_basic_operations();
	test_edge_cases();
	test_collisions();
	test_performance();

	printf("\n=== All Hash Table Tests Passed ===\n");
	return 0;
}
