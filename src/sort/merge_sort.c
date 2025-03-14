/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:11:35 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/03/14 11:18:48 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/sort/sort.h>
#include <stdlib.h>
#include <string.h>

static void merge(unsigned char *arr, size_t left, size_t mid, 
                 size_t right, size_t elem_size,
                 int (*compare)(const void *, const void *))
{
    size_t i, j, k;
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    // Create temporary arrays
    unsigned char *L = malloc(n1 * elem_size);
    unsigned char *R = malloc(n2 * elem_size);
    
    if (!L || !R)
    {
        if (L)
            free(L);
        if (R)
            free(R);
        return;
    }
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        memcpy(L + i * elem_size, arr + (left + i) * elem_size, elem_size);
    for (j = 0; j < n2; j++)
        memcpy(R + j * elem_size, arr + (mid + 1 + j) * elem_size, elem_size);
    
    // Merge the temporary arrays back
    i = 0;
    j = 0;
    k = left;
    
    while (i < n1 && j < n2)
    {
        if (compare(L + i * elem_size, R + j * elem_size) <= 0)
        {
            memcpy(arr + k * elem_size, L + i * elem_size, elem_size);
            i++;
        }
        else
        {
            memcpy(arr + k * elem_size, R + j * elem_size, elem_size);
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[]
    while (i < n1)
    {
        memcpy(arr + k * elem_size, L + i * elem_size, elem_size);
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[]
    while (j < n2)
    {
        memcpy(arr + k * elem_size, R + j * elem_size, elem_size);
        j++;
        k++;
    }
    
    // Free temporary arrays
    free(L);
    free(R);
}

static void merge_sort_recursive(unsigned char *arr, size_t left, size_t right,
                               size_t elem_size, int (*compare)(const void *, const void *))
{
    if (left < right)
    {
        size_t mid = left + (right - left) / 2;
        
        // Sort first and second halves
        merge_sort_recursive(arr, left, mid, elem_size, compare);
        merge_sort_recursive(arr, mid + 1, right, elem_size, compare);
        
        // Merge the sorted halves
        merge(arr, left, mid, right, elem_size, compare);
    }
}

/**
 * @brief Implements the merge sort algorithm
 * 
 * @param array Pointer to the array to be sorted
 * @param size Number of elements in the array
 * @param elem_size Size of each element in bytes
 * @param compare Comparison function to use
 */
void merge_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *))
{
    if (!array || size < 2 || !compare)
        return;
        
    merge_sort_recursive((unsigned char *)array, 0, size - 1, elem_size, compare);
}
