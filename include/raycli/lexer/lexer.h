/*
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYCLI_LEXER_LEXER_H
#define RAYCLI_LEXER_LEXER_H

#include <rayutils/stack.h>

#include "raycli/lexer/token.h"

RAY_STACK_DECLARATION(RayCLIToken, RayCLIToken, raycli_token)

typedef struct {
	int argc;
	char **argv;
	RaySpan span;
	size_t argv_index;
	size_t word_index;
} RayCLILexer;

RayCLILexer raycli_lexer_make(const int argc, char **const argv);
RayBool raycli_lexer_tokenize(RayCLILexer *const lexer, RayCLITokenStack **tokens);

#endif /* RAYCLI_LEXER_LEXER_H */
