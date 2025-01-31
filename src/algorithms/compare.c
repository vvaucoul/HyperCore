/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:55:51 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 19:55:51 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/algorithms/compare.h"
#include <ctype.h>
#include <string.h>

// Basic type comparisons
int compare_char(const void *a, const void *b) {
	return (*(const char *)a - *(const char *)b);
}

int compare_uchar(const void *a, const void *b) {
	return (*(const unsigned char *)a - *(const unsigned char *)b);
}

int compare_short(const void *a, const void *b) {
	return (*(const short *)a - *(const short *)b);
}

int compare_ushort(const void *a, const void *b) {
	const unsigned short va = *(const unsigned short *)a;
	const unsigned short vb = *(const unsigned short *)b;
	return (va < vb) ? -1 : (va > vb);
}

int compare_int(const void *a, const void *b) {
	return (*(const int *)a - *(const int *)b);
}

int compare_uint(const void *a, const void *b) {
	const unsigned int va = *(const unsigned int *)a;
	const unsigned int vb = *(const unsigned int *)b;
	return (va < vb) ? -1 : (va > vb);
}

int compare_long(const void *a, const void *b) {
	const long va = *(const long *)a;
	const long vb = *(const long *)b;
	return (va < vb) ? -1 : (va > vb);
}

int compare_ulong(const void *a, const void *b) {
	const unsigned long va = *(const unsigned long *)a;
	const unsigned long vb = *(const unsigned long *)b;
	return (va < vb) ? -1 : (va > vb);
}

int compare_float(const void *a, const void *b) {
	const float va = *(const float *)a;
	const float vb = *(const float *)b;
	if (va < vb) return -1;
	if (va > vb) return 1;
	return 0;
}

int compare_double(const void *a, const void *b) {
	const double va = *(const double *)a;
	const double vb = *(const double *)b;
	if (va < vb) return -1;
	if (va > vb) return 1;
	return 0;
}

// String comparisons
int compare_str(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

int compare_strn(const void *a, const void *b, size_t n) {
	return strncmp(*(const char **)a, *(const char **)b, n);
}

int compare_str_case(const void *a, const void *b) {
	const char *s1 = *(const char **)a;
	const char *s2 = *(const char **)b;

	while (*s1 && *s2) {
		int c1 = tolower((unsigned char)*s1);
		int c2 = tolower((unsigned char)*s2);
		if (c1 != c2) return c1 - c2;
		s1++;
		s2++;
	}
	return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

int compare_strn_case(const void *a, const void *b, size_t n) {
	const char *s1 = *(const char **)a;
	const char *s2 = *(const char **)b;

	while (n > 0 && *s1 && *s2) {
		int c1 = tolower((unsigned char)*s1);
		int c2 = tolower((unsigned char)*s2);
		if (c1 != c2) return c1 - c2;
		s1++;
		s2++;
		n--;
	}
	return (n == 0) ? 0 : (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
}

// Memory comparison
int compare_mem(const void *a, const void *b, size_t size) {
	return memcmp(a, b, size);
}

// Pointer comparison
int compare_ptr(const void *a, const void *b) {
	return (a < b) ? -1 : (a > b);
}

// Array comparison
int compare_array(const void *a, const void *b, size_t elem_size, size_t length, compare_func_t cmp) {
	const char *pa = a;
	const char *pb = b;

	for (size_t i = 0; i < length; i++) {
		int result = cmp(pa + (i * elem_size), pb + (i * elem_size));
		if (result != 0) return result;
	}
	return 0;
}
