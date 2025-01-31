/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_argparser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:31:42 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 11:06:10 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <hypercore.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static void _err_opt_required(const char *opt);
static void _err_opt_arg(const char *opt);
static void _err_opt_unknown(const char *opt);

const ParserConfig config = {
	.err_opt_required = _err_opt_required,
	.err_opt_arg	  = _err_opt_arg,
	.err_opt_unknown  = _err_opt_unknown,
};

static int _help_callback(char *arg) {
	(void)arg;
	printf("Usage: ./test_argparser [OPTIONS]\n");
	printf("Options:\n");
	printf("  --help, -h     Display this help message\n");
	printf("  --file, -f     Specify input file (required)\n");

	return (0);
}

static int _file_callback(char *arg) {
	if (access(arg, F_OK) != 0) {
		fprintf(stderr, "Error: File '%s' does not exist\n", arg);
		return (1);
	}
	printf("File specified: %s\n", arg);
	return (0);
}

static void _err_opt_required(const char *opt) {
	fprintf(stderr, "Error: Option %s is required\n", opt);
}

static void _err_opt_arg(const char *opt) {
	fprintf(stderr, "Error: Option %s requires an argument\n", opt);
}

static void _err_opt_unknown(const char *opt) {
	fprintf(stderr, "Error: Unknown option %s\n", opt);
}

int main(int argc, char **argv) {
	ParserContext *ctx = hc_parser_init(argc, argv, &config);
	if (ctx == NULL) return 1;

	// Add options
	hc_parser_add_option(ctx, "help", 'h', "Display this help message", false, false, _help_callback);
	hc_parser_add_option(ctx, "file", 'f', "Specify input file", true, true, _file_callback);

	// Parse and check result
	if (!hc_parser_parse(ctx)) {
		// _help_callback(NULL);
		hc_display_help(ctx);
		hc_parser_free(ctx);
		return 1;
	}

	hc_parser_free(ctx);
	return 0;
}