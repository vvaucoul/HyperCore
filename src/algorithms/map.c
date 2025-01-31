/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:39:04 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:39:05 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/algorithms/map.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

static MapEntry *create_entry(const void *key, size_t key_size, const void *value, size_t value_size) {
	MapEntry *entry = malloc(sizeof(MapEntry));
	if (!entry) {
		return NULL;
	}

	entry->key = malloc(key_size);
	if (!entry->key) {
		free(entry);
		return NULL;
	}

	entry->value = malloc(value_size);
	if (!entry->value) {
		free(entry->key);
		free(entry);
		return NULL;
	}

	memcpy(entry->key, key, key_size);
	memcpy(entry->value, value, value_size);
	entry->next = NULL;
	return entry;
}

int map_init(Map *map, size_t initial_capacity, size_t key_size, size_t (*hash_func)(const void *), int (*compare_func)(const void *, const void *)) {
	if (initial_capacity == 0) {
		initial_capacity = INITIAL_CAPACITY;
	}

	map->buckets = calloc(initial_capacity, sizeof(MapEntry *));
	if (!map->buckets) {
		return -1;
	}

	map->capacity	  = initial_capacity;
	map->size		  = 0;
	map->key_size	  = key_size;
	map->hash_func	  = hash_func;
	map->compare_func = compare_func;
	return 0;
}

static int map_resize(Map *map) {
	size_t new_capacity	   = map->capacity * 2;
	MapEntry **new_buckets = calloc(new_capacity, sizeof(MapEntry *));
	if (!new_buckets) {
		return -1;
	}

	// Rehash all entries
	for (size_t i = 0; i < map->capacity; i++) {
		MapEntry *entry = map->buckets[i];
		while (entry) {
			MapEntry *next		   = entry->next;
			size_t new_index	   = map->hash_func(entry->key) % new_capacity;
			entry->next			   = new_buckets[new_index];
			new_buckets[new_index] = entry;
			entry				   = next;
		}
	}

	free(map->buckets);
	map->buckets  = new_buckets;
	map->capacity = new_capacity;
	return 0;
}

int map_insert(Map *map, const void *key, const void *value, size_t value_size) {
	if ((float)map->size / map->capacity >= LOAD_FACTOR_THRESHOLD) {
		if (map_resize(map) != 0) {
			return -1;
		}
	}

	size_t index	= map->hash_func(key) % map->capacity;
	MapEntry *entry = map->buckets[index];

	// Check if key already exists
	while (entry) {
		if (map->compare_func(entry->key, key) == 0) {
			// Update value
			void *new_value = realloc(entry->value, value_size);
			if (!new_value) {
				return -1;
			}
			entry->value = new_value;
			memcpy(entry->value, value, value_size);
			return 0;
		}
		entry = entry->next;
	}

	// Create new entry
	MapEntry *new_entry = create_entry(key, map->key_size, value, value_size);
	if (!new_entry) {
		return -1;
	}

	new_entry->next		= map->buckets[index];
	map->buckets[index] = new_entry;
	map->size++;
	return 0;
}

void *map_get(Map *map, const void *key) {
	size_t index	= map->hash_func(key) % map->capacity;
	MapEntry *entry = map->buckets[index];

	while (entry) {
		if (map->compare_func(entry->key, key) == 0) {
			return entry->value;
		}
		entry = entry->next;
	}

	return NULL;
}

int map_erase(Map *map, const void *key) {
	size_t index	= map->hash_func(key) % map->capacity;
	MapEntry *entry = map->buckets[index];
	MapEntry *prev	= NULL;

	while (entry) {
		if (map->compare_func(entry->key, key) == 0) {
			if (prev) {
				prev->next = entry->next;
			} else {
				map->buckets[index] = entry->next;
			}

			free(entry->key);
			free(entry->value);
			free(entry);
			map->size--;
			return 0;
		}
		prev  = entry;
		entry = entry->next;
	}

	return -1;
}

int map_contains(Map *map, const void *key) {
	return map_get(map, key) != NULL;
}

size_t map_size(Map *map) {
	return map->size;
}

int map_empty(Map *map) {
	return map->size == 0;
}

void map_clear(Map *map) {
	for (size_t i = 0; i < map->capacity; i++) {
		MapEntry *entry = map->buckets[i];
		while (entry) {
			MapEntry *next = entry->next;
			free(entry->key);
			free(entry->value);
			free(entry);
			entry = next;
		}
		map->buckets[i] = NULL;
	}
	map->size = 0;
}

void map_destroy(Map *map) {
	map_clear(map);
	free(map->buckets);
	map->buckets  = NULL;
	map->capacity = 0;
}

// Common hash functions
size_t map_hash_string(const void *key) {
	const char *str = (const char *)key;
	size_t hash		= 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

size_t map_hash_int(const void *key) {
	return *(const int *)key;
}

// Common compare functions
int map_compare_string(const void *key1, const void *key2) {
	return strcmp((const char *)key1, (const char *)key2);
}

int map_compare_int(const void *key1, const void *key2) {
	return (*(const int *)key1) - (*(const int *)key2);
}
