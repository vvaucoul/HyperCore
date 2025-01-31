/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argsparser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:14:31 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:20:37 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/argsparser.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParserContext *hc_parser_init(int argc, char **argv, const ParserConfig *config) {
	ParserContext *ctx = malloc(sizeof(ParserContext));
	if (ctx == NULL) return (NULL);

	ctx->config	   = config;
	ctx->argc	   = argc;
	ctx->argv	   = argv;
	ctx->opt_idx   = 1; // Ignore first argument (program name)
	ctx->options   = NULL;
	ctx->n_options = 0;

	return (ctx);
}

bool hc_parser_add_option(ParserContext *ctx, char *name, char short_name, char *description, bool has_arg, bool required, int (*callback)(char *)) {
	if (ctx == NULL) return (false);

	// Reallocate options
	size_t new_n_options = ctx->n_options + 1;
	Option *new_options	 = realloc(ctx->options, new_n_options * sizeof(Option));
	if (new_options == NULL) return (false);

	// Update context
	ctx->options   = new_options;
	ctx->n_options = new_n_options;

	// Configure new option
	Option *opt = &ctx->options[new_n_options - 1];

	opt->name		 = strdup(name);
	opt->short_name	 = short_name;
	opt->description = strdup(description);
	opt->has_arg	 = has_arg;
	opt->required	 = required;
	opt->callback	 = callback;

	return (true);
}

static bool _parse_option(ParserContext *ctx, const char *opt) {
	if (ctx == NULL) return (false);

	size_t i				= 0;
	const char *option_name = opt;
	const char *arg_value	= NULL;
	char *arg_copy			= NULL;

	// Check for attached argument (e.g. -f=file.txt or --file=file.txt)
	char *equals = strchr(opt, '=');
	if (equals) {
		*equals	  = '\0'; // Temporarily split the string
		arg_value = equals + 1;
	}

	// Skip the leading dashes for comparison
	if (strncmp(opt, "--", 2) == 0) {
		option_name += 2;
	} else if (opt[0] == '-') {
		option_name += 1;
	} else {
		ctx->config->err_opt_unknown(opt);
		return false;
	}

	for (i = 0; i < ctx->n_options; i++) {
		Option *option = &ctx->options[i];

		// Check if it matches the long name (--option) or short name (-o)
		if ((strcmp(option_name, option->name) == 0) ||
			(opt[0] == '-' && opt[1] != '-' && opt[1] == option->short_name)) {

			if (option->callback != NULL) {
				int callback_ret = 0;
				if (option->has_arg) {
					if (arg_value) {
						arg_copy = strdup(arg_value);
						if (arg_copy) {
							callback_ret = option->callback(arg_copy);
							free(arg_copy);
						}
						if (equals) *equals = '=';
						return callback_ret == 0;
					} else if (ctx->opt_idx < (size_t)ctx->argc) {
						arg_copy = strdup(ctx->argv[ctx->opt_idx++]);
						if (arg_copy) {
							callback_ret = option->callback(arg_copy);
							free(arg_copy);
						}
						return callback_ret == 0;
					} else {
						ctx->config->err_opt_arg(opt);
						return false;
					}
				} else {
					callback_ret = option->callback(NULL);
					if (equals) *equals = '=';
					return callback_ret == 0;
				}
			}
		}
	}
	if (equals) *equals = '=';
	ctx->config->err_opt_unknown(opt);
	return false;
}

static bool _check_required_options(ParserContext *ctx) {
	if (ctx == NULL) return false;

	for (size_t i = 0; i < ctx->n_options; i++) {
		Option *option = &ctx->options[i];
		if (option->required) {
			// Check if this option was used
			bool found = false;
			for (int j = 1; j < ctx->argc; j++) {
				if ((strncmp(ctx->argv[j], "--", 2) == 0 && strcmp(ctx->argv[j] + 2, option->name) == 0) ||
					(ctx->argv[j][0] == '-' && ctx->argv[j][1] == option->short_name)) {
					found = true;
					break;
				}
			}
			if (!found) {
				ctx->config->err_opt_required(option->name);
				return false;
			}
		}
	}
	return true;
}

bool hc_parser_parse(ParserContext *ctx) {
	if (ctx == NULL) return false;

	bool success = true;
	while (ctx->opt_idx < (size_t)ctx->argc) {
		const char *arg = ctx->argv[ctx->opt_idx++];

		// Only parse as option if it starts with '-'
		if (arg[0] == '-') {
			if (!_parse_option(ctx, arg)) {
				success = false;
				break;
			}
		} else {
			// Handle non-option argument
			ctx->config->err_opt_unknown(arg);
			success = false;
			break;
		}
	}

	// Check required options only if parsing was successful
	if (success) {
		success = _check_required_options(ctx);
	}

	return success;
}

void hc_parser_free(ParserContext *ctx) {
	if (ctx != NULL) {
		size_t i;
		for (i = 0; i < ctx->n_options; ++i) {
			free(ctx->options[i].name);
			free(ctx->options[i].description);
		}
		free(ctx->options);
		free(ctx);
	}
}

void hc_display_help(ParserContext *ctx) {
	if (ctx == NULL) return;

	printf("Usage: %s [OPTIONS]\n", PROGRAM_NAME(ctx));
	printf("Options:\n");

	for (size_t i = 0; i < ctx->n_options; i++) {
		Option *option = &ctx->options[i];
		printf("  --%s, -%c\t%s%s\n",
			   option->name,
			   option->short_name,
			   option->description,
			   option->required ? " (required)" : "");
	}
}