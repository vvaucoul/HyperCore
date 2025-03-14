/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:11:48 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/03/14 11:21:04 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
# define SORT_H

#include <stdlib.h>

/* Sorting algorithms */
void bubble_sort(void *array, size_t size, size_t elem_size,
                int (*compare)(const void *, const void *));
                
void insertion_sort(void *array, size_t size, size_t elem_size,
                   int (*compare)(const void *, const void *));
                   
void merge_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *));
               
void quick_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *));
               
void radix_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *));
               
/* Specialized radix sort for integers */
void radix_sort_int(int *array, size_t size);

#endif /* SORT_H */