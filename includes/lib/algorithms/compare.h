/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:55:53 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 10:39:04 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPARE_H
#define COMPARE_H

#include <stddef.h>

/**
 * @brief Comparison functions return values
 * Return < 0 if a < b
 * Return 0 if a == b
 * Return > 0 if a > b
 */

/**
 * @brief Basic type comparison functions
 * All functions follow the standard comparison contract:
 * Returns negative if a < b, 0 if a == b, positive if a > b
 */

/**
 * @brief Compare two char values
 * @param a Pointer to first char
 * @param b Pointer to second char
 * @return int Comparison result
 */
int compare_char(const void *a, const void *b);

/**
 * @brief Compare two unsigned char values
 * @param a Pointer to first unsigned char
 * @param b Pointer to second unsigned char
 * @return int Comparison result
 */
int compare_uchar(const void *a, const void *b);

/**
 * @brief Compare two short values
 * @param a Pointer to first short
 * @param b Pointer to second short
 * @return int Comparison result
 */
int compare_short(const void *a, const void *b);

/**
 * @brief Compare two unsigned short values
 * @param a Pointer to first unsigned short
 * @param b Pointer to second unsigned short
 * @return int Comparison result
 */
int compare_ushort(const void *a, const void *b);

/**
 * @brief Compare two int values
 * @param a Pointer to first int
 * @param b Pointer to second int
 * @return int Comparison result
 */
int compare_int(const void *a, const void *b);

/**
 * @brief Compare two unsigned int values
 * @param a Pointer to first unsigned int
 * @param b Pointer to second unsigned int
 * @return int Comparison result
 */
int compare_uint(const void *a, const void *b);

/**
 * @brief Compare two long values
 * @param a Pointer to first long
 * @param b Pointer to second long
 * @return int Comparison result
 */
int compare_long(const void *a, const void *b);

/**
 * @brief Compare two unsigned long values
 * @param a Pointer to first unsigned long
 * @param b Pointer to second unsigned long
 * @return int Comparison result
 */
int compare_ulong(const void *a, const void *b);

/**
 * @brief Compare two float values
 * @param a Pointer to first float
 * @param b Pointer to second float
 * @return int Comparison result
 */
int compare_float(const void *a, const void *b);

/**
 * @brief Compare two double values
 * @param a Pointer to first double
 * @param b Pointer to second double
 * @return int Comparison result
 */
int compare_double(const void *a, const void *b);

/**
 * @brief String comparison functions
 */

/**
 * @brief Compare two null-terminated strings
 * @param a Pointer to first string
 * @param b Pointer to second string
 * @return int Comparison result
 */
int compare_str(const void *a, const void *b);

/**
 * @brief Compare two strings up to n characters
 * @param a Pointer to first string
 * @param b Pointer to second string
 * @param n Maximum number of characters to compare
 * @return int Comparison result
 */
int compare_strn(const void *a, const void *b, size_t n);

/**
 * @brief Case-insensitive string comparison
 * @param a Pointer to first string
 * @param b Pointer to second string
 * @return int Comparison result
 */
int compare_str_case(const void *a, const void *b);

/**
 * @brief Case-insensitive string comparison up to n characters
 * @param a Pointer to first string
 * @param b Pointer to second string
 * @param n Maximum number of characters to compare
 * @return int Comparison result
 */
int compare_strn_case(const void *a, const void *b, size_t n);

/**
 * @brief Memory comparison
 * @param a Pointer to first memory block
 * @param b Pointer to second memory block
 * @param size Size of memory blocks to compare
 * @return int Comparison result
 */
int compare_mem(const void *a, const void *b, size_t size);

/**
 * @brief Compare two pointers by their address values
 * @param a First pointer
 * @param b Second pointer
 * @return int Comparison result
 */
int compare_ptr(const void *a, const void *b);

/**
 * @brief Type definition for comparison function pointer
 */
typedef int (*compare_func_t)(const void *, const void *);

/**
 * @brief Compare two arrays using a custom comparison function
 * @param a Pointer to first array
 * @param b Pointer to second array
 * @param elem_size Size of each element
 * @param length Number of elements to compare
 * @param cmp Comparison function for individual elements
 * @return int Comparison result
 */
int compare_array(const void *a, const void *b, size_t elem_size, size_t length, compare_func_t cmp);

#endif // COMPARE_H
