/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:25:51 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/31 12:32:09 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <hypercore.h>
#include <lib/strings/strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test helper functions */
#define TEST_START(name) printf("Testing %s...\n", name)
#define TEST_END(name) printf("%s: OK\n", name)
#define ASSERT(condition) assert(condition)

/* Basic string operations tests */
static void test_basic_operations(void) {
	TEST_START("Basic Operations");

	// str_len
	ASSERT(str_len("hello") == 5);
	ASSERT(str_len("") == 0);
	ASSERT(str_len(NULL) == 0);

	// str_nlen
	ASSERT(str_nlen("hello", 3) == 3);
	ASSERT(str_nlen("hi", 5) == 2);

	// str_empty
	ASSERT(str_empty("") == true);
	ASSERT(str_empty(NULL) == true);
	ASSERT(str_empty("hello") == false);

	// str_new & str_clear
	char *new_str = str_new("hello");
	ASSERT(str_equals(new_str, "hello"));
	str_clear(new_str);
	ASSERT(str_empty(new_str));
	free(new_str);

	// str_clone
	char *clone = str_clone("test");
	ASSERT(str_equals(clone, "test"));
	free(clone);

	TEST_END("Basic Operations");
}

/* Comparison functions tests */
static void test_comparison_functions(void) {
	TEST_START("Comparison Functions");

	// str_cmp
	ASSERT(str_cmp("hello", "hello") == 0);
	ASSERT(str_cmp("abc", "def") < 0);
	ASSERT(str_cmp("def", "abc") > 0);

	// str_ncmp
	ASSERT(str_ncmp("hello", "help", 3) == 0);
	ASSERT(str_ncmp("abc", "abx", 2) == 0);

	// str_case_cmp
	ASSERT(str_case_cmp("Hello", "hello") == 0);
	ASSERT(str_case_cmp("WORLD", "world") == 0);

	// str_equals
	ASSERT(str_equals("test", "test") == true);
	ASSERT(str_equals("test", "Test") == false);

	// str_starts_with & str_ends_with
	ASSERT(str_starts_with("hello world", "hello"));
	ASSERT(str_ends_with("hello world", "world"));

	TEST_END("Comparison Functions");
}

/* Copy and concatenation tests */
static void test_copy_concatenation(void) {
	TEST_START("Copy and Concatenation");

	char dest[20];
	memset(dest, 0, sizeof(dest)); // Initialize buffer with zeros

	// str_copy
	str_copy(dest, "hello");
	ASSERT(str_equals(dest, "hello"));

	// str_ncopy
	memset(dest, 0, sizeof(dest)); // Reset buffer
	str_ncopy(dest, "world", 3);
	dest[3] = '\0'; // Ensure null termination
	ASSERT(str_equals(dest, "wor"));

	// str_cat
	str_copy(dest, "hello");
	str_cat(dest, " world");
	ASSERT(str_equals(dest, "hello world"));

	// str_ncat
	str_copy(dest, "test");
	str_ncat(dest, "123456", 3);
	ASSERT(str_equals(dest, "test123"));

	TEST_END("Copy and Concatenation");
}

/* Search functions tests */
static void test_search_functions(void) {
	TEST_START("Search Functions");

	// str_chr & str_rchr
	const char *test = "hello world";
	ASSERT(*str_chr(test, 'l') == 'l');
	ASSERT(*str_rchr(test, 'l') == 'l');

	// str_str & str_rstr
	ASSERT(str_equals(str_str("hello world", "world"), "world"));
	ASSERT(str_equals(str_rstr("hello world world", "world"), "world"));

	// str_pbrk
	ASSERT(*str_pbrk("hello", "lo") == 'l');

	// str_span & str_cspan
	ASSERT(str_span("12345abc", "123456789") == 5);
	ASSERT(str_cspan("12345abc", "abc") == 5);

	TEST_END("Search Functions");
}

/* String manipulation tests */
static void test_string_manipulation(void) {
	TEST_START("String Manipulation");

	// str_upper & str_lower
	char str1[] = "Hello";
	str_upper(str1);
	ASSERT(str_equals(str1, "HELLO"));
	str_lower(str1);
	ASSERT(str_equals(str1, "hello"));

	// str_capitalize
	char str2[] = "hello world";
	str_capitalize(str2);
	ASSERT(str_equals(str2, "Hello world"));

	// str_reverse
	char str3[] = "hello";
	str_reverse(str3);
	ASSERT(str_equals(str3, "olleh"));

	// str_trim functions
	char *trimmed = str_new("  hello  ");
	char *result  = str_trim(trimmed);
	ASSERT(str_equals(result, "hello"));
	free(trimmed);

	// str_split & str_join
	char **split = str_split("hello,world", ",");
	ASSERT(str_equals(split[0], "hello"));
	ASSERT(str_equals(split[1], "world"));

	const char *parts[] = {"hello", "world", NULL};
	char *joined		= str_join(parts, " ");
	ASSERT(str_equals(joined, "hello world"));

	str_array_free(split);
	free(joined);

	// str_replace
	char *replaced = str_replace("hello world", "world", "there");
	ASSERT(str_equals(replaced, "hello there"));
	free(replaced);

	// str_substr
	char *sub = str_substr("hello world", 6, 5);
	ASSERT(str_equals(sub, "world"));
	free(sub);

	TEST_END("String Manipulation");
}

