/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:39:03 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:39:03 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
#define MAP_H

#include <stddef.h>

/**
 * @brief Map entry structure for key-value pairs
 */
typedef struct MapEntry {
	void *key;
	void *value;
	struct MapEntry *next;
} MapEntry;

/**
 * @brief Hash map implementation
 *
 * @param buckets Array of bucket pointers
 * @param size    Current number of elements
 * @param capacity Number of buckets
 * @param key_size Size of key type
 * @param hash_func Function to hash keys
 * @param compare_func Function to compare keys
 */
typedef struct {
	MapEntry **buckets;
	size_t size;
	size_t capacity;
	size_t key_size;
	size_t (*hash_func)(const void *key);
	int (*compare_func)(const void *key1, const void *key2);
} Map;

/**
 * @brief Initialize a new map
 *
 * @param initial_capacity Initial number of buckets
 * @param key_size Size of key type in bytes
 * @param hash_func Function to hash keys
 * @param compare_func Function to compare keys
 */
int map_init(Map *map, size_t initial_capacity, size_t key_size, size_t (*hash_func)(const void *), int (*compare_func)(const void *, const void *));

/**
 * @brief Insert or update key-value pair
 */
int map_insert(Map *map, const void *key, const void *value, size_t value_size);

/**
 * @brief Get value associated with key
 */
void *map_get(Map *map, const void *key);

/**
 * @brief Remove key-value pair
 */
int map_erase(Map *map, const void *key);

/**
 * @brief Check if key exists in map
 */
int map_contains(Map *map, const void *key);

/**
 * @brief Get current number of elements
 */
size_t map_size(Map *map);

/**
 * @brief Check if map is empty
 */
int map_empty(Map *map);

/**
 * @brief Remove all elements
 */
void map_clear(Map *map);

/**
 * @brief Destroy map and free memory
 */
void map_destroy(Map *map);

// Common hash functions
size_t map_hash_string(const void *key);
size_t map_hash_int(const void *key);

// Common compare functions
int map_compare_string(const void *key1, const void *key2);
int map_compare_int(const void *key1, const void *key2);

#endif // MAP_H
