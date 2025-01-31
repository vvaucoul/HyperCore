/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:16:58 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:31:29 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/**
 * @brief Dynamic array implementation similar to C++ std::vector
 *
 * The Vector structure provides a generic, dynamically-sized array implementation.
 * It automatically handles memory allocation and resizing as elements are added.
 *
 * @param capacity Current allocated capacity of the vector
 * @param size    Current number of elements in the vector
 * @param data    Pointer to the array of elements
 */
typedef struct {
	int capacity;
	int size;
	void *data;
} Vector;

/**
 * @brief Initialize a new vector with given initial capacity
 *
 * @param vector          Pointer to the vector to initialize
 * @param initial_capacity Initial capacity of the vector (if <= 0, uses default)
 * @return int           0 on success, -1 on failure
 */
int vector_init(Vector *vector, int initial_capacity);

/**
 * @brief Resize the vector's capacity
 *
 * Doubles the current capacity when called. Used internally when
 * more space is needed.
 *
 * @param vector Pointer to the vector to resize
 * @return int  0 on success, -1 on failure
 */
int vector_resize(Vector *vector);

/**
 * @brief Add an element to the end of the vector
 *
 * @param vector       Pointer to the vector
 * @param element      Pointer to the element to add
 * @param element_size Size of the element in bytes
 * @return int        0 on success, -1 on failure
 */
int vector_push_back(Vector *vector, const void *element, size_t element_size);

/**
 * @brief Get element at specified index
 *
 * @param vector Pointer to the vector
 * @param index  Index of the element to retrieve
 * @return void* Pointer to the element, NULL if index is invalid
 */
void *vector_get(Vector *vector, int index);

/**
 * @brief Get current number of elements in the vector
 *
 * @param vector Pointer to the vector
 * @return int  Current size of the vector
 */
int vector_size(Vector *vector);

/**
 * @brief Get current capacity of the vector
 *
 * @param vector Pointer to the vector
 * @return int  Current capacity of the vector
 */
int vector_capacity(Vector *vector);

/**
 * @brief Free all memory used by the vector
 *
 * @param vector Pointer to the vector to destroy
 */
void vector_destroy(Vector *vector);

/**
 * @brief Insert element at specified position
 *
 * @param vector       Pointer to the vector
 * @param index       Position where to insert
 * @param element      Element to insert
 * @param element_size Size of the element in bytes
 * @return int        0 on success, -1 on failure
 */
int vector_insert(Vector *vector, int index, const void *element, size_t element_size);

/**
 * @brief Remove element at specified position
 *
 * @param vector Pointer to the vector
 * @param index  Position of element to remove
 * @return int  0 on success, -1 on failure
 */
int vector_erase(Vector *vector, int index);

/**
 * @brief Remove all elements from the vector
 *
 * @param vector Pointer to the vector
 * @return int  0 on success, -1 on failure
 */
int vector_clear(Vector *vector);

/**
 * @brief Check if vector is empty
 *
 * @param vector Pointer to the vector
 * @return int  1 if empty, 0 if not empty
 */
int vector_empty(Vector *vector);

/**
 * @brief Get first element of the vector
 *
 * @param vector Pointer to the vector
 * @return void* Pointer to first element, NULL if vector is empty
 */
void *vector_front(Vector *vector);

/**
 * @brief Get last element of the vector
 *
 * @param vector Pointer to the vector
 * @return void* Pointer to last element, NULL if vector is empty
 */
void *vector_back(Vector *vector);

#endif // VECTOR_H