/* Number conversion tests */
static void test_number_conversion(void) {
	TEST_START("Number Conversion");

	// str_to_int & int_to_str
	ASSERT(str_to_int("123") == 123);
	ASSERT(str_to_int("-456") == -456);
	char *num_str = int_to_str(789);
	ASSERT(str_equals(num_str, "789"));
	free(num_str);

	// str_to_long & long_to_str
	ASSERT(str_to_long("123456789") == 123456789L);
	char *long_str = long_to_str(987654321L);
	ASSERT(str_equals(long_str, "987654321"));
	free(long_str);

	// str_to_double & double_to_str
	ASSERT(str_to_double("123.456") == 123.456);
	char *double_str = double_to_str(123.456, 3);
	ASSERT(str_equals(double_str, "123.456"));
	free(double_str);

	TEST_END("Number Conversion");
}

/* String validation tests */
static void test_string_validation(void) {
	TEST_START("String Validation");

	// str_is_alpha
	ASSERT(str_is_alpha("Hello") == true); // Changed to true since both cases are alphabetic
	ASSERT(str_is_alpha("hello") == true);
	ASSERT(str_is_alpha("hello123") == false);
	ASSERT(str_is_alpha("hello!") == false);

	// str_is_digit
	ASSERT(str_is_digit("123") == true);
	ASSERT(str_is_digit("12.3") == false);

	// str_is_alnum
	ASSERT(str_is_alnum("hello123") == true);
	ASSERT(str_is_alnum("hello 123") == false);

	// str_is_space
	ASSERT(str_is_space("   \t\n") == true);
	ASSERT(str_is_space("   x  ") == false);

	// str_is_upper & str_is_lower
	ASSERT(str_is_upper("HELLO") == true);
	ASSERT(str_is_lower("hello") == true);

	// str_count_char
	ASSERT(str_count_char("hello", 'l') == 2);
	ASSERT(str_count_char("world", 'x') == 0);

	TEST_END("String Validation");
}

/* Memory management tests */
static void test_memory_management(void) {
	TEST_START("Memory Management");

	// str_array_len
	const char *arr[] = {"hello", "world", NULL};
	ASSERT(str_array_len((char **)arr) == 2);

	// str_array_free (testing with allocation)
	char **array = malloc(sizeof(char *) * 3);
	array[0]	 = str_new("test1");
	array[1]	 = str_new("test2");
	array[2]	 = NULL;
	str_array_free(array);

	TEST_END("Memory Management");
}

/* Fixed null handling tests */
static void test_null_handling(void) {
	TEST_START("Null Pointer Handling");

	ASSERT(str_len(NULL) == 0);
	char *copy_result = str_copy(NULL, "test");
	ASSERT(copy_result == NULL);
	char *chr_result = str_chr(NULL, 'a');
	ASSERT(chr_result == NULL);
	char *upper_result = str_upper(NULL);
	ASSERT(upper_result == NULL);
	ASSERT(str_is_alpha(NULL) == false);

	TEST_END("Null Pointer Handling");
}

/* Fixed edge cases tests */
static void test_edge_cases(void) {
	TEST_START("Edge Cases");

	// Empty string handling
	ASSERT(str_len("") == 0);
	ASSERT(str_empty("") == true); // Changed from str_is_empty to str_empty

	// Long string handling
	char long_str[1000];
	memset(long_str, 'a', 999);
	long_str[999] = '\0';
	ASSERT(str_len(long_str) == 999);

	// Special characters
	char *null_result = str_chr("test\0hidden", '\0');
	ASSERT(null_result != NULL);

	TEST_END("Edge Cases");
}

/* Main test function */
int main(void) {
	printf("Starting String Library Tests\n");
	printf("============================\n\n");

	test_basic_operations();
	test_comparison_functions();
	test_copy_concatenation();
	test_search_functions();
	test_string_manipulation();
	test_number_conversion();
	test_string_validation();
	test_memory_management();
	test_null_handling();
	test_edge_cases();

	printf("\nAll tests passed successfully!\n");
	return 0;
}
