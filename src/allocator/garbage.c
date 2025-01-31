/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hc_garbage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/01/30 11:11:47 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 11:32:09 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/garbage.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Internal structure for allocation tracking.
 */
typedef struct GarbageManager {
	GarbageContext context; /**< Garbage manager context */
	bool verbose_mode;		/**< Verbose mode enabled/disabled */
} GarbageManager;

static GarbageManager manager = {0};

/**
 * @brief Initializes a new context for allocation tracking.
 *
 * This function is called by garbage_init().
 */
static void initialize_context(GarbageContext *ctx) {
	ctx->total_allocations		= 0;
	ctx->total_frees			= 0;
	ctx->current_allocated_size = 0;
	ctx->head					= NULL;
	ctx->tail					= NULL;
}

/**
 * @brief Prints a message if verbose mode is enabled.
 */
static void verbose_printf(const char *format, ...) {
	if (manager.verbose_mode) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

GarbageContext *garbage_init(bool verbose_mode) {
	initialize_context(&manager.context);
	manager.verbose_mode = verbose_mode;

	verbose_printf("Initializing garbage manager...\n");
	return &manager.context;
}

void garbage_cleanup(GarbageContext *ctx) {
	MemoryBlock *current = ctx->head;
	MemoryBlock *next;

	while (current != NULL) {
		next = current->next;
		free(current->ptr); // Free the actual allocated memory first
		free(current);		// Then free the block structure
		current = next;
	}

	initialize_context(ctx);
	verbose_printf("Garbage manager cleanup completed.\n");
}

void garbage_add_block(GarbageContext *ctx, void *ptr, size_t size) {
	MemoryBlock *new_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));

	if (new_block == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory for block tracking.\n");
		return;
	}

	new_block->ptr			   = ptr;
	new_block->size			   = size;
	new_block->allocation_time = time(NULL);
	new_block->prev			   = ctx->tail;
	new_block->next			   = NULL;

	if (ctx->head == NULL) {
		ctx->head = new_block;
	} else {
		ctx->tail->next = new_block;
	}

	ctx->tail = new_block;
	ctx->current_allocated_size += size;
	ctx->total_allocations++;

	verbose_printf("New allocation:\n");
	verbose_printf("- Pointer: %p\n", ptr);
	verbose_printf("- Size: %zu bytes\n", size);
}

void garbage_remove_block(GarbageContext *ctx, void *ptr) {
	MemoryBlock *block = garbage_find_block(ctx, ptr);

	if (block == NULL) {
		fprintf(stderr, "Error: Block not found in list.\n");
		return;
	}

	// Print debug info before freeing
	verbose_printf("Block freed:\n");
	verbose_printf("- Pointer: %p\n", ptr);

	// Detach block from linked list
	if (block->prev != NULL) {
		block->prev->next = block->next;
	} else {
		ctx->head = block->next;
	}

	if (block->next != NULL) {
		block->next->prev = block->prev;
	} else {
		ctx->tail = block->prev;
	}

	ctx->current_allocated_size -= block->size;
	ctx->total_frees++;

	// Store pointer value before freeing
	void *freed_ptr = block->ptr;

	// Mark as freed before freeing the block structure
	block->ptr = NULL;

	// Free the block structure
	free(block);

	// Update other blocks to prevent double-free
	MemoryBlock *current = ctx->head;
	while (current != NULL) {
		if (current->ptr == freed_ptr) {
			current->ptr = NULL; // Mark any other references as freed
		}
		current = current->next;
	}
}

MemoryBlock *garbage_find_block(GarbageContext *ctx, void *ptr) {
	MemoryBlock *current = ctx->head;

	while (current != NULL) {
		if (current->ptr == ptr) {
			return current;
		}
		current = current->next;
	}

	return NULL;
}

void garbage_report_usage(GarbageContext *ctx) {
	printf("Memory Report:\n");
	printf("- Total allocations: %zu\n", ctx->total_allocations);
	printf("- Total frees: %zu\n", ctx->total_frees);
	printf("- Current total allocated size: %zu bytes\n", ctx->current_allocated_size);

	if (ctx->head == NULL) {
		printf("No current allocations.\n");
		return;
	}

	printf("\nList of allocated blocks:\n");
	MemoryBlock *current = ctx->head;

	while (current != NULL) {
		printf("- Pointer: %p\n", current->ptr);
		printf("- Size: %zu bytes\n", current->size);
		printf("- Allocation time: %s", ctime(&current->allocation_time));

		current = current->next;
	}
}

void garbage_free_oldest_block(GarbageContext *ctx) {
	if (ctx->head == NULL) {
		verbose_printf("No blocks to free.\n");
		return;
	}

	MemoryBlock *oldest = ctx->head;
	void *ptr			= oldest->ptr;

	verbose_printf("Freeing oldest block:\n");
	verbose_printf("- Pointer: %p\n", ptr);

	garbage_remove_block(ctx, ptr); // This will free both the memory and the block
}

void *gc_malloc(GarbageContext *ctx, size_t size) {
	void *ptr = malloc(size);
	if (ptr != NULL) {
		garbage_add_block(ctx, ptr, size);
	}
	return ptr;
}

void *gc_calloc(GarbageContext *ctx, size_t nmemb, size_t size) {
	void *ptr = calloc(nmemb, size);
	if (ptr != NULL) {
		garbage_add_block(ctx, ptr, nmemb * size);
	}
	return ptr;
}

void *gc_realloc(GarbageContext *ctx, void *ptr, size_t size) {
	if (ptr == NULL) {
		return gc_malloc(ctx, size);
	}

	MemoryBlock *block = garbage_find_block(ctx, ptr);
	if (block == NULL) {
		void *new_ptr = realloc(ptr, size);
		if (new_ptr != NULL) {
			garbage_add_block(ctx, new_ptr, size);
		}
		return new_ptr;
	}

	// Remove the old block before reallocating
	garbage_remove_block(ctx, ptr);

	void *new_ptr = realloc(ptr, size);
	if (new_ptr != NULL) {
		// Add the new block after reallocation
		garbage_add_block(ctx, new_ptr, size);
	}

	return new_ptr;
}

void *gc_strdup(GarbageContext *ctx, const char *str) {
	size_t len = strlen(str) + 1;
	void *ptr  = gc_malloc(ctx, len);
	if (ptr != NULL) {
		memcpy(ptr, str, len);
	}
	return ptr;
}

void gc_free(GarbageContext *ctx, void *ptr) {
	if (ptr == NULL) return;
	garbage_remove_block(ctx, ptr);
}
