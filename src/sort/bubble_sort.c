/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bubble_sort.c                                      :+:      :+:    :+:   */
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
 * @brief Implements the bubble sort algorithm
 * 
 * @param array Pointer to the array to be sorted
 * @param size Number of elements in the array
 * @param elem_size Size of each element in bytes
 * @param compare Comparison function to use
 */
void bubble_sort(void *array, size_t size, size_t elem_size,
                int (*compare)(const void *, const void *))
{
    unsigned char *arr = (unsigned char *)array;
    unsigned char *temp = NULL;
    size_t i, j;
    int swapped;

    if (!array || size < 2 || !compare || !(temp = (unsigned char *)malloc(elem_size)))
        return;

    for (i = 0; i < size - 1; i++)
    {
        swapped = 0;
        for (j = 0; j < size - i - 1; j++)
        {
            if (compare(arr + j * elem_size, arr + (j + 1) * elem_size) > 0)
            {
                // Swap elements
                memcpy(temp, arr + j * elem_size, elem_size);
                memcpy(arr + j * elem_size, arr + (j + 1) * elem_size, elem_size);
                memcpy(arr + (j + 1) * elem_size, temp, elem_size);
                swapped = 1;
            }
        }

        // If no swapping occurred in this pass, array is sorted
        if (!swapped)
            break;
    }

    free(temp);
}
