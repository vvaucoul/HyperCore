/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:54:53 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 12:33:55 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_H
#define STRINGS_H

#include <stdbool.h>
#include <stddef.h>

/* Basic string operations */
size_t str_len(const char *str);
size_t str_nlen(const char *str, size_t maxlen);
bool str_empty(const char *str);
char *str_new(const char *str);
void str_clear(char *str);
char *str_clone(const char *str);

/* Comparison functions */
int str_cmp(const char *s1, const char *s2);
int str_ncmp(const char *s1, const char *s2, size_t n);
int str_case_cmp(const char *s1, const char *s2);
bool str_equals(const char *s1, const char *s2);
bool str_starts_with(const char *str, const char *prefix);
bool str_ends_with(const char *str, const char *suffix);

/* Copy and concatenation */
char *str_copy(char *dest, const char *src);
char *str_ncopy(char *dest, const char *src, size_t n);
char *str_cat(char *dest, const char *src);
char *str_ncat(char *dest, const char *src, size_t n);
size_t str_lcpy(char *dest, const char *src, size_t size);
size_t str_lcat(char *dest, const char *src, size_t size);

/* Search functions */
char *str_chr(const char *str, int c);
char *str_rchr(const char *str, int c);
char *str_str(const char *haystack, const char *needle);
char *str_rstr(const char *haystack, const char *needle);
char *str_pbrk(const char *str, const char *chars);
size_t str_span(const char *str, const char *accept);
size_t str_cspan(const char *str, const char *reject);

/* String manipulation */
char *str_upper(char *str);
char *str_lower(char *str);
char *str_capitalize(char *str);
char *str_reverse(char *str);
char *str_trim(char *str);
char *str_trim_left(char *str);
char *str_trim_right(char *str);
char **str_split(const char *str, const char *delim);
char *str_join(const char **strings, const char *delim);
char *str_replace(const char *str, const char *old, const char *new);
char *str_substr(const char *str, size_t start, size_t len);

/* Number conversion */
int str_to_int(const char *str);
long str_to_long(const char *str);
double str_to_double(const char *str);
char *int_to_str(int num);
char *long_to_str(long num);
char *double_to_str(double num, int precision);

/* String validation */
bool str_is_alpha(const char *str);
bool str_is_digit(const char *str);
bool str_is_alnum(const char *str);
bool str_is_space(const char *str);
bool str_is_upper(const char *str);
bool str_is_lower(const char *str);
int str_count_char(const char *str, char c);

/* Memory management */
void str_array_free(char **array);
size_t str_array_len(char **array);

#endif // STRINGS_H
