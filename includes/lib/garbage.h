/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hc_garbage.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:11:52 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 11:34:52 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_H
#define GARBAGE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Structure representing an allocated memory trace.
 */
typedef struct MemoryBlock {
	void *ptr;				  /**< Pointer to allocated memory */
	size_t size;			  /**< Size of allocated memory */
	time_t allocation_time;	  /**< Allocation timestamp */
	struct MemoryBlock *prev; /**< Pointer to previous block */
	struct MemoryBlock *next; /**< Pointer to next block */
} MemoryBlock;

/**
 * @brief Structure representing the garbage manager context.
 */
typedef struct GarbageContext {
	size_t total_allocations;	   /**< Total number of allocations made */
	size_t total_frees;			   /**< Total number of frees made */
	size_t current_allocated_size; /**< Current total allocated memory size */
	MemoryBlock *head;			   /**< First block in the linked list */
	MemoryBlock *tail;			   /**< Last block in the linked list */
} GarbageContext;

/**
 * @brief Initialize the garbage manager.
 *
 * Initializes a new context for tracking allocations and frees.
 *
 * @param[in] verbose_mode Enable/disable verbose mode (message display)
 * @return GarbageContext* Pointer to initialized context, or NULL on error
 */
GarbageContext *garbage_init(bool verbose_mode);

/**
 * @brief Free allocated memory and release garbage manager resources.
 *
 * Cleans up all previous allocations and frees the context.
 *
 * @param[in] ctx Garbage manager context
 */
void garbage_cleanup(GarbageContext *ctx);

/**
 * @brief Add a memory block to the list of allocated blocks.
 *
 * Used to notify the manager that an allocation has occurred.
 *
 * @param[in] ctx Garbage manager context
 * @param[in] ptr Pointer to allocated memory
 * @param[in] size Size of allocation
 */
void garbage_add_block(GarbageContext *ctx, void *ptr, size_t size);

/**
 * @brief Remove a memory block from the list of allocated blocks.
 *
 * Used to notify the manager that a free has occurred.
 *
 * @param[in] ctx Garbage manager context
 * @param[in] ptr Pointer to memory to free
 */
void garbage_remove_block(GarbageContext *ctx, void *ptr);

/**
 * @brief Display a report on memory usage.
 *
 * Shows all current allocations with their sizes and timestamps.
 *
 * @param[in] ctx Garbage manager context
 */
void garbage_report_usage(GarbageContext *ctx);

/**
 * @brief Search for a memory block by its pointer.
 *
 * Returns the block corresponding to the specified pointer, or NULL if not found.
 *
 * @param[in] ctx Garbage manager context
 * @param[in] ptr Pointer to search for
 * @return MemoryBlock* Found block, or NULL if not found
 */
MemoryBlock *garbage_find_block(GarbageContext *ctx, void *ptr);

/**
 * @brief Free the oldest memory block.
 *
 * Used for manual object lifecycle management.
 *
 * @param[in] ctx Garbage manager context
 */
void garbage_free_oldest_block(GarbageContext *ctx);

// Memory allocation wrappers
void *gc_malloc(GarbageContext *ctx, size_t size);
void *gc_calloc(GarbageContext *ctx, size_t nmemb, size_t size);
void *gc_realloc(GarbageContext *ctx, void *ptr, size_t size);
void gc_free(GarbageContext *ctx, void *ptr);
void *gc_strdup(GarbageContext *ctx, const char *str);

#endif // GARBAGE_H
