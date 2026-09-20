#include <assert.h>

#include "raycli/parser/parser.h"

static RayCLIToken *raycli_parser_advance_and_get_token(RayCLIParser *const parser);
static RayCLIToken *raycli_parser_get_current_token(RayCLIParser *const parser);

RayCLIParser raycli_parser_make(RayCLITokenStack *const tokens) {
	assert(tokens != NULL);

	return (RayCLIParser) {
		tokens,
		.index = 0,
	};
}

RayBool raycli_parser_parse(RayCLIParser *const parser, RayCLICmdline *cmdline) {
	assert(parser != NULL);

	RayCLIToken *current_token = raycli_parser_get_current_token(parser);
	RayCLITokenStack *general_options = NULL;
	RayCLIToken *command = NULL;
	RayCLITokenStack *options = NULL;
	RayCLITokenStack *arguments = NULL;

	while (current_token->kind != RAYCLI_TOKEN_KIND_END_OF_LINE && current_token->kind != RAYCLI_TOKEN_KIND_IDENTIFIER) {
		if (general_options == NULL && !raycli_token_stack_create(&general_options)) {
			return false;
		}

		if (!raycli_token_stack_push(&general_options, *current_token)) {
			raycli_token_stack_free(&general_options);
			return false;
		}

		current_token = raycli_parser_advance_and_get_token(parser);
	}

	if (current_token->kind == RAYCLI_TOKEN_KIND_END_OF_LINE) {
		goto end;
	}

	command = current_token;
	current_token = raycli_parser_advance_and_get_token(parser);

	while (current_token->kind != RAYCLI_TOKEN_KIND_END_OF_LINE) {
		switch (current_token->kind) {
			case RAYCLI_TOKEN_KIND_IDENTIFIER: {
				if (arguments == NULL && !raycli_token_stack_create(&arguments)) {
					return false;
				}

				if (!raycli_token_stack_push(&arguments, *current_token)) {
					raycli_token_stack_free(&arguments);
					return false;
				}
			} break;

			case RAYCLI_TOKEN_KIND_SHORT_OPTION:
			case RAYCLI_TOKEN_KIND_LONG_OPTION: {
				if (options == NULL && !raycli_token_stack_create(&options)) {
					return false;
				}

				if (!raycli_token_stack_push(&options, *current_token)) {
					raycli_token_stack_free(&options);
					return false;
				}
			} break;

			default: break;
		}

		current_token = raycli_parser_advance_and_get_token(parser);
	}

end:
	*cmdline = (RayCLICmdline) { general_options, command, options, arguments };
	return true;
}

RayCLIToken *raycli_parser_advance_and_get_token(RayCLIParser *const parser) {
	assert(parser != NULL);
	parser->index += 1;
	return raycli_parser_get_current_token(parser);
}

RayCLIToken *raycli_parser_get_current_token(RayCLIParser *const parser) {
	assert(parser != NULL);
	return &parser->tokens->elements[parser->index];
}
