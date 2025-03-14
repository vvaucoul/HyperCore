/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:11:35 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/03/14 11:18:50 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/sort/sort.h>
#include <string.h>

/**
 * @brief Implements the insertion sort algorithm
 * 
 * @param array Pointer to the array to be sorted
 * @param size Number of elements in the array
 * @param elem_size Size of each element in bytes
 * @param compare Comparison function to use
 */
void insertion_sort(void *array, size_t size, size_t elem_size,
                   int (*compare)(const void *, const void *))
{
    unsigned char *arr = (unsigned char *)array;
    unsigned char *key = NULL;
    int j;
    size_t i;

    if (!array || size < 2 || !compare || !(key = (unsigned char *)malloc(elem_size)))
        return;

    for (i = 1; i < size; i++)
    {
        // Store current element as key
        memcpy(key, arr + i * elem_size, elem_size);
        
        // Move elements greater than key one position ahead
        j = i - 1;
        while (j >= 0 && compare(arr + j * elem_size, key) > 0)
        {
            memcpy(arr + (j + 1) * elem_size, arr + j * elem_size, elem_size);
            j--;
        }
        
        // Place key at its correct position
        memcpy(arr + (j + 1) * elem_size, key, elem_size);
    }

    free(key);
}
