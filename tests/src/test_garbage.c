/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_garbage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:27:59 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:23:03 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test helper functions
static void print_test_header(const char *test_name) {
	printf("\n=== Running Test: %s ===\n", test_name);
}

static void assert_not_null(void *ptr, const char *message) {
	if (ptr == NULL) {
		fprintf(stderr, "Assertion failed: %s\n", message);
		exit(1);
	}
}

// Basic allocation tests
static void test_basic_allocations(void) {
	print_test_header("Basic Allocations");

	// Initialize garbage context
	GarbageContext *ctx = garbage_init(true);
	assert_not_null(ctx, "Failed to initialize garbage collector");

	// Test malloc
	int *num = gc_malloc(ctx, sizeof(int));
	assert_not_null(num, "gc_malloc failed");
	*num = 42;

	// Test calloc
	int *array = gc_calloc(ctx, 5, sizeof(int));
	assert_not_null(array, "gc_calloc failed");
	assert(array[0] == 0 && array[4] == 0); // Verify initialization

	// Test strdup
	const char *original = "Hello, World!";
	char *copy			 = gc_strdup(ctx, original);
	assert_not_null(copy, "gc_strdup failed");
	assert(strcmp(original, copy) == 0);

	garbage_report_usage(ctx);

	// Cleanup
	garbage_cleanup(ctx);
}

// Reallocation tests
static void test_reallocation(void) {
	print_test_header("Reallocation");

	// Initialize garbage context
	GarbageContext *ctx = garbage_init(true);
	assert_not_null(ctx, "Failed to initialize garbage collector");

	// Test realloc with NULL
	int *ptr = gc_realloc(ctx, NULL, sizeof(int) * 5);
	assert_not_null(ptr, "gc_realloc with NULL failed");

	// Test realloc with existing pointer
	ptr = gc_realloc(ctx, ptr, sizeof(int) * 10);
	assert_not_null(ptr, "gc_realloc with existing pointer failed");

	// Test realloc with size 0 (should free)
	ptr = gc_realloc(ctx, ptr, 0);
	assert(ptr == NULL);

	garbage_report_usage(ctx);

	// Cleanup
	garbage_cleanup(ctx);
}

// Stress test
static void test_stress(void) {
	print_test_header("Stress Test");

	// Initialize garbage context
	GarbageContext *ctx = garbage_init(true);
	assert_not_null(ctx, "Failed to initialize garbage collector");

#define STRESS_COUNT 1000
	void *ptrs[STRESS_COUNT];

	// Allocate many blocks
	for (int i = 0; i < STRESS_COUNT; i++) {
		size_t size = (rand() % 1000) + 1;
		ptrs[i]		= gc_malloc(ctx, size);
		assert_not_null(ptrs[i], "Stress test allocation failed");
	}

	// Free half randomly
	for (int i = 0; i < STRESS_COUNT / 2; i++) {
		int idx = rand() % STRESS_COUNT;
		if (ptrs[idx] != NULL) {
			gc_free(ctx, ptrs[idx]);
			ptrs[idx] = NULL;
		}
	}

	// Reallocate remaining blocks
	for (int i = 0; i < STRESS_COUNT; i++) {
		if (ptrs[i] != NULL) {
			size_t new_size = (rand() % 1000) + 1;
			ptrs[i]			= gc_realloc(ctx, ptrs[i], new_size);
		}
	}

	garbage_report_usage(ctx);

	// Cleanup
	garbage_cleanup(ctx);
}

// Edge cases test
static void test_edge_cases(void) {
	print_test_header("Edge Cases");

	// Initialize garbage context
	GarbageContext *ctx = garbage_init(true);
	assert_not_null(ctx, "Failed to initialize garbage collector");

	// Test zero-size allocation
	void *ptr = gc_malloc(ctx, 0);
	assert(ptr != NULL); // Some implementations return valid ptr for size 0

	// Test NULL pointer free
	gc_free(ctx, NULL); // Should not crash

	// Test large allocation
	ptr = gc_malloc(ctx, 1024 * 1024 * 10); // 10MB
	assert_not_null(ptr, "Large allocation failed");

	// Test multiple frees (should handle gracefully)
	gc_free(ctx, ptr);
	gc_free(ctx, ptr); // Should print error but not crash

	garbage_report_usage(ctx);

	// Cleanup
	garbage_cleanup(ctx);
}

// Oldest block management test
static void test_oldest_block_management(void) {
	print_test_header("Oldest Block Management");

	// Initialize garbage context
	GarbageContext *ctx = garbage_init(true);
	assert_not_null(ctx, "Failed to initialize garbage collector");

	// Allocate several blocks
	int *block1 = gc_malloc(ctx, sizeof(int));
	int *block2 = gc_malloc(ctx, sizeof(int));
	int *block3 = gc_malloc(ctx, sizeof(int));

	// Free oldest block (should be block1)
	garbage_free_oldest_block(ctx);
	assert(garbage_find_block(ctx, block1) == NULL);

	// Free oldest block again (should be block2)
	garbage_free_oldest_block(ctx);
	assert(garbage_find_block(ctx, block2) == NULL);

	// Free remaining block
	garbage_free_oldest_block(ctx);
	assert(garbage_find_block(ctx, block3) == NULL);

	garbage_report_usage(ctx);

	// Cleanup
	garbage_cleanup(ctx);
}

int main(void) {
	// Run all tests independently
	test_basic_allocations();
	test_reallocation();
	test_stress();
	test_edge_cases();
	test_oldest_block_management();

	printf("\n=== All tests completed successfully ===\n");
	return 0;
}
