/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:22:28 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:50:33 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>
#include <stdio.h>
#include <stdlib.h>

void test_basic_operations(void) {
	Vector vec;
	printf("\n=== Testing Basic Operations ===\n");

	if (vector_init(&vec, 0) != 0) {
		printf("FAIL: Vector initialization\n");
		return;
	}
	printf("PASS: Vector initialized\n");

	// Test push_back
	int numbers[] = {1, 2, 3, 4, 5};
	for (int i = 0; i < 5; i++) {
		if (vector_push_back(&vec, &numbers[i], sizeof(int)) != 0) {
			printf("FAIL: Push back operation at index %d\n", i);
			vector_destroy(&vec);
			return;
		}
	}
	printf("PASS: Added 5 elements via push_back\n");
	printf("Current size: %d, Capacity: %d\n", vector_size(&vec), vector_capacity(&vec));

	vector_destroy(&vec);
	printf("PASS: Vector destroyed\n");
}

void test_insert_erase(void) {
	Vector vec;
	printf("\n=== Testing Insert and Erase ===\n");

	vector_init(&vec, 2);

	int values[] = {10, 20, 30};
	for (int i = 0; i < 3; i++) {
		vector_push_back(&vec, &values[i], sizeof(int));
	}

	// Test insert
	int insert_val = 15;
	if (vector_insert(&vec, 1, &insert_val, sizeof(int)) == 0) {
		printf("PASS: Inserted %d at position 1\n", insert_val);
	}

	// Verify insertion
	int *val = vector_get(&vec, 1);
	if (val && *val == insert_val) {
		printf("PASS: Retrieved correct inserted value\n");
	}

	// Test erase
	if (vector_erase(&vec, 1) == 0) {
		printf("PASS: Erased element at position 1\n");
	}

	printf("Size after operations: %d\n", vector_size(&vec));
	vector_destroy(&vec);
}

void test_edge_cases(void) {
	Vector vec;
	printf("\n=== Testing Edge Cases ===\n");

	if (vector_init(&vec, 1) != 0) {
		printf("FAIL: Vector initialization failed\n");
		return;
	}

	// Test empty vector operations
	if (vector_empty(&vec)) {
		printf("PASS: Vector is initially empty\n");
	} else {
		printf("FAIL: Vector should be empty\n");
	}

	// Test invalid index access
	printf("\nTesting out-of-bounds access (expected error message):\n");
	void *result = vector_get(&vec, 5);
	if (result == NULL) {
		printf("PASS: Properly handled out-of-bounds access\n");
	} else {
		printf("FAIL: Out-of-bounds access did not return NULL\n");
	}

	// Test front/back on empty vector
	printf("\nTesting front/back on empty vector (expected error messages):\n");
	result = vector_front(&vec);
	if (result == NULL) {
		printf("PASS: Front properly handles empty vector\n");
	}

	result = vector_back(&vec);
	if (result == NULL) {
		printf("PASS: Back properly handles empty vector\n");
	}

	// Test inserting at invalid positions
	printf("\nTesting invalid insertions:\n");
	int value = 42;
	if (vector_insert(&vec, -1, &value, sizeof(int)) != 0) {
		printf("PASS: Properly rejected negative index\n");
	}
	if (vector_insert(&vec, 2, &value, sizeof(int)) != 0) {
		printf("PASS: Properly rejected out-of-bounds insert\n");
	}

	vector_destroy(&vec);
	printf("PASS: Vector destroyed\n");
}

void test_stress(void) {
	Vector vec;
	printf("\n=== Stress Testing ===\n");

	vector_init(&vec, 1);

	// Add many elements to force multiple resizes
	const int num_elements = 1000;
	for (int i = 0; i < num_elements; i++) {
		if (vector_push_back(&vec, &i, sizeof(int)) != 0) {
			printf("FAIL: Stress test failed at element %d\n", i);
			vector_destroy(&vec);
			return;
		}
	}

	printf("PASS: Successfully added %d elements\n", num_elements);
	printf("Final size: %d, Capacity: %d\n", vector_size(&vec), vector_capacity(&vec));

	vector_destroy(&vec);
}

int main() {
	printf("Starting Vector Tests\n");
	printf("====================\n");

	test_basic_operations();
	test_insert_erase();
	test_edge_cases();
	test_stress();

	printf("\nAll tests completed!\n");
	return 0;
}
