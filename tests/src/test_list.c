/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:35:11 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 21:08:26 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>
#include <stdio.h>
#include <stdlib.h>

void test_basic_operations(void) {
	List list;
	printf("\n=== Testing Basic Operations ===\n");

	if (list_init(&list) != 0) {
		printf("FAIL: List initialization\n");
		return;
	}
	printf("PASS: List initialized\n");

	// Test push operations
	int numbers[] = {1, 2, 3, 4, 5};

	// Test push_back
	for (int i = 0; i < 3; i++) {
		if (list_push_back(&list, &numbers[i], sizeof(int)) != 0) {
			printf("FAIL: Push back operation at index %d\n", i);
			list_destroy(&list);
			return;
		}
	}
	printf("PASS: Added 3 elements via push_back\n");

	// Test push_front
	for (int i = 3; i < 5; i++) {
		if (list_push_front(&list, &numbers[i], sizeof(int)) != 0) {
			printf("FAIL: Push front operation at index %d\n", i);
			list_destroy(&list);
			return;
		}
	}
	printf("PASS: Added 2 elements via push_front\n");
	printf("Current size: %zu\n", list_size(&list));

	list_destroy(&list);
	printf("PASS: List destroyed\n");
}

void test_insert_erase(void) {
	List list;
	printf("\n=== Testing Insert and Erase ===\n");

	list_init(&list);

	// Add initial elements
	int values[] = {10, 20, 30};
	for (int i = 0; i < 3; i++) {
		list_push_back(&list, &values[i], sizeof(int));
	}

	// Test insert in middle
	int insert_val = 15;
	if (list_insert(&list, 1, &insert_val, sizeof(int)) == 0) {
		printf("PASS: Inserted %d at position 1\n", insert_val);
	}

	// Verify insertion
	int *val = list_get(&list, 1);
	if (val && *val == insert_val) {
		printf("PASS: Retrieved correct inserted value\n");
	}

	// Test erase
	if (list_erase(&list, 1) == 0) {
		printf("PASS: Erased element at position 1\n");
	}

	printf("Size after operations: %zu\n", list_size(&list));
	list_destroy(&list);
}

void test_edge_cases(void) {
	List list;
	printf("\n=== Testing Edge Cases ===\n");

	list_init(&list);

	// Test empty list operations
	if (list_empty(&list)) {
		printf("PASS: List is initially empty\n");
	}

	// Test invalid index access
	printf("\nTesting out-of-bounds access (expected error message):\n");
	void *result = list_get(&list, 5);
	if (result == NULL) {
		printf("PASS: Properly handled out-of-bounds access\n");
	}

	// Test front/back on empty list
	printf("\nTesting front/back on empty list (expected error messages):\n");
	if (list_front(&list) == NULL && list_back(&list) == NULL) {
		printf("PASS: Front/Back properly handle empty list\n");
	}

	// Test pop operations on empty list
	if (list_pop_front(&list) == -1 && list_pop_back(&list) == -1) {
		printf("PASS: Pop operations properly handle empty list\n");
	}

	// Test inserting at invalid positions
	printf("\nTesting invalid insertions:\n");
	int value = 42;
	if (list_insert(&list, 1, &value, sizeof(int)) != 0) {
		printf("PASS: Properly rejected out-of-bounds insert\n");
	}

	list_destroy(&list);
	printf("PASS: List destroyed\n");
}

void test_traversal(void) {
	List list;
	printf("\n=== Testing List Traversal ===\n");

	list_init(&list);

	// Add 5 elements
	for (int i = 0; i < 5; i++) {
		list_push_back(&list, &i, sizeof(int));
	}

	// Test forward traversal
	printf("Forward traversal: ");
	for (size_t i = 0; i < list_size(&list); i++) {
		int *val = list_get(&list, i);
		if (val) {
			printf("%d ", *val);
		}
	}
	printf("\nPASS: Forward traversal completed\n");

	list_destroy(&list);
}

int main() {
	printf("Starting List Tests\n");
	printf("==================\n");

	test_basic_operations();
	test_insert_erase();
	test_edge_cases();
	test_traversal();

	printf("\nAll tests completed!\n");
	return 0;
}
