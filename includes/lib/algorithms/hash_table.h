/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:50:59 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:50:59 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Hash table entry structure
 */
typedef struct HashEntry {
	void *key;
	void *value;
	struct HashEntry *next;
} HashEntry;

/**
 * @brief Hash table structure
 */
typedef struct {
	HashEntry **buckets;
	size_t size;
	size_t capacity;
	size_t key_size;
	unsigned int (*hash_func)(const void *key);
	int (*compare_func)(const void *a, const void *b);
} HashTable;

/**
 * @brief Initialize a new hash table
 * @param table Pointer to hash table structure
 * @param initial_capacity Initial number of buckets
 * @param key_size Size of key type in bytes
 * @param hash_func Hash function for keys
 * @param compare_func Comparison function for keys
 * @return bool true on success, false on failure
 */
bool hash_table_init(HashTable *table, size_t initial_capacity, size_t key_size, unsigned int (*hash_func)(const void *key), int (*compare_func)(const void *a, const void *b));

/**
 * @brief Insert a key-value pair into the hash table
 * @return bool true on success, false on failure
 */
bool hash_table_insert(HashTable *table, const void *key, const void *value, size_t value_size);

/**
 * @brief Remove an entry from the hash table
 * @return bool true if found and removed, false if not found
 */
bool hash_table_remove(HashTable *table, const void *key);

/**
 * @brief Find a value by its key
 * @return void* pointer to value if found, NULL if not found
 */
void *hash_table_find(const HashTable *table, const void *key);

/**
 * @brief Get current number of entries in the hash table
 */
size_t hash_table_size(const HashTable *table);

/**
 * @brief Check if hash table is empty
 */
bool hash_table_empty(const HashTable *table);

/**
 * @brief Clear all entries from the hash table
 */
void hash_table_clear(HashTable *table);

/**
 * @brief Destroy the hash table and free all resources
 */
void hash_table_destroy(HashTable *table);

#endif // HASH_TABLE_H
