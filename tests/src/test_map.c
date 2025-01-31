/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:39:02 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:39:02 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_int_map(void) {
	Map map;
	printf("\n=== Testing Integer Key Map ===\n");

	if (map_init(&map, 0, sizeof(int), map_hash_int, map_compare_int) != 0) {
		printf("FAIL: Map initialization\n");
		return;
	}
	printf("PASS: Map initialized\n");

	// Test insertion
	for (int i = 0; i < 5; i++) {
		int value = i * 10;
		if (map_insert(&map, &i, &value, sizeof(int)) != 0) {
			printf("FAIL: Insert operation for key %d\n", i);
			map_destroy(&map);
			return;
		}
	}
	printf("PASS: Inserted 5 key-value pairs\n");

	// Test retrieval
	for (int i = 0; i < 5; i++) {
		int *value = map_get(&map, &i);
		if (value && *value == i * 10) {
			printf("PASS: Retrieved correct value %d for key %d\n", *value, i);
		} else {
			printf("FAIL: Retrieved incorrect value for key %d\n", i);
		}
	}

	printf("Current size: %zu\n", map_size(&map));
	map_destroy(&map);
	printf("PASS: Map destroyed\n");
}

void test_string_map(void) {
	Map map;
	printf("\n=== Testing String Key Map ===\n");

	if (map_init(&map, 0, sizeof(char *), map_hash_string, map_compare_string) != 0) {
		printf("FAIL: Map initialization\n");
		return;
	}
	printf("PASS: Map initialized\n");

	// Test string key insertion
	const char *keys[] = {"one", "two", "three", "four", "five"};
	int values[]	   = {1, 2, 3, 4, 5};

	for (int i = 0; i < 5; i++) {
		if (map_insert(&map, keys[i], &values[i], sizeof(int)) != 0) {
			printf("FAIL: Insert operation for key '%s'\n", keys[i]);
			map_destroy(&map);
			return;
		}
	}
	printf("PASS: Inserted 5 string key-value pairs\n");

	// Test retrieval
	for (int i = 0; i < 5; i++) {
		int *value = map_get(&map, keys[i]);
		if (value && *value == values[i]) {
			printf("PASS: Retrieved correct value %d for key '%s'\n", *value, keys[i]);
		} else {
			printf("FAIL: Retrieved incorrect value for key '%s'\n", keys[i]);
		}
	}

	map_destroy(&map);
	printf("PASS: Map destroyed\n");
}

void test_map_operations(void) {
	Map map;
	printf("\n=== Testing Map Operations ===\n");

	map_init(&map, 0, sizeof(int), map_hash_int, map_compare_int);

	// Test empty map
	if (map_empty(&map)) {
		printf("PASS: Map is initially empty\n");
	}

	// Test insertion and update
	int key = 1, value = 100;
	map_insert(&map, &key, &value, sizeof(int));

	value = 200; // Update value
	map_insert(&map, &key, &value, sizeof(int));

	int *retrieved = map_get(&map, &key);
	if (retrieved && *retrieved == 200) {
		printf("PASS: Value successfully updated\n");
	}

	// Test contains
	if (map_contains(&map, &key)) {
		printf("PASS: Contains key check successful\n");
	}

	// Test erase
	if (map_erase(&map, &key) == 0) {
		printf("PASS: Key-value pair erased\n");
	}

	// Test clear
	for (int i = 0; i < 5; i++) {
		map_insert(&map, &i, &i, sizeof(int));
	}
	map_clear(&map);
	if (map_size(&map) == 0) {
		printf("PASS: Map cleared successfully\n");
	}

	map_destroy(&map);
}

void test_collision_handling(void) {
	Map map;
	printf("\n=== Testing Collision Handling ===\n");

	// Initialize with small capacity to force collisions
	map_init(&map, 2, sizeof(int), map_hash_int, map_compare_int);

	// Insert multiple items that will hash to the same bucket
	for (int i = 0; i < 10; i++) {
		int value = i * 100;
		if (map_insert(&map, &i, &value, sizeof(int)) == 0) {
			printf("PASS: Inserted value %d\n", value);
		}
	}

	// Verify all values can be retrieved correctly
	for (int i = 0; i < 10; i++) {
		int *value = map_get(&map, &i);
		if (value && *value == i * 100) {
			printf("PASS: Retrieved correct value %d after collision\n", *value);
		}
	}

	printf("Final size: %zu, Capacity: %zu\n", map_size(&map), map.capacity);
	map_destroy(&map);
}

int main() {
	printf("Starting Map Tests\n");
	printf("=================\n");

	test_int_map();
	test_string_map();
	test_map_operations();
	test_collision_handling();

	printf("\nAll tests completed!\n");
	return 0;
}
