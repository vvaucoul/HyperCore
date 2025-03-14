/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radix_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:11:35 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/03/14 11:18:49 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/sort/sort.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Finds the maximum value in an array of integers
 * 
 * @param arr Array of integers
 * @param n Size of the array
 * @return The maximum value in the array
 */
static int get_max(int *arr, size_t n)
{
    int max = arr[0];
    for (size_t i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

/**
 * @brief Performs counting sort on the given array according to the digit
 *        represented by exp (where exp is 10^i for i-th digit)
 * 
 * @param arr Array to be sorted
 * @param n Size of the array
 * @param exp Current digit's place value
 * @param output Temporary output array
 */
static void counting_sort(int *arr, size_t n, int exp, int *output)
{
    int count[10] = {0};
    size_t i;
    
    // Count occurrences of each digit
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
    
    // Change count[i] so that count[i] contains actual position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    // Build the output array
    for (i = n - 1; i < n; i--) // Using i < n to handle case where i is size_t and wraps around
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        if (i == 0) break; // Break when i becomes 0, as size_t can't be negative
    }
    
    // Copy the output array to arr[]
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

/**
 * @brief Implements radix sort for an array of integers
 * 
 * @param array Array of integers to be sorted
 * @param size Number of elements in the array
 * 
 * @note This function only works for integer arrays. For other types,
 *       please use a different sorting algorithm.
 */
void radix_sort_int(int *array, size_t size)
{
    if (!array || size < 2)
        return;
    
    // Find the maximum number to know number of digits
    int max = get_max(array, size);
    
    // Create a temporary output array
    int *output = (int *)malloc(size * sizeof(int));
    if (!output)
        return;
    
    // Do counting sort for every digit
    for (int exp = 1; max / exp > 0; exp *= 10)
        counting_sort(array, size, exp, output);
    
    free(output);
}

/**
 * @brief A wrapper for radix sort that only works on integer arrays
 * 
 * @param array Pointer to the array to be sorted
 * @param size Number of elements in the array
 * @param elem_size Size of each element in bytes
 * @param compare Comparison function (unused, only for interface consistency)
 * 
 * @note This function checks if the element size is sizeof(int) and only
 *       proceeds if it is. For other types, it does nothing.
 */
void radix_sort(void *array, size_t size, size_t elem_size,
               int (*compare)(const void *, const void *))
{
    (void)compare; // Unused parameter
    
    if (!array || size < 2)
        return;
    
    // Radix sort only works for integers in this implementation
    if (elem_size == sizeof(int))
    {
        radix_sort_int((int *)array, size);
    }
    // For other types, just return without sorting
}
