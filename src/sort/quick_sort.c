/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:11:35 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/03/14 11:18:49 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/sort/sort.h>
#include <string.h>
#include <stdlib.h>

static void swap(unsigned char *a, unsigned char *b, size_t elem_size)
{
    unsigned char *temp = malloc(elem_size);
    
    if (!temp)
        return;
        
    memcpy(temp, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, temp, elem_size);
    
    free(temp);
}

static size_t partition(unsigned char *arr, size_t low, size_t high, 
                      size_t elem_size, int (*compare)(const void *, const void *))
{
    // Using the last element as pivot
    unsigned char *pivot = arr + high * elem_size;
    size_t i = low - 1;
    size_t j;
    
    for (j = low; j <= high - 1; j++)
    {
        if (compare(arr + j * elem_size, pivot) <= 0)
        {
            i++;
            swap(arr + i * elem_size, arr + j * elem_size, elem_size);
        }
    }
    swap(arr + (i + 1) * elem_size, arr + high * elem_size, elem_size);
    return (i + 1);
}

static void quick_sort_recursive(unsigned char *arr, size_t low, size_t high,
                               size_t elem_size, int (*compare)(const void *, const void *))
{
    if (low < high)
    {
        // Find pivot element such that
        // elements smaller than pivot are on the left
        // elements greater than pivot are on the right
        size_t pi = partition(arr, low, high, elem_size, compare);
        
        // Recursively sort elements before and after partition
        if (pi > 0)
            quick_sort_recursive(arr, low, pi - 1, elem_size, compare);
        quick_sort_recursive(arr, pi + 1, high, elem_size, compare);
    }
}

/**
 * @brief Implements the quicksort algorithm
 * 
 * @param array Pointer to the array to be sorted
 * @param size Number of elements in the array
 * @param elem_size Size of each element in bytes
 * @param compare Comparison function to use
 */
void quick_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *))
{
    if (!array || size < 2 || !compare)
        return;
        
    quick_sort_recursive((unsigned char *)array, 0, size - 1, elem_size, compare);
}
