/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hc_argsparser.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:16:42 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 11:08:46 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HC_ARGSPARSER_H
#define HC_ARGSPARSER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Option {
	char *name;		   // Option name (e.g., "-h", "--help")
	char short_name;   // Option short name (e.g., "-h")
	char *description; // Option description

	bool has_arg;  // Does the option have an argument?
	bool required; // Is the option required?

	int (*callback)(char *); // Callback function
} Option;

typedef struct ParserConfig {
	void (*err_opt_required)(const char *opt);
	void (*err_opt_arg)(const char *opt);
	void (*err_opt_unknown)(const char *opt);
} ParserConfig;

typedef struct ParserContext {
	const ParserConfig *config; // Parser configuration

	int argc;	 // Number of arguments
	char **argv; // Arguments

	size_t opt_idx; // Current option index

	Option *options;  // Options
	size_t n_options; // Number of options
} ParserContext;

#define PROGRAM_NAME(ctx) ((ctx)->argv[0])

/**
 * @brief Initializes a new parser context for command line arguments
 *
 * @param argc The argument count received from main
 * @param argv The argument vector received from main
 *
 * @return ParserContext* A pointer to the newly initialized parser context
 *                       or NULL if initialization fails
 *
 * @note The returned parser context must be freed using hc_parser_free() when no longer needed
 */
ParserContext *hc_parser_init(int argc, char **argv, const ParserConfig *config);

/**
 * @brief Adds a new option to the parser context
 *
 * @param ctx The parser context to add the option to
 * @param name The long name of the option (e.g., "help")
 * @param short_name The short name of the option (e.g., "h")
 * @param description A description of what the option does
 * @param has_arg Whether the option requires an argument
 * @param required Whether the option is required
 * @param callback Function to be called when the option is parsed (can be NULL)
 *
 * @return true if the option was successfully added, false otherwise
 */
bool hc_parser_add_option(ParserContext *ctx, char *name, char short_name, char *description, bool has_arg, bool required, int (*callback)(char *));

/**
 * @brief Parses command line arguments according to the parser context configuration
 *
 * @param ctx Pointer to a ParserContext structure containing parser configuration
 *
 * @return true if parsing was successful
 * @return false if parsing failed or encountered errors
 *
 * The function processes command line arguments based on the rules and options
 * defined in the parser context. It validates arguments and populates the
 * corresponding data structures within the context.
 */
bool hc_parser_parse(ParserContext *ctx);

/**
 * @brief Deallocates resources associated with a parser context
 *
 * @param ctx Pointer to the ParserContext structure to be freed
 *
 * This function cleans up and frees all memory allocated for the parser context.
 * After calling this function, the context pointer should not be used.
 */
void hc_parser_free(ParserContext *ctx);

/**
 * @brief Displays help information for command line arguments
 *
 * Prints usage information and available command line options based on
 * the configuration stored in the ParserContext.
 *
 * @param ctx Pointer to the ParserContext containing argument parsing configuration
 */
void hc_display_help(ParserContext *ctx);

#endif // HC_ARGSPARSER_H