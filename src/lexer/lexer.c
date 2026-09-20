#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <ctype.h>

#include <rayutils/diagnostics/log.h>
#include <rayutils/types.h>

#include "raycli/extension/rayutils/diagnostics/log.h"
#include "raycli/lexer/lexer.h"

RAY_STACK_IMPLEMENTATION(RayCLIToken, RayCLIToken, raycli_token, NULL)

static RayBool raycli_lexer_tokenize_next(RayCLILexer *const lexer, RayCLIToken *const token);
static RayBool raycli_lexer_tokenize_option(RayCLILexer *const lexer, RayCLIToken *const token);
static RayCLIToken raycli_lexer_tokenize_identifier(RayCLILexer *const lexer, const RayU8 offset);
static char raycli_lexer_get_current_char(RayCLILexer *const lexer);

RayCLILexer raycli_lexer_make(const int argc, char **const argv) {
	assert(argv != NULL);
	assert(*argv != NULL);

	return (RayCLILexer) {
		argc, argv,

		.span = (RaySpan) {
			.stream = "command line",
			.function = NULL,
			.line = 1,
			.column = strlen(argv[0]) + 1,
		},

		.argv_index = 1,
		.word_index = 0,
	};
}

RayBool raycli_lexer_tokenize(RayCLILexer *const lexer, RayCLITokenStack **tokens) {
	assert(lexer != NULL);

	if (!raycli_token_stack_create(tokens)) {
		return false;
	}

	while (lexer->argv_index < (size_t) lexer->argc) {
		RayCLIToken next_token;

		if (!raycli_lexer_tokenize_next(lexer, &next_token) || !raycli_token_stack_push(tokens, next_token)) {
			raycli_token_stack_free(tokens);
			return false;
		}

		lexer->argv_index++;
		lexer->word_index = 0;
	}

	const RayCLIToken token = {
		.kind = RAYCLI_TOKEN_KIND_END_OF_LINE,
		.text = NULL,
		.span = lexer->span,
	};

	if (!raycli_token_stack_push(tokens, token)) {
		raycli_token_stack_free(tokens);
		return false;
	}

	return true;
}

RayBool raycli_lexer_tokenize_next(RayCLILexer *const lexer, RayCLIToken *const token) {
	assert(lexer != NULL);
	assert(token != NULL);

	RayBool return_value = true;

	/*
	 *	Trim the text before using it, useful for quoted arguments, which also
	 *	prevents whitespace-only arguments. Then the offset is applied for
	 *	options.
	 */

	char **text = &lexer->argv[lexer->argv_index];
	size_t text_length = strlen(*text);

	while (isspace(*(*text))) ++(*text);

	text_length = strlen(*text);

	while (text_length > 0 && isspace((*text)[text_length - 1])) {
		(*text)[text_length - 1] = '\0';
		text_length = strlen(*text);
	}

	if (raycli_lexer_get_current_char(lexer) == '-') {
		return_value = raycli_lexer_tokenize_option(lexer, token);
	} else {
		*token =  raycli_lexer_tokenize_identifier(lexer, 0);
	}

	if (text_length == 0) {
		RAYCLI_LOG_CMDLINE(
			RAY_LOG_METADATA_OPTION_ERROR,
			RAY_LOG_METADATA_CODE(RAYCLI_LOG_METADATA_CODE_SYS_CMDLINE, RAYCLI_LOG_METADATA_CODE_CAT_CMDLINE_LEX, RAYCLI_LOG_METADATA_CODE_REA_CMDLINE_EMPTIDEN),
			stderr,
			lexer->argv_index,
			"Cannot pass an empty %s.\n",
			token->kind == RAYCLI_TOKEN_KIND_IDENTIFIER
			? "identifier" : "option"
		);

		return false;
	}

	lexer->span.column += text_length + 2;
	return return_value;
}

RayBool raycli_lexer_tokenize_option(RayCLILexer *const lexer, RayCLIToken *const token) {
	assert(lexer != NULL);
	assert(token != NULL);

	lexer->word_index += 1;

	if (raycli_lexer_get_current_char(lexer) == '-') {
		/* Long option. */
		lexer->word_index += 1;
		*token = raycli_lexer_tokenize_identifier(lexer, 2);
		token->kind = RAYCLI_TOKEN_KIND_LONG_OPTION;
		return true;
	}

	/* Short option. */
	*token = raycli_lexer_tokenize_identifier(lexer, 1);

	if (strlen(token->text) > 1) {
		RAYCLI_LOG_CMDLINE(
			RAY_LOG_METADATA_OPTION_ERROR,
			RAY_LOG_METADATA_CODE(RAYCLI_LOG_METADATA_CODE_SYS_CMDLINE, RAYCLI_LOG_METADATA_CODE_CAT_CMDLINE_LEX, RAYCLI_LOG_METADATA_CODE_REA_CMDLINE_LNGSHRTOPT),
			stderr,
			lexer->argv_index,
			"Short option `-%s' is too long.\n",
			token->text
		);

		return false;
	}

	token->kind = RAYCLI_TOKEN_KIND_SHORT_OPTION;
	return true;
}

RayCLIToken raycli_lexer_tokenize_identifier(RayCLILexer *const lexer, const RayU8 offset) {
	return (RayCLIToken) {
		.kind = RAYCLI_TOKEN_KIND_IDENTIFIER,
		.text = &lexer->argv[lexer->argv_index][offset],
		.span = lexer->span,
	};
}

char raycli_lexer_get_current_char(RayCLILexer *const lexer) {
	assert(lexer != NULL);
	return lexer->argv[lexer->argv_index][lexer->word_index];
}
