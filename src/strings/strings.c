/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:54:54 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 12:48:25 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lib/strings/strings.h"
#include <stdio.h>
#include <stdlib.h>

/* Basic string operations */
size_t str_len(const char *str) {
	size_t len = 0;
	while (str && str[len])
		len++;
	return len;
}

size_t str_nlen(const char *str, size_t maxlen) {
	size_t len = 0;
	while (str && str[len] && len < maxlen)
		len++;
	return len;
}

bool str_empty(const char *str) {
	return (!str || *str == '\0');
}

char *str_new(const char *str) {
	if (!str)
		return NULL;
	size_t len = str_len(str);
	char *new  = malloc(len + 1);
	if (!new)
		return NULL;
	return str_copy(new, str);
}

void str_clear(char *str) {
	if (str)
		*str = '\0';
}

char *str_clone(const char *str) {
	return str_new(str);
}

/* Comparison functions */
int str_cmp(const char *s1, const char *s2) {
	if (!s1 || !s2)
		return (s1 == s2) ? 0 : (s1 ? 1 : -1);
	while (*s1 && *s2 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int str_ncmp(const char *s1, const char *s2, size_t n) {
	if (!n || !s1 || !s2)
		return 0;
	while (--n && *s1 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int str_case_cmp(const char *s1, const char *s2) {
	unsigned char c1, c2;
	if (!s1 || !s2)
		return (s1 == s2) ? 0 : (s1 ? 1 : -1);
	do {
		c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + ('a' - 'A') : *s1;
		c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + ('a' - 'A') : *s2;
		if (!c1)
			break;
		s1++;
		s2++;
	} while (c1 == c2);
	return c1 - c2;
}

bool str_equals(const char *s1, const char *s2) {
	return str_cmp(s1, s2) == 0;
}

bool str_starts_with(const char *str, const char *prefix) {
	if (!str || !prefix)
		return false;
	return str_ncmp(str, prefix, str_len(prefix)) == 0;
}

bool str_ends_with(const char *str, const char *suffix) {
	if (!str || !suffix)
		return false;
	size_t str_len_val = str_len(str);
	size_t suffix_len  = str_len(suffix);
	if (suffix_len > str_len_val)
		return false;
	return str_cmp(str + str_len_val - suffix_len, suffix) == 0;
}

/* Copy and concatenation */
char *str_copy(char *dest, const char *src) {
	char *d = dest;
	if (!dest || !src)
		return dest;
	while ((*d++ = *src++))
		;
	return dest;
}

char *str_ncopy(char *dest, const char *src, size_t n) {
	if (!dest || !src)
		return dest;

	size_t i;
	for (i = 0; i < n && src[i]; i++)
		dest[i] = src[i];
	for (; i < n; i++)
		dest[i] = '\0';

	return dest;
}

char *str_cat(char *dest, const char *src) {
	char *d = dest;
	if (!dest || !src)
		return dest;
	while (*d)
		d++;
	while ((*d++ = *src++))
		;
	return dest;
}

/* Additional Copy and Concatenation Functions */
char *str_ncat(char *dest, const char *src, size_t n) {
	char *d = dest;
	if (!dest || !src)
		return dest;
	while (*d)
		d++;
	while (n-- && *src)
		*d++ = *src++;
	*d = '\0';
	return dest;
}

size_t str_lcpy(char *dest, const char *src, size_t size) {
	size_t src_len = str_len(src);
	if (size) {
		size_t n = (src_len < size - 1) ? src_len : (size - 1);
		str_ncopy(dest, src, n);
		dest[n] = '\0';
	}
	return src_len;
}

size_t str_lcat(char *dest, const char *src, size_t size) {
	size_t dest_len = str_len(dest);
	size_t src_len	= str_len(src);

	if (size > dest_len + 1) {
		size_t n = size - dest_len - 1;
		str_ncat(dest, src, n);
	}
	return dest_len + src_len;
}

/* Search functions */
char *str_chr(const char *str, int c) {
	if (!str)
		return NULL;
	while (*str && *str != (char)c)
		str++;
	return (*str == (char)c) ? (char *)str : NULL;
}

char *str_rchr(const char *str, int c) {
	const char *last = NULL;
	if (!str)
		return NULL;
	while (*str) {
		if (*str == (char)c)
			last = str;
		str++;
	}
	return (char *)last;
}

char *str_str(const char *haystack, const char *needle) {
	size_t needle_len;
	if (!haystack || !needle)
		return NULL;
	if (!*needle)
		return (char *)haystack;
	needle_len = str_len(needle);
	while (*haystack) {
		if (!str_ncmp(haystack, needle, needle_len))
			return (char *)haystack;
		haystack++;
	}
	return NULL;
}

/* Additional Search Functions */
char *str_rstr(const char *haystack, const char *needle) {
	char *last = NULL;
	if (!haystack || !needle || !*needle)
		return (char *)haystack;

	while ((haystack = str_str(haystack, needle))) {
		last = (char *)haystack;
		haystack++;
	}
	return last;
}

char *str_pbrk(const char *str, const char *chars) {
	if (!str || !chars)
		return NULL;
	while (*str) {
		const char *c = chars;
		while (*c) {
			if (*str == *c)
				return (char *)str;
			c++;
		}
		str++;
	}
	return NULL;
}

size_t str_span(const char *str, const char *accept) {
	const char *s = str;
	while (*s) {
		const char *a = accept;
		bool found	  = false;
		while (*a) {
			if (*s == *a++) {
				found = true;
				break;
			}
		}
		if (!found)
			break;
		s++;
	}
	return s - str;
}

size_t str_cspan(const char *str, const char *reject) {
	const char *s = str;
	while (*s) {
		const char *r = reject;
		while (*r) {
			if (*s == *r)
				return s - str;
			r++;
		}
		s++;
	}
	return s - str;
}

/* String manipulation */
char *str_upper(char *str) {
	char *s = str;
	if (!str)
		return NULL;
	while (*s) {
		if (*s >= 'a' && *s <= 'z')
			*s -= 32;
		s++;
	}
	return str;
}

char *str_lower(char *str) {
	char *s = str;
	if (!str)
		return NULL;
	while (*s) {
		if (*s >= 'A' && *s <= 'Z')
			*s += 32;
		s++;
	}
	return str;
}

/* Additional String Manipulation */
char *str_capitalize(char *str) {
	if (!str || !*str)
		return str;
	*str = (*str >= 'a' && *str <= 'z') ? *str - 32 : *str;
	return str_lower(str + 1) - 1;
}

char *str_reverse(char *str) {
	if (!str)
		return NULL;
	size_t len = str_len(str);
	for (size_t i = 0; i < len / 2; i++) {
		char tmp		 = str[i];
		str[i]			 = str[len - 1 - i];
		str[len - 1 - i] = tmp;
	}
	return str;
}

char *str_trim(char *str) {
	if (!str)
		return NULL;
	char *end = str + str_len(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
	*(end + 1) = '\0';
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return str;
}

char *str_trim_left(char *str) {
	if (!str)
		return NULL;
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return str;
}

char *str_trim_right(char *str) {
	if (!str)
		return NULL;
	char *end = str + str_len(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
	*(end + 1) = '\0';
	return str;
}

/* Helper function for str_split */
static char *str_token(char *str, const char *delim, char **saveptr) {
	if (!str && !*saveptr)
		return NULL;

	if (!str)
		str = *saveptr;

	// Skip leading delimiters
	str += str_span(str, delim);
	if (*str == '\0') {
		*saveptr = str;
		return NULL;
	}

	// Find end of token
	char *token = str;
	str			= str_pbrk(token, delim);
	if (str) {
		*str	 = '\0';
		*saveptr = str + 1;
	} else {
		*saveptr = token + str_len(token);
	}
	return token;
}

char **str_split(const char *str, const char *delim) {
	if (!str || !delim)
		return NULL;

	size_t count	= 1;
	const char *tmp = str;
	while ((tmp = str_str(tmp, delim))) {
		count++;
		tmp++;
	}

	char **result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;

	char *copy = str_new(str);
	if (!copy) {
		free(result);
		return NULL;
	}

	size_t i	  = 0;
	char *saveptr = NULL;
	char *token	  = str_token(copy, delim, &saveptr);

	while (token) {
		result[i] = str_new(token);
		if (!result[i]) {
			str_array_free(result);
			free(copy);
			return NULL;
		}
		token = str_token(NULL, delim, &saveptr);
		i++;
	}
	result[i] = NULL;
	free(copy);
	return result;
}

char *str_join(const char **strings, const char *delim) {
	if (!strings || !delim)
		return NULL;

	size_t total_len = 0;
	size_t delim_len = str_len(delim);
	size_t count	 = 0;

	for (size_t i = 0; strings[i]; i++) {
		total_len += str_len(strings[i]);
		if (i > 0)
			total_len += delim_len;
		count++;
	}

	char *result = malloc(total_len + 1);
	if (!result)
		return NULL;

	char *ptr = result;
	for (size_t i = 0; i < count; i++) {
		if (i > 0) {
			str_copy(ptr, delim);
			ptr += delim_len;
		}
		size_t len = str_len(strings[i]);
		str_copy(ptr, strings[i]);
		ptr += len;
	}
	*ptr = '\0';
	return result;
}

/* String validation */
bool str_is_alpha(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (!((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z')))
			return false;
		str++;
	}
	return true;
}

bool str_is_digit(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (*str < '0' || *str > '9')
			return false;
		str++;
	}
	return true;
}

/* Additional String Validation */
bool str_is_alnum(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (!(((*str >= '0') && (*str <= '9')) ||
			  ((*str >= 'A') && (*str <= 'Z')) ||
			  ((*str >= 'a') && (*str <= 'z'))))
			return false;
		str++;
	}
	return true;
}

bool str_is_space(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (*str != ' ' && *str != '\t' && *str != '\n' &&
			*str != '\r' && *str != '\f' && *str != '\v')
			return false;
		str++;
	}
	return true;
}

bool str_is_upper(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (*str < 'A' || *str > 'Z')
			return false;
		str++;
	}
	return true;
}

bool str_is_lower(const char *str) {
	if (!str)
		return false;
	while (*str) {
		if (*str < 'a' || *str > 'z')
			return false;
		str++;
	}
	return true;
}

/* Memory management */
void str_array_free(char **array) {
	if (!array)
		return;
	for (size_t i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

size_t str_array_len(char **array) {
	size_t len = 0;
	if (!array)
		return 0;
	while (array[len])
		len++;
	return len;
}

/* Number Conversion Functions */
long str_to_long(const char *str) {
	if (!str)
		return 0;
	long result = 0;
	int sign	= 1;

	if (*str == '-') {
		sign = -1;
		str++;
	}

	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}

	return result * sign;
}

char *long_to_str(long num) {
	char buffer[32];
	int i		  = 30;
	bool negative = num < 0;

	buffer[31] = '\0';
	if (num == 0) {
		buffer[i] = '0';
		return str_new(&buffer[i]);
	}

	if (negative)
		num = -num;

	while (num > 0) {
		buffer[i--] = '0' + (num % 10);
		num /= 10;
	}

	if (negative)
		buffer[i--] = '-';

	return str_new(&buffer[i + 1]);
}

char *double_to_str(double num, int precision) {
	char *result = malloc(32);
	if (!result)
		return NULL;

	snprintf(result, 32, "%.*f", precision, num);
	return result;
}

int str_count_char(const char *str, char c) {
	int count = 0;
	if (!str)
		return 0;

	while (*str) {
		if (*str == c)
			count++;
		str++;
	}
	return count;
}

char *str_substr(const char *str, size_t start, size_t len) {
	if (!str)
		return NULL;

	size_t str_length = str_len(str);
	if (start >= str_length)
		return NULL;

	if (len > str_length - start)
		len = str_length - start;

	char *result = malloc(len + 1);
	if (!result)
		return NULL;

	str_ncopy(result, str + start, len);
	result[len] = '\0';
	return result;
}

char *str_replace(const char *str, const char *old, const char *new) {
	if (!str || !old || !new)
		return NULL;

	size_t old_len = str_len(old);
	if (old_len == 0)
		return str_clone(str);

	// Count occurrences of old string
	size_t count	= 0;
	const char *tmp = str;
	while ((tmp = str_str(tmp, old))) {
		count++;
		tmp += old_len;
	}

	if (count == 0)
		return str_clone(str);

	// Calculate new string length
	size_t new_len	   = str_len(new);
	size_t str_len_val = str_len(str);
	size_t result_len  = str_len_val + count * (new_len - old_len);

	// Allocate memory for result
	char *result = malloc(result_len + 1);
	if (!result)
		return NULL;

	// Build the result string
	char *dest		= result;
	const char *src = str;
	while (*src) {
		if (str_ncmp(src, old, old_len) == 0) {
			str_copy(dest, new);
			src += old_len;
			dest += new_len;
		} else {
			*dest++ = *src++;
		}
	}
	*dest = '\0';

	return result;
}

int str_to_int(const char *str) {
	if (!str)
		return 0;

	int result = 0;
	int sign   = 1;

	// Handle sign
	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	// Convert digits
	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}

	return result * sign;
}

double str_to_double(const char *str) {
	if (!str)
		return 0.0;

	double result	= 0.0;
	double fraction = 0.0;
	double power	= 1.0;
	int sign		= 1;

	// Handle sign
	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	// Process integer part
	while (*str >= '0' && *str <= '9') {
		result = result * 10.0 + (*str - '0');
		str++;
	}

	// Process fractional part
	if (*str == '.') {
		str++;
		while (*str >= '0' && *str <= '9') {
			power *= 10.0;
			fraction = fraction + (*str - '0') / power;
			str++;
		}
	}

	return sign * (result + fraction);
}

char *int_to_str(int num) {
	char buffer[12]; // Enough for 32-bit int including sign and null terminator
	int i		  = 10;
	bool negative = num < 0;

	buffer[11] = '\0';

	if (num == 0) {
		buffer[i] = '0';
		return str_new(&buffer[i]);
	}

	if (negative)
		num = -num;

	while (num > 0) {
		buffer[i--] = '0' + (num % 10);
		num /= 10;
	}

	if (negative)
		buffer[i--] = '-';

	return str_new(&buffer[i + 1]);
}
