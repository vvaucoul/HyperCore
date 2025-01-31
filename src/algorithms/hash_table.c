/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:51:00 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:51:07 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/algorithms/hash_table.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR_THRESHOLD 0.75

static bool resize_table(HashTable *table, size_t new_capacity) {
	HashEntry **new_buckets = calloc(new_capacity, sizeof(HashEntry *));
	if (!new_buckets) return false;

	// Rehash all entries
	for (size_t i = 0; i < table->capacity; i++) {
		HashEntry *entry = table->buckets[i];
		while (entry) {
			HashEntry *next		   = entry->next;
			size_t new_index	   = table->hash_func(entry->key) % new_capacity;
			entry->next			   = new_buckets[new_index];
			new_buckets[new_index] = entry;
			entry				   = next;
		}
	}

	free(table->buckets);
	table->buckets	= new_buckets;
	table->capacity = new_capacity;
	return true;
}

bool hash_table_init(HashTable *table, size_t initial_capacity, size_t key_size, unsigned int (*hash_func)(const void *key), int (*compare_func)(const void *a, const void *b)) {
	table->buckets = calloc(initial_capacity, sizeof(HashEntry *));
	if (!table->buckets) return false;

	table->size			= 0;
	table->capacity		= initial_capacity;
	table->key_size		= key_size;
	table->hash_func	= hash_func;
	table->compare_func = compare_func;
	return true;
}

bool hash_table_insert(HashTable *table, const void *key, const void *value, size_t value_size) {
	// Check load factor and resize if necessary
	if ((float)table->size / table->capacity >= LOAD_FACTOR_THRESHOLD) {
		if (!resize_table(table, table->capacity * 2)) {
			return false;
		}
	}

	unsigned int hash = table->hash_func(key);
	size_t index	  = hash % table->capacity;

	// Check for existing key
	HashEntry *current = table->buckets[index];
	while (current) {
		if (table->compare_func(current->key, key) == 0) {
			// Update existing entry
			void *new_value = malloc(value_size);
			if (!new_value) return false;
			memcpy(new_value, value, value_size);
			free(current->value);
			current->value = new_value;
			return true;
		}
		current = current->next;
	}

	// Create new entry
	HashEntry *new_entry = malloc(sizeof(HashEntry));
	if (!new_entry) return false;

	new_entry->key	 = malloc(table->key_size);
	new_entry->value = malloc(value_size);
	if (!new_entry->key || !new_entry->value) {
		free(new_entry->key);
		free(new_entry->value);
		free(new_entry);
		return false;
	}

	memcpy(new_entry->key, key, table->key_size);
	memcpy(new_entry->value, value, value_size);

	// Insert at beginning of bucket
	new_entry->next		  = table->buckets[index];
	table->buckets[index] = new_entry;
	table->size++;

	return true;
}

bool hash_table_remove(HashTable *table, const void *key) {
	unsigned int hash = table->hash_func(key);
	size_t index	  = hash % table->capacity;

	HashEntry *current = table->buckets[index];
	HashEntry *prev	   = NULL;

	while (current) {
		if (table->compare_func(current->key, key) == 0) {
			if (prev) {
				prev->next = current->next;
			} else {
				table->buckets[index] = current->next;
			}

			free(current->key);
			free(current->value);
			free(current);
			table->size--;
			return true;
		}
		prev	= current;
		current = current->next;
	}

	return false;
}

void *hash_table_find(const HashTable *table, const void *key) {
	unsigned int hash = table->hash_func(key);
	size_t index	  = hash % table->capacity;

	HashEntry *current = table->buckets[index];
	while (current) {
		if (table->compare_func(current->key, key) == 0) {
			return current->value;
		}
		current = current->next;
	}

	return NULL;
}

size_t hash_table_size(const HashTable *table) {
	return table->size;
}

bool hash_table_empty(const HashTable *table) {
	return table->size == 0;
}

void hash_table_clear(HashTable *table) {
	for (size_t i = 0; i < table->capacity; i++) {
		HashEntry *current = table->buckets[i];
		while (current) {
			HashEntry *next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
		table->buckets[i] = NULL;
	}
	table->size = 0;
}

void hash_table_destroy(HashTable *table) {
	hash_table_clear(table);
	free(table->buckets);
	table->buckets	= NULL;
	table->capacity = 0;
}
